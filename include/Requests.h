// WARNING - ALL CHAR ARRAYS RETURNED BY THESE METHODS ARE
// DYNAMICALLY ALLOCATED. TO PREVENT MEMORY LEAKS, MAKE SURE
// YOU PROPERLY DEALLOCATE THEM AFTER YOU'RE DOING USING THEM.

// Request and validate a user-inputted name.
char *requestNameInput();

// Request and validate a user-inputted phone number
// phoneNumberIndex - An integer between 1 and 5, inclusive
char *requestPhoneNumberInput(int phoneNumberIndex);

// Requests and validate a user-inputted email address
// emailAddressIndex - An integer betweeen 1 and 5, inclusive/
char *requestEmailAddressInput(int emailAddressIndex);