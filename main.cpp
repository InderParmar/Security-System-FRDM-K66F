#include "mbed.h"
#include "BufferedSerial.h"
#include "Lcd_Keypad.h"


DigitalIn irSensor(PTC16);
PwmOut speaker(PTD0);

static BufferedSerial serial_port(PTC4, PTC3, 9600);
bool systemLocked = false; // Flag to indicate system is locked and needs reset
bool keepRunning = true; // Control the alarm thread loop
Thread alarmThread; // Thread to handle the alarm system

string correctCode = "9995"; // Define the correct code
string masterCode = "88994455"; // Define the master code
string enteredCode = "";
int attempts = 3; // Number of attempts for the regular code
int masterAttempts = 2; // Number of attempts for the master code
bool masterCodeMode = false; // Flag to track if the system is in master code mode

void playTone(float frequency, float duration_ms) {
    speaker.period(1.0 / frequency); // Set the speaker's period
    speaker.write(0.5f); // Set duty cycle to 50%

    ThisThread::sleep_for(chrono::milliseconds(int(duration_ms))); // Duration of the tone

    speaker.write(0.0f); // Turn off the tone
}

void playAlarmSequence() {
    // Play a sequence of tones as an alarm
    playTone(1000.0, 500); // Tone 1 for 500 ms
    ThisThread::sleep_for(chrono::milliseconds(250)); // Pause for 250 ms
    playTone(800.0, 500); // Tone 2 for 500 ms
    ThisThread::sleep_for(chrono::milliseconds(250)); // Pause for 250 ms
    playTone(1200.0, 500); // Tone 3 for 500 ms
}

void alarmHandler() {
    while(keepRunning) {
        if (systemLocked && irSensor.read() == 0) {
            playAlarmSequence();
        }
        ThisThread::sleep_for(chrono::milliseconds(100));
    }
}

void manageAlarmThread(bool start) {
    if (start) {
        keepRunning = true; // Ensure thread loop will run
        if (!alarmThread.get_state()) {
            alarmThread.start(alarmHandler); // Start only if not already running
        }
    } else {
        keepRunning = false; // Signal thread to stop
        alarmThread.join(); // Wait for the thread to finish
    }
}

int main() {
    initKeypad();
    setupResetButton(); // Set up the reset button with its interrupt
    displayWelcomeMessage();
    speaker.period(1.0 / 1000.0); // Set initial speaker period
    speaker.write(0.0); // Start with speaker off
    alarmThread.start(alarmHandler); // Start the alarm thread

    while(true) {
        if (!systemLocked) {
            manageAlarmThread(false); // Stop the alarm thread
            systemLocked = displayUserSystemlocked(); // Ask user and update lock state
            manageAlarmThread(systemLocked); // Manage alarm thread based on new state
        } else {
            // In locked state, continue to check for user input to unlock the system
            char key = getKeypadInput();
            if (key != '\0') {
                if (key == '#') {
                    // If '#' is pressed, check the code
                   checkCode(enteredCode, correctCode, masterCode, masterCodeMode, masterAttempts, attempts);
                } else if (key == '*') {
                    // Clear the entered code if '*' is pressed
                    enteredCode.clear();
                } else {
                    // Add the digit to the entered code and display an asterisk
                    enteredCode += key;
                    displayCharacter('*');
                }
            }
        }
        ThisThread::sleep_for(chrono::milliseconds(100)); // Debounce delay
    }
}

