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
		strcpy("empty name", address_book->list[i].name[0]);
		for (int j = 0; j < PHONE_NUMBER_COUNT; ++j) {
			strcpy("empty phone", address_book->list[i].phone_numbers[j]);
			strcpy("empty email", address_book->list[i].email_addresses[j]);
		}
	}

	if (ret == 0) // file exisits
	{
		/* 
		 * Do the neccessary step to open the file
		 * Do error handling
		 */ 

		// read content

		// process and load content

		for (int i = 0; i < MAX_ENTRIES; ++i) {
			ContactInfo *ci = &(address_book->list[i]);
			printf("%d: %s[phone: ", ci->si_no, ci->name);

			for (int j = 0; j < PHONE_NUMBER_COUNT; ++j)
			{
				printf("%s,", ci->phone_numbers[j]);
			}

			printf("emails: ");
			for (int j = 0; j < EMAIL_ID_COUNT; ++j)
			{
				printf("%s,", ci->email_addresses[j]);
			}

			printf("]\n");
		}

	}
	else // file doesn't exists
	{
		/* Create a file for adding entries */
		address_book->fp = fopen(DEFAULT_FILE, "w");
		if (address_book->fp == NULL) {
			return e_fail;
		}
		address_book->count = 0;
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

	fclose(address_book->fp);
	free(address_book->list);

	return e_success;
}
