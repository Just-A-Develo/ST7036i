#ifndef ST7036I_H
#define ST7036I_H

#include <Arduino.h>

/**
 * @brief I2C address and command mode definitions for the LCD controller.
 */
extern const char slave2w;   ///< I2C slave address of the LCD
extern const char comsend;   ///< Prefix for sending commands to the LCD
extern const char datasend;  ///< Prefix for sending data (characters) to the LCD

/**
 * @brief Global cursor tracking variables for 2x20 character LCD.
 */
extern uint8_t cursorRow;    ///< Current row position (0 or 1)
extern uint8_t cursorCol;    ///< Current column position (0 to 19)

/**
 * @brief Enable or disable the display
 * 
 * @param enable True to enable the display
 */
void displayOn(bool enable);

/**
 * @brief Displays a null-terminated string starting from the current cursor position.
 * 
 * @param text Pointer to the null-terminated string to display.
 */
void showText(const char* text);

/**
 * @brief Writes a single character at the current cursor position.
 * 
 * @param c The character to display.
 */
void writeChar(char c);

/**
 * @brief Initializes the LCD hardware and resets the controller.
 */
void LCDinit();

/**
 * @brief Sends the initial command sequence for CiZ-based LCDs.
 */
void CiZ_init();

/**
 * @brief Clears the entire display and returns cursor to (0,0).
 */
void cleardisplay();

/**
 * @brief Moves the cursor to the beginning of the second line.
 */
void nextline();

/**
 * @brief Scrolls the entire display one character to the left.
 */
void scrollLeft();

/**
 * @brief Scrolls the entire display one character to the right.
 */
void scrollRight();

/**
 * @brief Enables the visible cursor without blinking.
 */
void cursorOn();

/**
 * @brief Hides the cursor from the display.
 */
void cursorOff();

/**
 * @brief Enables or disables blinking of the cursor.
 * 
 * @param enable True to enable blinking, false to disable.
 */
void blinkCursor(bool enable);

/**
 * @brief Sets the cursor to a specific column and row.
 * 
 * @param col Column index (0–19)
 * @param row Row index (0 or 1)
 */
void setCursor(uint8_t col, uint8_t row);

/**
 * @brief Returns the cursor to the home position (0,0).
 */
void home();

/**
 * @class DisplayWriter
 * @brief Provides a chainable interface for text positioning and alignment on a 2x20 LCD.
 * 
 * Use this class for convenient positioning and formatting of text on the LCD display.
 * 
 * Example:
 * @code
 * show("Hello").center().show();
 * show("World").at(0,1).right().show();
 * @endcode
 */
class DisplayWriter {
public:
    /**
     * @brief Constructs a DisplayWriter for a specific text.
     * 
     * @param t Pointer to the null-terminated text string to be displayed.
     */
    DisplayWriter(const char* t);

    /**
     * @brief Sets the cursor to a specific column and row before displaying.
     * 
     * @param col Column index (0–19)
     * @param row Row index (0 or 1)
     * @return Reference to this DisplayWriter for chaining.
     */
    DisplayWriter& at(uint8_t col, uint8_t row);

    /**
     * @brief Centers the text horizontally on the current row.
     * 
     * @return Reference to this DisplayWriter for chaining.
     */
    DisplayWriter& center();

    /**
     * @brief Right-aligns the text on the current row.
     * 
     * @return Reference to this DisplayWriter for chaining.
     */
    DisplayWriter& right();

    /**
     * @brief Left-aligns the text (default).
     * 
     * @return Reference to this DisplayWriter for chaining.
     */
    DisplayWriter& left();

    /**
     * @brief Sends the text to the display at the configured position.
     */
    void show() const;

private:
    const char* text; ///< Pointer to the text to display
};

/**
 * @brief Creates a DisplayWriter instance to display a given string.
 * 
 * @param text Null-terminated text string to be displayed.
 * @return A DisplayWriter instance for method chaining.
 */
DisplayWriter show(const char* text);

#endif // LCD_H
