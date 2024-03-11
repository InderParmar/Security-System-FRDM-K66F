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
