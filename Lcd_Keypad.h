
#ifndef _LCD_KEYPAD_H
#define _LCD_KEYPAD_H
#include "mbed.h"
#include "LCDi2c.h"
#include <iostream>
using namespace std;


extern  string correctCode; // Define the correct code
extern  string masterCode; // Define the master code
extern string enteredCode;
extern int attempts; // Number of attempts for the regular code
extern int masterAttempts; // Number of attempts for the master code
extern bool masterCodeMode; // Flag to track if the system is in master code mode
extern bool systemLocked; // Flag to indicate system is locked and needs reset

// Keypad setup
DigitalOut rowPins[4] = {PTD12, PTD13, PTE25, PTE24}; // Rows
DigitalIn colPins[4] = {PTC0, PTB11, PTB10, PTE7}; // Columns

InterruptIn resetButton(PTD1); // Initialize the push button on pin PTC16
DigitalOut redLED(PTE8);

DigitalOut greenLED(PTE7);

// LCD setup
LCDi2c lcd(LCD16x2, 0x27); // LCD type and I2C address


void greenOn();

void redOn();

void initKeypad();

void displayWelcomeMessage();

void displayAttemptsLeft();

void displayMasterCodeEntry();

void displayIncorrectMasterCode();

void displayFinalMessage();

void resetSystem(int attempts, int masterAttempts, int masterCodeMode, std::string enteredCode);

void isr_resetSystem();

void setupResetButton();

void checkCode( string &code, const string &correctCode, const string &masterCode, bool &masterCodeMode, int &masterAttempts, int &attempts);

char getKeypadInput();

void displayCharacter(char c);

bool displayUserSystemlocked();

#endif