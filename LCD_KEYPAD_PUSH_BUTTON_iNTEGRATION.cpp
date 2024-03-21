#include "mbed.h"
#include "LCDi2c.h"
#include <iostream>
using namespace std;
// Keypad setup
DigitalOut rowPins[4] = {PTC12, PTC8, PTC7, PTC6}; // Rows
DigitalIn colPins[4] = {PTC5, PTC4, PTC3, PTC2}; // Columns

InterruptIn resetButton(PTD1); // Initialize the push button on pin PTC16
DigitalOut redLED(PTE8);
DigitalOut greenLED(PTE7);

// LCD setup
LCDi2c lcd(LCD16x2, 0x27); // LCD type and I2C address
const string correctCode = "9995"; // Define the correct code
const string masterCode = "88994455"; // Define the master code
string enteredCode = "";
int attempts = 3; // Number of attempts for the regular code
int masterAttempts = 2; // Number of attempts for the master code
bool masterCodeMode = false; // Flag to track if the system is in master code mode
bool systemLocked = false; // Flag to indicate system is locked and needs reset
void greenOn() {
    greenLED = 1; // Turn on green LED
    redLED = 0;   // Make sure red LED is off
}

void redOn() {
    redLED = 1;   // Turn on red LED
    greenLED = 0; // Make sure green LED is off
}



void initKeypad() {
    for (auto &rowPin : rowPins) {
        rowPin = 1;
    }
    for (auto &colPin : colPins) {
        colPin.mode(PullUp);
    }
}
void displayWelcomeMessage() {
    greenOn();
    lcd.cls(); // Clear the LCD
    lcd.locate(0, 0);
    lcd.printf("Enter your access");
    lcd.locate(0, 1);
    lcd.printf("code:");
}
void displayAttemptsLeft() {
    lcd.cls(); // Clear the LCD
    lcd.locate(0, 0);

    if (attempts > 1) {
        lcd.printf("Incorrect Code");
        ThisThread::sleep_for(2s); // Display the message for 2 seconds
        lcd.cls();
        lcd.printf("%d attempts left", attempts); // Show remaining attempts
    } else 
    if (attempts > 0) {
        lcd.printf("Incorrect Code");
        ThisThread::sleep_for(2s); // Display the message for 2 seconds
        lcd.cls();
        lcd.printf("%d attempt left", attempts); // Show remaining attempts
    } 
    else
    {
        lcd.cls();
        lcd.locate(0, 0);
        lcd.printf("No More Tries"); // Inform the user that no more attempts are left
        lcd.locate(0, 1);
        lcd.printf("Enter MasterCode"); // Prompt the user to enter the master code
    }
    ThisThread::sleep_for(2s); // Give some time to read the message before resetting
    displayWelcomeMessage(); // Go back to the welcome message
}
void displayMasterCodeEntry() {
    redOn();
    lcd.cls(); // Clear the LCD
    lcd.locate(0, 0);
    lcd.printf("Master Code:");
}

void displayIncorrectMasterCode() {
    redOn();
    lcd.cls(); // Clear the LCD
    lcd.locate(0, 0);
    lcd.printf("Incorrect Master");
    lcd.locate(0, 1);
    lcd.printf("Code");
    ThisThread::sleep_for(2s); // Give some time to read the message
    displayMasterCodeEntry(); // Prompt for master code again
}

void displayFinalMessage() {
redOn();
    systemLocked = true; // Set system locked flag
    lcd.cls(); // Clear the LCD
    lcd.locate(0, 0);
    lcd.printf("Fuck Off");
    lcd.locate(0, 1);
    lcd.printf("Thief!"); // Display your final message
}


void resetSystem() {
    greenOn();
    systemLocked = false; // Clear system locked flag
    attempts = 3; // Reset attempt counter
    masterAttempts = 2; // Reset master code attempt counter
    masterCodeMode = false; // Exit master code mode
    enteredCode.clear(); // Clear any entered code
    displayWelcomeMessage(); // Display the welcome message
}

void isr_resetSystem() {
    NVIC_SystemReset(); // Trigger a system reset
}


void setupResetButton() {
    resetButton.mode(PullUp); // Enable internal pull-up resistor
    resetButton.fall(&isr_resetSystem); // Attach ISR for the reset system
}
void checkCode(const string &code) {
    greenOn();
    lcd.cls(); // Clear the LCD
    lcd.locate(0, 0);
        lcd.printf("Verifying...");
    ThisThread::sleep_for(2s); // Simulate the time taken to verify the code
    if ((code == correctCode && !masterCodeMode) || (code == masterCode && masterCodeMode)) {
        lcd.cls(); // Clear the LCD
        lcd.locate(0, 0);
        if(masterCodeMode)
        {
            redOn();
            lcd.printf("Master OK");
            ThisThread::sleep_for(2s);

        }else {
        greenOn();     
        lcd.printf("Unlocked");
        ThisThread::sleep_for(2s);

        }
        resetSystem(); // Reset system to initial state
    } else {
        lcd.cls(); // Clear the LCD
        lcd.locate(0, 0);
        if (masterCodeMode) {
            --masterAttempts;
            if(masterAttempts <= 0) {
                displayFinalMessage(); // Display the final message and lock the system
            } else {
                displayIncorrectMasterCode(); // Show the incorrect master code message
            }
        } else {
            --attempts;
            if(attempts <= 0) {
                masterCodeMode = true; // Switch to master code mode
                displayMasterCodeEntry(); // Prompt for master code
            } else {
                displayAttemptsLeft(); // Show the remaining attempts for the regular code
            }
        }
    }
    enteredCode = ""; // Reset the entered code for the next attempt
}

char getKeypadInput() {
    char keys[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    for (int row = 0; row < 4; ++row) {
        rowPins[row] = 0;
        for (int col = 0; col < 4; ++col) {
            if (colPins[col] == 0) {
                rowPins[row] = 1;
                return keys[row][col];
            }
        }
        rowPins[row] = 1;
    }
    return '\0'; // Return null character if no key is pressed
}

void displayCharacter(char c) {
    if (c != '\0') {
        lcd.putc(c); // Display character on LCD
    }
}

int main() {
    initKeypad();
    setupResetButton(); // Set up the reset button with its interrupt
    displayWelcomeMessage();

    while (true) {
        // If the system is locked, wait for a reset and ignore other inputs
        if (systemLocked) continue;

        char key = getKeypadInput(); // Get keypad input
        if (key != '\0') {
            if (systemLocked) {
                // If the system is locked, ignore key presses and wait for reset
                continue;
            }

            if (key == '#') { // If '#' is pressed, check the code
                checkCode(enteredCode);
            } else {
                // In master code mode, check if the entered code length exceeds master code length
                if (masterCodeMode && enteredCode.length() >= masterCode.length()) {
                    continue; // Ignore further inputs if the master code length is exceeded
                }

                enteredCode += key; // Append the key to the entered code
                displayCharacter('*'); // Display an asterisk for each digit
            }
        }

        ThisThread::sleep_for(100ms); // Delay to allow for button release and debouncing
    }
}
