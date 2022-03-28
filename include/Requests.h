#ifndef REQUEST_H
#define REQUEST_H

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

// Use to make user acknowledge that no contacts exist
// '\0' will always return
char requestConfirmNoContactsExist();

char requestConfirmNoMorePhoneNumbers();
char requestConfirmNoMoreEmails();

// Use to request the user exits the Search Contact Results screen
// If 'q' is returned --> Return to main menu
// If 's' is returned --> Return to search menu
char requestExitSearchContactDisplay();

// Use to request the user chooses whether to select a contact to edit/delete
// or return to the Main Menu after searching for a contact to edit/delete.
// If 's' is returned --> Prompt for a serial number
// If 'q' is returned --> Return to main menu
char requestSelectOrQuitEditDeleteContactSearch();

// Ask the user if they want to confirm the deletion of the selected contact.
// If 'y' is returned --> Delete the contact
// If '\0' is returned --> Do not delete the contact.
// In either case, return to the Main Menu.
char requestConfirmContactDeletion();

// Use to request the user exists the List Contact results screen
// This function will only return 'q' --> return to main menu.
// UNUSED DUE TO PAGINATOR
char requestExitListContacts();

// Use to obtain user confirmation that the Save option was executed.
// This function will only return '\0' --> display main menu again.
char requestSaveConfirmation();

// Use to determine whether to save before exiting the program.
// If 'n' is returned --> Do not save
// If 'y' is returned --> Save
// Either way, exit the program.
char requestSaveOnExit();

// Be very careful using this
char readChar(char *inputPrompt, char *errorPrompt, char *validInputs, int numberValidInputs, int returnNullOnBadValue);

// Use to request the user select an option in the Main Menu.
// Returns a MenuFeatures enum (an integer between 0 and 6)
MenuFeatures requestMainMenuInput();

// Use to request the user select an option in the following menus:
    // Add Contact Menu
    // Edit Contact Menu - Selecting what to edit
// Returns a MenuOptions enum (a value between 0 and 3)
MenuOptions requestFourOptionMenuInput();

// Use to request the user select an option in the following menus:
    // Search Menu
    // Edit Contact Menu - Selecting what to search by.
    // Delete Contact Menu - Selecting what to search by.
// Returns a MenuOptions enum (a value between 0 and 4)
MenuOptions requestFiveOptionMenuInput();

// Requests that the user enter an index for a phone number
// Returns a value between 1 and 5
int requestPhoneNumberIndex();

// Requests that the user enter an index for an Email ID
// Returns a value between 1 and 5
int requestEmailIndex();

// Request that the user enter a serial number
// numberOfContacts - How many contacts currently exist?
// requestPurpose - In what sub-menu is the request being made?
// Returns a number between 1 and numberOfContacts
int requestSerialNumber(int numberOfContacts, Modes requestPurpose);
#endif