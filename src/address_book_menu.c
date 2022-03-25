#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// #include "abk_fileops.h"
// #include "abk_log.h"
// #include "abk_menus.h"
// #include "abk.h"
//#include "../include/address_book_menu.h"
//#include "../include/address_book_fops.h"
//#include "../include/address_book.h"

#include "address_book_fops.h"
#include "address_book_menu.h"
#include "address_book.h"


int get_option(int type, const char *msg)
{
	/*
	 * Mutilfuction user intractions like
	 * Just an enter key detection
	 * Read an number
	 * Read a charcter
	 */ 

	/* Fill the code to add above functionality */
	
	int result;
	printf("%s", msg);
	switch (type)
	{
		case NONE:
			scanf(" ");
			break;
		case NUM:
			scanf("%d", &result);
			fflush(stdin);
			break;
		case CHAR:
			scanf(" %lc", &result);
			break;
	}
	return result;
}

Status save_prompt(AddressBook *address_book)
{
	char option;

	do
	{
		main_menu();

		option = get_option(CHAR, "\rEnter 'N' to Ignore and 'Y' to Save: ");

		if (option == 'Y')
		{
			save_file(address_book);
			printf("Exiting. Data saved in %s\n", DEFAULT_FILE);

			break;
		}
	} while (option != 'N');

	free(address_book->list);

	return e_success;
}

Status list_contacts(AddressBook *address_book, const char *title, int *index, const char *msg, Modes mode)
{
	/* 
	 * Add code to list all the contacts availabe in address_book.csv file
	 * Should be menu based
	 * The menu provide navigation option if the entries increase the page size
	 */
	
	menu_header(title);

	// standard list contacts header format, 110 chars long
	printf("==============================================================================================================\n");
	printf(": S.No : Name                            : Phone No                        : Email ID                        :\n");
	printf("==============================================================================================================\n");

	// collect contact info from address_book->list[index]
	ContactInfo *cI = &(address_book->list[*index]);

	switch (mode) {
		case e_search:
			break;
		case e_edit:
			break;
		case e_delete:
			break;
		case e_list:
			print_format_list(cI);

			// user input, (n)ext, (p)revious, (q)uit
			char option;
			option = get_option(CHAR, msg);
			int newIndex;
			char newMsg[100];
			switch (option)
			{
				case 'n':
					newIndex = *index + 1;
					if (newIndex == address_book->count - 1) {
						strcpy(newMsg, "Press: [p]=previous, [q] | Cancel: ");
					} else {
						strcpy(newMsg, "Press: [n]=next, [p]=previous, [q] | Cancel: ");
					}
					list_contacts(address_book, "Search Result:\n", &newIndex, newMsg, e_list);
					break;
				case 'p':
					newIndex = *index - 1;
					if (newIndex == 0) {
						strcpy(newMsg, "Press: [n]=next, [q] | Cancel: ");
					} else {
						strcpy(newMsg, "Press: [n]=next, [p]=previous, [q] | Cancel: ");
					}
					list_contacts(address_book, "Search Result:\n", &newIndex, newMsg, e_list); //todo fix message

					break;
				case 'q':
					break;
				default:
					// todo catch invalid input other than n, p, q
					break;

			}

			break;
	}

	return e_success;
}

void menu_header(const char *str)
{
	fflush(stdout);

	// system("clear");

	printf("#######  Address Book  #######\n");
	if (str[0] != '\0')
	{
		printf("#######  %s\n", str);
	}
}

void main_menu(void)
{
	menu_header("Features:\n");

	printf("0. Exit\n");
	printf("1. Add Contact\n");
	printf("2. Search Contact\n");
	printf("3. Edit Contact\n");
	printf("4. Delete Contact\n");
	printf("5. List Contacts\n");
	printf("6. Save\n");
	printf("\n");
	printf("Please select an option: ");
}

Status menu(AddressBook *address_book)
{
	ContactInfo backup;
	Status ret;
	int option;

	do
	{
		main_menu();

		option = get_option(NUM, "");

		if ((address_book-> count == 0) && (option != e_add_contact))
		{
			get_option(NONE, "No entries found!!. Would you like to add? Use Add Contacts");

			continue;
		}

		switch (option)
		{
			case e_add_contact:
				/* Add your implementation to call add_contacts function here */
				add_contacts(address_book);
				break;
			case e_search_contact:
				search_contact(address_book);
				break;
			case e_edit_contact:
				edit_contact(address_book);
				break;
			case e_delete_contact:
				delete_contact(address_book);
				break;
			case e_list_contacts:
				; // this is necessary to be here to be after a label
				int index = 0;
				char msg[100];
				if (address_book->count > 1)
					strcpy(msg, "Press: [n] = next, [q] | Cancel: ");
				else
					strcpy(msg, "Press: [q] | Cancel: ");
				list_contacts(address_book, "Search Result:\n", &index, msg, e_list);
				break;
				/* Add your implementation to call list_contacts function here */
			case e_save:
				save_file(address_book);
				// load_file(address_book); // todo reload it right after saving to continue working on address book?
				break;
			case e_exit:
				break;
		}
	} while (option != e_exit);

	return e_success;
}

Status add_contacts(AddressBook *address_book)
{
	/* Add the functionality for adding contacts here */

	char nameInput[NAME_COUNT][NAME_LEN];
	char phonesInput[PHONE_NUMBER_COUNT][NUMBER_LEN];
	char emailsInput[EMAIL_ID_COUNT][EMAIL_ID_LEN];

	int phonesInputCounter = 0;
	int emailsInputCounter = 0;

	int nameFlag = 0;
	int phoneFlag = 0;
	int emailFlag = 0;
	int running = 1;

	while (running) {
		menu_header("Add Contact:\n");

		// print menu
		printf("0. Back\n");
		printf("1. Name       : ");
		if (nameFlag) {
			printf("%s", nameInput[0]);
		}
		printf("\n2. Phone No %d : ", phonesInputCounter + 1);
		if (phoneFlag) {
			for (int i = 0; i < phonesInputCounter; ++i) {
				printf("%s ", phonesInput[i]);
			}
		}
		printf("\n3. Email ID %d : ", emailsInputCounter + 1);
		if (emailFlag) {
			for (int i = 0; i < emailsInputCounter; ++i) {
				printf("%s ", emailsInput[i]);
			}
		}
		printf("\n");


		// menu input
		int option;
		option = get_option(NUM, "\nPlease select an option: ");

		switch (option)
		{
			case e_first_opt: // exit
				// save information if there's a name at least
				if (nameFlag) {
					address_book->count += 1;
					int index = address_book->count - 1;
					address_book->list[index].si_no = index + 1;
					strcpy(address_book->list[index].name[0], nameInput[0]);
					for (int i = 0; i < PHONE_NUMBER_COUNT; ++i) {
						if (i < phonesInputCounter) {
							strcpy(address_book->list[index].phone_numbers[i], phonesInput[i]);
						} else {
							strcpy(address_book->list[index].phone_numbers[i], "");
						}
						if (i < emailsInputCounter) {
							strcpy(address_book->list[index].email_addresses[i], emailsInput[i]);
						} else {
							strcpy(address_book->list[index].email_addresses[i], "");
						}
					}
				}
				
				running = 0;
				break;
			case e_second_opt: // add name, only 1 name
				printf("Enter the name: ");
				scanf("%s", nameInput[0]);

				// todo input validation
				nameFlag = 1;
				break;
			case e_third_opt: // add phone number, only 5
				printf("Enter Phone Number %d: [Please reenter the same option for additional Phone Number]: ", phonesInputCounter + 1);
				scanf("%s", phonesInput[phonesInputCounter]);

				// todo input validation
				phoneFlag = 1;
				++phonesInputCounter;
				break;
			case e_fourth_opt: // add email, only 5
				printf("Enter Email ID %d: [Please reenter the same option for additional Email ID]: ", emailsInputCounter + 1);
				scanf("%s", emailsInput[emailsInputCounter]);

				// todo input validation
				emailFlag = 1;
				++emailsInputCounter;
				break;
			default: // todo bad input
				break;
		}
	}

	// todo validation, no ','

	return e_success;
}

Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
{
	for (int i = 0; i < address_book->count; ++i) {
		
	}

	return e_success;
}

Status search_contact(AddressBook *address_book)
{
	/* Add the functionality for search contacts here */

	menu_header("Search Contact by:\n");

	// menu
	printf("0. Back\n");
	printf("1. Name\n");
	printf("2. Phone No\n");
	printf("3. Email ID\n");
	printf("4. Serial No\n");

	// read input
	int option;
	option = get_option(NUM, "Please select an option: ");

	char userInput[NAME_LEN];
	switch (option)
	{
		case e_first_opt: // back
			break;
		case e_second_opt: // name
			printf("Enter the Name: ");
			scanf("%s", userInput);
			break;
		case e_third_opt: // phone
			printf("Enter the Phone Number: ");
			scanf("%s", userInput);
			break;
		case e_fourth_opt: // email
			printf("Enter the Email ID: ");
			scanf("%s", userInput);
			break;
		case e_fifth_opt: // serial number
			break;
		default: // bad inputs
			break;
	}


	return e_success;
}

ContactInfo *getContactBySI_NO(AddressBook *address_book, int si_no){
	for(int contact = 0; contact < address_book->count, contact++){
		if(((address_book->list) + contact)->si_no == si_no){
			return ((address_book->list) + contact);
		}
	}
	return NULL;
}

Status edit_contact(AddressBook *address_book){
	/* Add the functionality for edit contacts here */

	int STR_LEN;
	char userInput[max(max(NAME_LEN, EMAIL_ID_LEN), NUMBER_LEN)]; //char array for user input.  NOTE: size set at NAME_LEN = 32.  This is ok because Phone No and Email max size is the same.
	char *msg = "Press: [s] = Select, Press: [q] | Cancel: ";
	int option;

	do{
		ContactInfo *contact;
		
		menu_header("Edit Contact by:\n");

		// menu
		printf("1. Name\n");
		printf("2. Phone No\n");
		printf("3. Email ID\n");
		printf("4. Serial Number\n");
		printf("0. Back\n");

		// read input
		
		option = get_option(NUM, "Please select an option: ");

		switch(option){
			case e_first_opt: // back
				break;
			case e_second_opt: // name
				printf("Enter the name you wish to edit: ");
				fgets(userInput, NAME_LEN, stdin);
				STR_LEN = strlen(userInput) - 1;

				if (userInput[STR_LEN] == '\n'){
					userInput[STR_LEN] = '\0'
				}
				break;
			case e_third_opt: // phone
				printf("Enter the phone number you wish to edit: ");
				fgets(userInput, NUMBER_LEN, stdin);
				STR_LEN = strlen(userInput) - 1;
				if (userInput[STR_LEN] == '\n'){
					userInput[STR_LEN] = '\0'
				}
				break;
			case e_fourth_opt: //email
				printf("Enter the email address you wish to edit: ");
				fgets(userInput, EMAIL_ID_LEN, stdin);
				STR_LEN = strlen(userInput) - 1;
				if (userInput[STR_LEN] == '\n'){
					userInput[STR_LEN] = '\0'
				}
				break;
			case e_fifth_opt: // serial number
				printf("Enter the serial number you wish to edit: ");
				fgets(userInput, 32, stdin);
				STR_LEN = strlen(userInput) - 1;
				if (userInput[STR_LEN] == '\n'){
					userInput[STR_LEN] = '\0'
				}
				break;
			default:
				option = e_no_opt;
				break;

		}

	}while(option != 0);

	return e_success;
}

Status delete_contact(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */

	return e_success;
}


void print_format_list(const ContactInfo* contactInfo)
{
	/**
	 * @brief print a contactInfo into the search result format
	 */
	printf(": %-5d: %-32s: %-32s: %-32s:\n",
		contactInfo->si_no, contactInfo->name[0], contactInfo->phone_numbers[0], contactInfo->email_addresses[0]
	);
	for (int j = 1; j < PHONE_NUMBER_COUNT; ++j) {
		printf(": %*c: %*c: %-32s: %-32s:\n", 5, ' ', 32, ' ', contactInfo->phone_numbers[j], contactInfo->email_addresses[j]);
	}
	printf("==============================================================================================================\n");
}