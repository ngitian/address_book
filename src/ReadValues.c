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

// Must be between 1 and 32 characters (33rd character is for null-terminating string value)
// Only letters, spaces and periods
char *readNameStringOld(char *inputPrompt, char *errorPrompt)
{
    int i = 0;
    char* inputString = (char *)malloc((NAME_LEN + 1) * sizeof(char));
    char currentChar;

    askForInput:
    printf("%s", inputPrompt);

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
    inputString[i] = '\0';
    return inputString;

    badInput:
    printf("%s", errorPrompt);
    goto askForInput;
}

// Must be between 0 and 32 characters (33rd character is for null-terminating values)
// Only numbers allowed
char *readPhoneNumberOld(char *inputPrompt, char *errorPrompt)
{
    int i = 0;
    char* inputString = (char *)malloc((NUMBER_LEN + 1) * sizeof(char));
    char currentChar;

    askForInput:
    printf("%s", inputPrompt);

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
    inputString[i] = '\0';
    return inputString;

    badInput:
    printf("%s", errorPrompt);
    goto askForInput;

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

int main()
{
    while(1)
    {
        printf("%s\n", requestNameInput());
        printf("%s\n", requestPhoneNumberInput(1));
    }
    return 0;
}