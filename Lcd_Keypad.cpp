#include "Lcd_Keypad.h"

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
bool displayUserSystemlocked() {
    greenOn();
    lcd.cls(); // Clear the LCD
    lcd.locate(0, 0);
    lcd.printf("Activate Alarm?");
    lcd.locate(0, 1);
    lcd.printf("0 to lock or 1 to unlock?");
    char retval = getKeypadInput();
    while (retval != '0' && retval != '1') {
        // Keep requesting input until a valid key is pressed
        ThisThread::sleep_for(chrono::milliseconds(100)); // Avoid CPU overload
        retval = getKeypadInput();
    }

    return retval == '0' ? true : false; // Return true if '0', false if '1'

}

void displayAttemptsLeft(int& attempts) {
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


void resetSystem(int &attempts, int &masterAttempts, int &masterCodeMode, std::string &enteredCode) {
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

void checkCode( string &code, const string &correctCode, const string &masterCode, bool &masterCodeMode, int &masterAttempts, int &attempts) {
    greenOn();
    lcd.cls(); // Clear the LCD
    lcd.locate(0, 0);
    lcd.printf("Verifying...");
    ThisThread::sleep_for(2s); // Simulate the time taken to verify the code

    if (!masterCodeMode && code == correctCode) {
        // Correct access code entered
        greenOn();
        lcd.cls(); // Clear the LCD
        lcd.locate(0, 0);
        lcd.printf("Access Granted");
        systemLocked= false;
        ThisThread::sleep_for(2s);
        // Further actions to unlock or allow access can be added here
    } else if (masterCodeMode && code == masterCode) {
        // Correct master code entered
        redOn();
        lcd.cls(); // Clear the LCD
        lcd.locate(0, 0);
        lcd.printf("Master Access Granted");
        systemLocked = false;
        ThisThread::sleep_for(2s);
        // Further actions for master access can be added here
    } else {
        if (masterCodeMode) {
            // Incorrect master code
            --masterAttempts;
            if (masterAttempts <= 0) {
                // Failed master code entry
                displayFinalMessage(); // Lock the system and display final message
            } else {
                displayIncorrectMasterCode(); // Show the incorrect master code message
            }
        } else {
            // Incorrect access code
            --attempts;
            if (attempts <= 0) {
                masterCodeMode = true; // Switch to master code mode
                masterAttempts = 3; // Reset master code attempts
                displayMasterCodeEntry(); // Prompt for master code
            } else {
                displayAttemptsLeft(attempts); // Show the remaining attempts for the access code
            }
        }
    }
    code = ""; // Reset the entered code for the next attempt
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