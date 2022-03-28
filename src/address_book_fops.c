#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

#include "../include/address_book_fops.h"
#include "../include/address_book.h"

Status load_file(AddressBook *address_book)
{
	int ret;

	/* 
	 * Check for file existance
	 */
	ret = access(DEFAULT_FILE, F_OK);

	// populate address book
	address_book->list = malloc(MAX_ENTRIES * sizeof(ContactInfo));

	if (address_book->list == NULL)
	{
		printf("System did not allocate enough memory");
		return e_fail;
	}

	for (int i = 0; i < MAX_ENTRIES; ++i) {
		address_book->list[i].si_no = -1;
		strcpy(address_book->list[i].name[0], "empty name");
		for (int j = 0; j < PHONE_NUMBER_COUNT; ++j) {
			strcpy(address_book->list[i].phone_numbers[j], "empty phone");
			strcpy(address_book->list[i].email_addresses[j], "empty email");
		}
	}

	if (ret == 0) // file exisits
	{
		/* 
		 * Do the neccessary step to open the file
		 * Do error handling
		 */

		address_book->fp = fopen(DEFAULT_FILE, "r");

		// todo csv file input validation 

		// read first line, number of entries
		int size;
		fscanf(address_book->fp, "%d\n", &size);
		address_book->count = size;
		
		// read, process and load content
		for (int row = 0; row < size; ++row) {
			Status ret = parseCSVRow(address_book, row);
			if (ret == e_fail) {
				return e_fail;
			}
		}
		
	}
	else // file doesn't exists
	{
		/* Create a file for adding entries later */
		address_book->fp = fopen(DEFAULT_FILE, "w");
		if (address_book->fp == NULL) {
			return e_fail;
		}
		address_book->count = 0;
		fprintf(address_book->fp, "0");
	}

	fclose(address_book->fp);

	return e_success;
}

Status save_file(AddressBook *address_book)
{
	/*
	 * Write contacts back to file.
	 * Re write the complete file currently
	 */ 
	address_book->fp = fopen(DEFAULT_FILE, "w");

	if (address_book->fp == NULL)
	{
		return e_fail;
	}

	/* 
	 * Add the logic to save the file
	 * Make sure to do error handling
	 */

	fprintf(address_book->fp, "%d", address_book->count);
	for (int i = 0; i<address_book->count; ++i) {
		ContactInfo *cI = &(address_book->list[i]);
		fprintf(address_book->fp, "\n%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,",
			cI->name[0],
			cI->phone_numbers[0], cI->phone_numbers[1], cI->phone_numbers[2], cI->phone_numbers[3], cI->phone_numbers[4],
			cI->email_addresses[0], cI->email_addresses[1], cI->email_addresses[2], cI->email_addresses[3], cI->email_addresses[4]
		);
	}


	free(address_book->list);
	fclose(address_book->fp);

	return e_success;
}

Status parseCSVRow(AddressBook *address_book, int row) {

	// declare variables
	char nameInput[NAME_LEN][NAME_COUNT];
	char phonesInput[PHONE_NUMBER_COUNT][NUMBER_LEN];
	char emailsInput[EMAIL_ID_COUNT][EMAIL_ID_LEN];

	// parsing
	char tmp[NAME_LEN] = "";
	char c;

	int fieldCounter = 0;

	while ((c=fgetc(address_book->fp)) != EOF) {
		if (c == '\n') {
			break;
		}
		else if (c == FIELD_DELIMITER) {
			switch (fieldCounter) {
				case 0:
					strcpy(nameInput[0], tmp);
					break;
				case 1:
					strcpy(phonesInput[0], tmp);
					break;
				case 2:
					strcpy(phonesInput[1], tmp);
					break;
				case 3:
					strcpy(phonesInput[2], tmp);
					break;
				case 4:
					strcpy(phonesInput[3], tmp);
					break;
				case 5:
					strcpy(phonesInput[4], tmp);
					break;
				case 6:
					strcpy(emailsInput[0], tmp);
					break;
				case 7:
					strcpy(emailsInput[1], tmp);
					break;
				case 8:
					strcpy(emailsInput[2], tmp);
					break;
				case 9:
					strcpy(emailsInput[3], tmp);
					break;
				case 10:
					strcpy(emailsInput[4], tmp);
					break;

			}
			++fieldCounter;
			tmp[0] = '\0';
		} else {
			strncat(tmp, &c, 1);
		}
	}
	if (fieldCounter != NAME_COUNT + PHONE_NUMBER_COUNT + EMAIL_ID_COUNT) {
		printf("Invalid CSV format\n");
		return e_fail;
	}

	// store input in address_book's list of ContactInfo
	address_book->list[row].si_no = row + 1;

	strcpy(address_book->list[row].name[0], nameInput[0]);
	strcpy(address_book->list[row].phone_numbers[0], phonesInput[0]);
	strcpy(address_book->list[row].phone_numbers[1], phonesInput[1]);
	strcpy(address_book->list[row].phone_numbers[2], phonesInput[2]);
	strcpy(address_book->list[row].phone_numbers[3], phonesInput[3]);
	strcpy(address_book->list[row].phone_numbers[4], phonesInput[4]);
	strcpy(address_book->list[row].email_addresses[0], emailsInput[0]);
	strcpy(address_book->list[row].email_addresses[1], emailsInput[1]);
	strcpy(address_book->list[row].email_addresses[2], emailsInput[2]);
	strcpy(address_book->list[row].email_addresses[3], emailsInput[3]);
	strcpy(address_book->list[row].email_addresses[4], emailsInput[4]);

	return e_success;
}