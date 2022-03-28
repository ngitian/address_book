#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/address_book.h"
#include "../include/Requests.h"

int flushBuffer()
{
    int currentChar, numExtraValues = 0;
    while ((currentChar = getchar()) != '\n' && currentChar != EOF)
        ++numExtraValues;
    return numExtraValues;
}

int readName(char *inputString)
{
    char currentChar;
    int i = 0;

    for(i = 0; i < NAME_LEN - 1; i++)
    {
        currentChar = getchar();
        switch(currentChar)
        {
            case EOF: case '\n':
                if(i == 0)
                    goto badInput;
                else
                    goto goodInput;
            
            case '.': case ' ':
                goto goodCharInput;

            default:
                if((currentChar >= 'A' && currentChar <= 'Z') || (currentChar >= 'a' && currentChar <= 'z'))
                    goto goodCharInput;
                else
                {
                    flushBuffer();
                    goto badInput;
                }
        }
        goodCharInput:
        inputString[i] = currentChar;
    }
    if(flushBuffer() > 0)
        goto badInput;
    
    goodInput:
    return i;
    badInput:
    return -1;
}

int readPhoneNumber(char *inputString)
{
    char currentChar;
    int i = 0;

    for(i = 0; i < NUMBER_LEN - 1; i++)
    {
        currentChar = getchar();
        switch(currentChar)
        {
            case EOF: case '\n':
                goto goodInput;
            
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                goto goodCharInput;

            default:
                flushBuffer();
                goto badInput;
        }
        goodCharInput:
        inputString[i] = currentChar;
    }
    if(flushBuffer() > 0)
        goto badInput;
    
    goodInput:
    return i;
    badInput:
    return -1;
}

int readEmailAddress(char *inputString)
{
    char currentChar;
    int i = 0, hasAtSymbol = 0, hasPeriodInDomain = 0;

    for(i = 0; i < EMAIL_ID_LEN - 1; i++)
    {
        currentChar = getchar();  
        switch(currentChar)
        {
            case EOF: case '\n':
                goto goodInput;
            
            case '@':
                if(hasAtSymbol == 0)
                {
                    hasAtSymbol = 1;
                    goto goodCharInput;
                }
                else
                {
                    flushBuffer();
                    goto badInput;
                }
            
            case '.':
                if(hasAtSymbol == 1)
                    hasPeriodInDomain = 1;
                goto goodCharInput;

            // Various special characters that are allowed unconditionally
            // to be in email addresses
            case '!': case '#': case '$': case '%': case '&':
            case '\'': case '*': case '+': case '-': case '/':
            case '=': case '?': case '^': case '_': case '`':
            case '{': case '|': case '}': case '~':

            default:
                // Alphanumerics
                if((currentChar >= 'A' && currentChar <= 'Z') || (currentChar >= 'a' && currentChar <= 'z')
                    || (currentChar >= '0' && currentChar <= '9'))
                    goto goodCharInput;
                else
                {
                    flushBuffer();
                    goto badInput;
                }
        }
        goodCharInput:
        inputString[i] = currentChar;
    }
    if(flushBuffer() > 0)
        goto badInput;

    goodInput:
    if((hasAtSymbol == 0 || hasPeriodInDomain == 0) && i > 0)
        goto badInput;
    return i;

    badInput:
    return -1;
}

char *readString(char *inputPrompt, char *errorPrompt, int maxInputLength, int (*functionPtr)(char *))
{
    int nullIndex = 0;
    char* inputString = (char *)malloc((maxInputLength + 1) * sizeof(char));

    askForInput:
    printf("%s", inputPrompt);

    nullIndex = (*functionPtr)(inputString);
    if(nullIndex > -1)
    {
        inputString[nullIndex] = '\0';
        return inputString;
    }
    else
    {
        printf("%s", errorPrompt);
        goto askForInput;
    }
}

char *requestNameInput()
{
    char *inputPrompt = "Enter the Name: ";
    char *errorPrompt = "ERROR - Improper Input - Names Must:\n"
        "> Range from 1-32 characters, including the null terminal\n"
        "> Consist only of letters, spaces and periods.\n"
        "Please try again.\n";

    return readString(inputPrompt, errorPrompt, NAME_LEN - 1, readName);
}

char *requestPhoneNumberInput(int phoneNumberIndex, Modes requestPurpose)
{
    char inputPromptBuffer[300];
    char *inputPrompt = inputPromptBuffer;
    char *errorPrompt = "ERROR - Improper Input - Phone Numbers Must:\n"
        "> Range from 0-32 characters, including the null terminal [Inputting nothing cancels your choice].\n"
        "> Consist only of numbers.\n"
        "Please try again.\n";

    switch(requestPurpose)
    {
        case e_add:
            sprintf(inputPromptBuffer, "%s%d%s", "Enter Phone Number ", phoneNumberIndex, 
                ": [You may enter up to 5 Phone Numbers by selecting the same option after this]: ");
            break;

        case e_edit:
            sprintf(inputPromptBuffer, "%s%d%s", "Enter Phone Number ", phoneNumberIndex,
                ": [Just enter removes the entry]: ");
            break;

        case e_search:
            sprintf(inputPromptBuffer, "%s", "Enter the Phone Number to search for: ");
            break;

        // UNDEFINED BEHAVIOR
        case e_list: case e_delete:
        return NULL;
    }

    return readString(inputPrompt, errorPrompt, NUMBER_LEN - 1, readPhoneNumber);
}

char *requestEmailAddressInput(int emailAddressIndex, Modes requestPurpose)
{
    char inputPromptBuffer[300];
    char *inputPrompt = inputPromptBuffer;
    char *errorPrompt = "ERROR - Improper Input - Email Addresses Must:\n"
        "> Range from 0-32 characters, including the null terminal [Inputting nothing cancels your choice].\n"
        "> Contain exactly one '@' between the email username and domain name.\n"
        "> Have at least one period in the domain name.\n"
        "> The usage of certain special characters may be restricted.\n"
        "Please try again.\n";

    switch(requestPurpose)
    {
        case e_add:
            sprintf(inputPromptBuffer, "%s%d%s", "Enter Email ID ", emailAddressIndex, 
                ": [You may enter up to 5 Email IDs by selecting the same option after this]: ");
            break;

        case e_edit:
            sprintf(inputPromptBuffer, "%s%d%s", "Enter Email ID ", emailAddressIndex,
                ": [Just enter removes the entry]: ");
            break;

        case e_search:
            sprintf(inputPromptBuffer, "%s", "Enter the Email ID to search for: ");
            break;

        // UNDEFINED BEHAVIOR
        case e_list: case e_delete:
        return NULL;
    }
    return readString(inputPrompt, errorPrompt, EMAIL_ID_LEN - 1, readEmailAddress);
}

char readChar(char *inputPrompt, char *errorPrompt, char *validInputs, int numberValidInputs, int returnNullOnBadValue)
{
    char inputChar;
    int i = 0;

    askForInput:
    printf("%s", inputPrompt);

    inputChar = getchar();
    if(inputChar == EOF || inputChar == '\n')
        goto singleCharInput;
    else
    {
        if(flushBuffer() > 0)
        {
            if(returnNullOnBadValue == 1)
                return '\0';
            else
                goto badInput;
        }
            
    }

    singleCharInput:
    for(i = 0; i < numberValidInputs; i++)
    {
        // Auto-convert to lowercase when applicable
        if(validInputs[i] == inputChar)
        {
            if(inputChar >= 'A' && inputChar <= 'Z')
                return inputChar + 32;
            else
                return inputChar;
        }      
    }
    if(returnNullOnBadValue == 1)
        return '\0';

    badInput:
    printf("%s", errorPrompt);
    goto askForInput;
}

char requestConfirmNoContactsExist()
{
    char *inputPrompt = "No entries found!. Would you like to add? Use Add Contacts. ";
    char *errorPrompt = "An error happened";

    return readChar(inputPrompt, errorPrompt, "", 0, 1);
}

char requestConfirmNoMorePhoneNumbers()
{    
    char *inputPrompt = "You cannot enter anymore phone numbers! ";
    char *errorPrompt = "An error happened";

    return readChar(inputPrompt, errorPrompt, "", 0, 1);
}

char requestConfirmNoMoreEmails()
{    
    char *inputPrompt = "You cannot enter anymore emails! ";
    char *errorPrompt = "An error happened";

    return readChar(inputPrompt, errorPrompt, "", 0, 1);
}

char requestExitSearchContactDisplay()
{
    char *inputPrompt = "Press: [s] = Return to Search Menu, [q] | Cancel: ";
    char *errorPrompt = "ERROR - Please enter a valid option\n";

    return readChar(inputPrompt, errorPrompt, "qQsS", 4, 0);
}

char requestSelectOrQuitEditDeleteContactSearch()
{
    char *inputPrompt = "Press: [s] = Select, [q] | Cancel: ";
    char *errorPrompt = "ERROR - Please enter a valid option\n";

    return readChar(inputPrompt, errorPrompt, "qQsS", 4, 0);
}

char requestConfirmContactDeletion()
{
    char *inputPrompt = "Press 'Y' to delete. [Press any key to ignore]: ";
    char *errorPrompt = "ERROR - Please enter exactly one character\n";

    return readChar(inputPrompt, "", "yY", 2, 1);
}

char requestExitListContacts()
{
    char *inputPrompt = "Press: [q] | Cancel: ";
    char *errorPrompt = "ERROR - Please enter a valid option\n";

    return readChar(inputPrompt, errorPrompt, "qQ", 2, 0);
}

char requestSaveConfirmation()
{
    char *inputPrompt = "Done. Press any key to continue: ";
    char *errorPrompt = "ERROR - Please enter exactly one character\n";

    return readChar(inputPrompt, errorPrompt, "", 0, 1);
}

char requestSaveOnExit()
{
    char *inputPrompt = "Enter 'N' to Ignore and 'Y' to Save: ";
    char *errorPrompt = "ERROR - Please enter a valid option\n";

    return readChar(inputPrompt, errorPrompt, "nNyY", 4, 0);
}

int readPositiveInteger(char *inputPrompt, char *errorPrompt, int minimumValue, int maximumValue)
{
    const int MAX_NUMBER_DIGITS = 9;
    char number[MAX_NUMBER_DIGITS];
    char currentChar;
    int i = 0, j = 0, value = 0;

    askForInput:
    printf("%s", inputPrompt);

    for(i = 0, value = 0; i < MAX_NUMBER_DIGITS; i++)
    {
        currentChar = getchar();
        switch(currentChar)
        {
            case EOF: case '\n':
                if(i == 0)
                    goto badInput;
                else
                    goto goodInput;
            
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                goto goodCharInput;
            
            default:
                flushBuffer();
                goto badInput;
        }
        goodCharInput:
        number[i] = currentChar;
    }
    if(flushBuffer() > 0)
        goto badInput;

    goodInput:
    for(j = 0; j < i; j++)
        value += pow(10, i - j - 1) * (number[j] - '0');

    if(value < minimumValue || value > maximumValue)
        goto badInput;
    else
        return value;

    badInput:
    printf("%s", errorPrompt);
    goto askForInput;
}

MenuFeatures requestMainMenuInput()
{
    char *inputPrompt = "Please select an option: ";
    char *errorPrompt = "ERROR - Please enter a value between 0 and 6\n";

    return readPositiveInteger(inputPrompt, errorPrompt, 0, 6);
}

MenuOptions requestFourOptionMenuInput()
{
    char *inputPrompt = "Please select an option: ";
    char *errorPrompt = "ERROR - Please enter a value between 0 and 3\n";

    return readPositiveInteger(inputPrompt, errorPrompt, 0, 3);
}

MenuOptions requestFiveOptionMenuInput()
{
    char *inputPrompt = "Please select an option: ";
    char *errorPrompt = "ERROR - Please select a value between 0 and 4\n";

    return readPositiveInteger(inputPrompt, errorPrompt, 0, 4);
}

int requestPhoneNumberIndex()
{
    char *inputPrompt = "Enter Phone Number index to be changed [Max 5]: ";
    char *errorPrompt = "ERROR - Please enter a value between 1 and 5\n";

    return readPositiveInteger(inputPrompt, errorPrompt, 1, 5);
}

int requestEmailIndex()
{
    char *inputPrompt = "Enter Email ID index to be changed [Max 5]: ";
    char *errorPrompt = "ERROR - Please enter a value between 1 and 5\n";

    return readPositiveInteger(inputPrompt, errorPrompt, 1, 5);
}

int requestSerialNumber(int numberOfContacts, Modes requestPurpose)
{
    char inputPromptBuffer[300];
    char errorPromptBuffer[300];
    sprintf(errorPromptBuffer, "%s%d%s", 
        "ERROR - Please select a value between 1 and ", numberOfContacts, "\n");

    char *inputPrompt = inputPromptBuffer;
    char *errorPrompt = errorPromptBuffer;

    switch(requestPurpose)
    {
        case e_edit:
            sprintf(inputPromptBuffer, "%s", "Select a Serial Number (S.No) to Edit: ");
        
        case e_delete:
            sprintf(inputPromptBuffer, "%s", "Select a Serial Number (S.No) to Delete: ");

        case e_search:
            sprintf(inputPromptBuffer, "%s", "Enter a Serial Number (S.No) to search for: ");

        // Undefined behavior
        case e_add: case e_list:
            return -1;
    }
    return readPositiveInteger(inputPrompt, errorPrompt, 1, numberOfContacts);
}
