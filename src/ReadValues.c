#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/address_book.h"

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

    for(i = 0; i < NAME_LEN; i++)
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

    for(i = 0; i < NUMBER_LEN; i++)
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

    for(i = 0; i < EMAIL_ID_LEN; i++)
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
        "> Range from 1-32 characters\n"
        "> Consist only of letters, spaces and periods.\n"
        "Please try again.\n";

    return readString(inputPrompt, errorPrompt, NAME_LEN, readName);
}

char *requestPhoneNumberInput(int phoneNumberIndex)
{
    char inputPromptBuffer[30];
    sprintf(inputPromptBuffer, "%s%d%s", "Enter Phone Number ", phoneNumberIndex, ": ");

    char *inputPrompt = inputPromptBuffer;
    char *errorPrompt = "ERROR - Improper Input - Phone Numbers Must:\n"
        "> Range from 0-32 characters [Inputting nothing cancels your choice].\n"
        "> Consist only of numbers.\n"
        "Please try again.\n";
    
    return readString(inputPrompt, errorPrompt, NUMBER_LEN, readPhoneNumber);
}

char *requestEmailAddressInput(int emailAddressIndex)
{
    char inputPromptBuffer[30];
    sprintf(inputPromptBuffer, "%s%d%s", "Enter Email ID ", emailAddressIndex, ": ");

    char *inputPrompt = inputPromptBuffer;
    char *errorPrompt = "ERROR - Improper Input - Email Addresses Must:\n"
        "> Range from 0-32 characters [Inputting nothing cancels your choice].\n"
        "> Contain exactly one '@' between the email username and domain name.\n"
        "> Have at least one period in the domain name.\n"
        "> The usage of certain special characters may be restricted.\n"
        "Please try again.\n";

    readString(inputPrompt, errorPrompt, EMAIL_ID_LEN, readEmailAddress);
}

//int main()
//{
    //while(1)
    //{
        //char * name = requestNameInput();
        //printf("%s\n", name);
        //free(name);

        //char * number = requestPhoneNumberInput(1);
        //printf("%s\n", number);
        //free(number);

        //char * email = requestEmailAddressInput(1);
        //printf("%s\n", email);
        //free(email);
    //}
    //return 0;
//}