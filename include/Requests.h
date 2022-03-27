#include "address_book.h"

/*
WARNING - ALL CHAR ARRAYS RETURNED BY THE FOLLOWING THREE 
METHODS ARE DYNAMICALLY ALLOCATED. TO PREVENT MEMORY LEAKS, 
MAKE SURE YOU PROPERLY DEALLOCATE THEM AFTER YOU'RE DONE USING THEM.
*/

// Request and validate a user-inputted name.
char *requestNameInput();

// Request and validate a user-inputted phone number
// phoneNumberIndex - An integer between 1 and 5, inclusive
// requestPurpose - In what sub-menu is the request being made?
char *requestPhoneNumberInput(int phoneNumberIndex, Modes requestPurpose);

// Requests and validate a user-inputted email address
// emailAddressIndex - An integer betweeen 1 and 5, inclusive
// requestPurpose - In what sub-menu is the request being made?
char *requestEmailAddressInput(int emailAddressIndex, Modes requestPurpose);

/*
The values returned by the following functions are NOT
dynamically allocated.
*/

// Use to request the user input a number in the Main Menu.
// Returns a MenuFeatures enum (an integer between 0 and 6)
MenuFeatures requestMainMenuInput();

// Use to request the user input a number in the Add Contact Menu.
// Returns a MenuOptions enum (a value between 0 and 3)
MenuOptions requestAddContactMenuInput();