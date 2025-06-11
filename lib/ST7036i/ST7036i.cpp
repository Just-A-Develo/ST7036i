#include <Arduino.h>
#include <ST7036i.h>
#include <Wire.h>

int RES = 22;
int ASDA = 20;
int ASCL = 21;

const char text1[] = {"  Newhaven Display  "};
const char text2[] = {" Clear Display Test "};
const char text3[] = {"    COG  Display    "};
const char text4[] = {"  2x20  Characters  "};

const char slave2w = 0x3C; // 3E for CiZ (0x7C shifted over 1 bit)  //3C for BiZ (0x78 shifted over 1 bit)
const char comsend = 0x00;
const char datasend = 0x40;
const char line2 = 0xC0;

uint8_t cursorCol = 0;
uint8_t cursorRow = 0;

void showText(const char *text)
{
  for (size_t i = 0; i < strlen(text); i++)
  {
    writeChar(text[i]);
  }
}

void updateCursor(int charsWritten)
{
  cursorCol += charsWritten;
  if (cursorCol >= 20)
  { // Max 20 kolommen per rij
    cursorCol = 0;
    cursorRow = (cursorRow + 1) % 2; // Spring naar volgende rij, max 2 rijen
  }
}

DisplayWriter::DisplayWriter(const char *t)
{
  text = t;
}

void DisplayWriter::show() const
{
  showText(text);
}

DisplayWriter show(const char *text)
{
  return DisplayWriter(text);
}

DisplayWriter &DisplayWriter::at(uint8_t col, uint8_t row)
{
  setCursor(col, row);
  show();
  return *this;
}

DisplayWriter &DisplayWriter::center()
{
  int len = strlen(text);
  int padding = (20 - len) / 2;
  setCursor(padding, cursorRow);
  show();
  return *this;
}

DisplayWriter &DisplayWriter::right()
{
  int len = strlen(text);
  int padding = 20 - len;
  setCursor(padding, cursorRow);
  show();
  return *this;
}

DisplayWriter &DisplayWriter::left()
{
  setCursor(cursorCol, cursorRow); // Of zet cursor op huidige positie, geen verschuiving
  show();
  return *this;
}

void writeChar(char c)
{
  Wire.beginTransmission(slave2w);
  Wire.write(datasend);
  Wire.write(c);
  Wire.endTransmission();

  updateCursor(1);
}

void setCursor(uint8_t kolom, uint8_t rij)
{
  const uint8_t row_offsets[] = {0x00, 0x40};
  if (rij > 1)
    rij = 1;

  cursorCol = kolom;
  cursorRow = rij;

  Wire.beginTransmission(slave2w);
  Wire.write(comsend);
  Wire.write(0x80 | (kolom + row_offsets[rij]));
  Wire.endTransmission();
  delay(5);
}

void scrollLeft()
{
  Wire.beginTransmission(slave2w);
  Wire.write(comsend);
  Wire.write(0x18); // Cursor or display shift, display shift left
  Wire.endTransmission();
}

void scrollRight()
{
  Wire.beginTransmission(slave2w);
  Wire.write(comsend);
  Wire.write(0x1C); // Cursor or display shift, display shift right
  Wire.endTransmission();
}

void displayOn(bool enable)
{
  Wire.beginTransmission(slave2w);
  Wire.write(comsend);
  Wire.write(enable ? 0x50 : 0x54);
  Wire.endTransmission();
}

void blinkCursor(bool enable)
{
  Wire.beginTransmission(slave2w);
  Wire.write(comsend);
  Wire.write(enable ? 0x0D : 0x0C); // Display ON + Cursor ON + Blink ON/OFF
  Wire.endTransmission();
}

void cursorOn()
{
  Wire.beginTransmission(slave2w);
  Wire.write(comsend);
  Wire.write(0x0E); // Display ON + Cursor ON + Blink OFF
  Wire.endTransmission();
}

void cursorOff()
{
  Wire.beginTransmission(slave2w);
  Wire.write(comsend);
  Wire.write(0x0C); // Display ON + Cursor OFF + Blink OFF
  Wire.endTransmission();
}

void nextline(void)
{
  Wire.beginTransmission(slave2w);
  Wire.write(comsend);
  Wire.write(line2);
  Wire.endTransmission();
  updateCursor(20);
}

void cleardisplay(void)
{
  Wire.beginTransmission(slave2w);
  Wire.write(comsend);
  Wire.write(0x01);
  Wire.endTransmission();
}

void home()
{
  Wire.beginTransmission(slave2w);
  Wire.write(comsend);
  Wire.write(0x02); // Return home instructie
  Wire.endTransmission();
  cursorCol = 0;
  cursorRow = 0;
}

void CiZ_init()
{
  Wire.beginTransmission(slave2w);
  Wire.write(comsend);
  Wire.write(0x39);
  delay(1);
  Wire.write(0x14);
  Wire.write(0x70);
  Wire.write(0x5E);
  Wire.write(0x6D);
  Wire.write(0x0C);
  Wire.write(0x01);
  Wire.write(0x06);
  Wire.endTransmission();
}

void LCDinit()
{
  pinMode(RES, OUTPUT);
  pinMode(ASCL, OUTPUT);
  pinMode(ASDA, OUTPUT);
  digitalWrite(RES, HIGH);
  delay(10);
  CiZ_init();
  delay(5);
}

void test()
{
  show(text1);
  nextline();
  show(text2);
  delay(1500);
  cleardisplay();
  delay(2);

  show(text3);
  nextline();
  show(text4);
  delay(1500);
  cleardisplay();
  delay(2);
}