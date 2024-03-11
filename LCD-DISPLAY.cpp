////////////TO DISPLAY THE TEXT ON THE LCD////////////
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
