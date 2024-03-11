////////////TO DISPLAY THE TEXT ON THE LCD////////////
//library link : https://os.mbed.com/users/sstaub/code/mbedLCDi2c/

#include "mbed.h"
#include "LCDi2c.h"

// Assuming default I2C pins (D14 & D15 for the FRDM-K66F) will be used for SDA and SCL
LCDi2c lcd(LCD16x2, 0x27); // LCD type and I2C address

int main() {
    lcd.cls(); // Clear display
    lcd.locate(0, 0); // Set cursor location to row 0, column 0
    lcd.printf("START\n"); // Display text
    ThisThread::sleep_for(2s); // Wait for 2 seconds
    lcd.cls(); // Clear display
    lcd.locate(0, 0); // Set cursor location to row 0, column 0
    lcd.printf("Hello World!\n"); // Display text
    // The rest of your code...
}
////////////TO DISPLAY THE TEXT ON THE LCD////////////
#include "mbed.h"
#include "LCDi2c.h"

LCDi2c lcd(LCD16x2, 0x27); // Assuming 0x27 is the correct I2C address

int main() {
    lcd.cls(); // Clear display

    // Display your name and student number on the first row
    lcd.locate(0, 0); // Set cursor to the first column of the first row
    lcd.printf("Inder 164132219"); // Replace with your actual name and number
    
    // Display your lab partner's name and student number on the second row
    lcd.locate(0, 1); // Set cursor to the first column of the second row
    lcd.printf("Fahad 189890018"); // Replace with your partner's actual name and number

    // No need to clear the display or sleep in this simple static message scenario
}


//////////Scrolling     Effect///////=------------------
#include "mbed.h"
#include "LCDi2c.h"

// Initialize the LCD. Assuming default I2C pins and the correct I2C address.
LCDi2c lcd(LCD16x2, 0x27);

int main() {
    // The message to scroll
    const char* message = "SEP600 Embedded System is Awesome  ";
    const int messageLength = strlen(message); // Length of the message

    // Full message buffer that includes space for the display width
    const int displayWidth = 16;
    char displayBuffer[displayWidth + 1]; // +1 for the null-terminator

    int startIndex = 0; // Start index of the message to be displayed

    while (true) {
        // Copy a substring of the message into the display buffer
        for (int i = 0; i < displayWidth; ++i) {
            int messageIndex = (startIndex + i) % messageLength;
            displayBuffer[i] = message[messageIndex];
        }

        // Null-terminate the string
        displayBuffer[displayWidth] = '\0';

        // Show the substring on the display
        lcd.cls(); // Clear display
        lcd.locate(0, 0); // Set cursor to the start
        lcd.printf(displayBuffer); // Display the substring

        // Increase the start index for the next frame of the scroll
        startIndex++;
        if (startIndex >= messageLength) {
            startIndex = 0;
        }

        // Delay between scroll steps
        ThisThread::sleep_for(300ms);
    }
}



