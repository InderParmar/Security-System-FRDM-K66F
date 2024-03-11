#include "mbed.h"

// Define the pins for the rows and columns
DigitalOut rowPins[4] = {PTC12, PTC8, PTC7, PTC6}; // Rows
DigitalIn colPins[4] = {PTC5, PTC4, PTC3, PTC2}; // Columns

void init() {
    // Configure all row pins as output and set them high
    for (auto &rowPin : rowPins) {
        rowPin = 1;
    }
    // Configure all column pins as input with pull-up resistor
    for (auto &colPin : colPins) {
        colPin.mode(PullUp);
    }
}

int main() {
    init();
    
    while (true) {
        for (int row = 0; row < 4; ++row) {
            // Set the current row low
            rowPins[row] = 0;
            
            for (int col = 0; col < 4; ++col) {
                // If the column reads low, then the button is pressed
                if (colPins[col] == 0) {
                    // Button press detected
                    printf("Button pressed at row %d, col %d\n", row + 1, col + 1);
                    // Wait for button release and debouncing
                    while(colPins[col] == 0);
                    ThisThread::sleep_for(200ms);
                }
            }
            
            // Set the row back high
            rowPins[row] = 1;
        }
        // Short delay between scans
        ThisThread::sleep_for(20ms);
    }
}
