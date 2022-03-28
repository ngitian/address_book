#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// #include "abk_fileops.h"
// #include "abk_log.h"
// #include "abk_menus.h"
// #include "abk.h"
#include "../include/address_book_menu.h"
#include "../include/address_book_fops.h"
#include "../include/address_book.h"
#include "../include/Requests.h"

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
}

Status menu(AddressBook *address_book)
{
	ContactInfo backup;
	Status ret;
	MenuOptions option;

	do
	{
		main_menu();

		option = requestMainMenuInput();

		// Todo - garbage first value?
		if ((address_book-> count == 0) && (option != e_add_contact) && (option != e_exit))
		{
			requestConfirmNoContactsExist();
			continue;
		}

		switch (option)
		{
			case e_add_contact:
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
				list_contact(address_book, 0);
				break;
			case e_save:
				save_file(address_book);
				load_file(address_book); // todo reload it right after saving to continue working on address book?
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
		printf("\n2. Phone No 1 : ");
		if (phoneFlag) {
			printf("%s", phonesInput[0]);
			for (int i = 1; i < phonesInputCounter; ++i) {
				if (strcmp(phonesInput[i], "") != 0) {
					printf("\n            %d : %s", i+1, phonesInput[i]);
				}
			}
		}
		printf("\n3. Email ID 1 : ");
		if (emailFlag) {
			printf("%s", emailsInput[0]);
			for (int i = 1; i < emailsInputCounter; ++i) {
				if (strcmp(emailsInput[i], "") != 0) {
					printf("\n            %d : %s", i+1, emailsInput[i]);
				}
			}
		}
		printf("\n\n");


		// menu input
		MenuOptions option = requestFourOptionMenuInput();
		char * dynamicString;

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
				//printf("Enter the name: ");
				//scanf("%s", nameInput[0]);
				dynamicString = requestNameInput();
				for(int i = 0; i < NAME_LEN; i++)
					nameInput[0][i] = dynamicString[i];
				free(dynamicString);

				nameFlag = 1;
				break;
			case e_third_opt: // add phone number, only 5
				if(phonesInputCounter == 5)
				{
					requestConfirmNoMorePhoneNumbers();
					break;
				}
				//printf("Enter Phone Number %d: [Please reenter the same option for additional Phone Number]: ", phonesInputCounter + 1);
				//scanf("%s", phonesInput[phonesInputCounter]);

				dynamicString = requestPhoneNumberInput(phonesInputCounter + 1, e_add);
				if(dynamicString[0] == '\0') // Empty
				{
					free(dynamicString);
					break;
				}
				for(int i = 0; i < NUMBER_LEN; i++)
				{
					phonesInput[phonesInputCounter][i] = dynamicString[i];
				}
				free(dynamicString);

				phonesInputCounter++;
				phoneFlag = 1;
				break;
			case e_fourth_opt: // add email, only 5
				if(emailsInputCounter == 5)
				{
					requestConfirmNoMoreEmails();
					break;
				}
				//printf("Enter Email ID %d: [Please reenter the same option for additional Email ID]: ", emailsInputCounter + 1);
				//scanf("%s", emailsInput[emailsInputCounter]);
				dynamicString = requestEmailAddressInput(emailsInputCounter + 1, e_add);
				if(dynamicString[0] == '\0')
				{
					free(dynamicString);
					break;
				}
				for(int i = 0; i < EMAIL_ID_LEN; i++)
				{
					emailsInput[emailsInputCounter][i] = dynamicString[i];
				}
				free(dynamicString);

				emailFlag = 1;
				++emailsInputCounter;
				break;
			default:
				break;
		}
	}
	return e_success;
}

int search(const char *str, AddressBook *address_book, int loop_count, Fields field, int serialNumber)
{
	/**
	 * @brief search address_book list of ContactInfo based on field
	 * @return e_no_match=-7 if not found, else return index of found ContactList
	 */
	int found = -1;
	int list_size = address_book->count;
	// search if item exists
	switch(field)
	{
		case e_name:
			for (int i = loop_count; i < list_size && found == -1; ++i) {
				if ( strcmp(address_book->list[i].name[0], str) == 0) {
					found = i;
				}
			}
			break;
		case e_phone: // phone
			for (int i = loop_count; i < list_size && found == -1; ++i) {
				for (int j = 0; j < PHONE_NUMBER_COUNT && found == -1; ++j) {
					if (strcmp(address_book->list[i].phone_numbers[j], str) == 0) {
						found = i;
					}
				}
			}
		case e_email: // email
			for (int i = loop_count; i < list_size && found == -1; ++i) {
				for (int j = 0; j < EMAIL_ID_COUNT && found == -1; ++j) {
					if (strcmp(address_book->list[i].email_addresses[j], str) == 0) {
						found = i;
					}
				}
			}
		case e_si_no: // serial number
			;
			//int si = strtol(str, NULL, 10);
			for (int i = loop_count; i < list_size && found == -1; ++i) {
				if (address_book->list[i].si_no == serialNumber) {
					found = i;
				}
			}
		default:
			break;
	}

	// if not found
	if (found == -1) {
		return e_no_match;
	}

	return found;
}

Status search_contact(AddressBook *address_book)
{
	SEARCH_START:

	menu_header("Search Contact by:\n");

	// // menu
	printf("0. Back\n");
	printf("1. Name\n");
	printf("2. Phone No\n");
	printf("3. Email ID\n");
	printf("4. Serial No\n\n");

	// // read input
	MenuOptions option = requestFiveOptionMenuInput();

	char * dynamicString;
	int serialNumberInput = 0;
	char userInput[NAME_LEN];
	Fields field;
	switch (option)
	{
		case e_first_opt: // back
			return e_success;
			break;
		case e_second_opt: // name
			//printf("Enter the Name: ");
			//scanf("%s", userInput);
			dynamicString = requestNameInput();
			for(int i = 0; i < NAME_LEN; i++)
				userInput[i] = dynamicString[i];
			free(dynamicString);
			field = e_name;
			break;
		case e_third_opt: // phone
			//printf("Enter the Phone Number: ");
			//scanf("%s", userInput);
			dynamicString = requestPhoneNumberInput(-1, e_search);
			for(int i = 0; i < NUMBER_LEN; i++)
				userInput[i] = dynamicString[i];
			free(dynamicString);
			field = e_phone;
			break;
		case e_fourth_opt: // email
			//printf("Enter the Email ID: ");
			//scanf("%s", userInput);
			dynamicString = requestEmailAddressInput(-1, e_search);
			for(int i = 0; i < EMAIL_ID_LEN; i++)
				userInput[i] = dynamicString[i];
			free(dynamicString);
			field = e_email;
			break;
		case e_fifth_opt: // serial number
			//printf("Enter the Serial Number: ");
			//scanf("%s", userInput);
			serialNumberInput = requestSerialNumber(address_book-> count, e_search);
			field = e_si_no;
			break;
		default: // bad inputs
			break;
	}

	print_header("Search Result:\n");

	// display all matching result
	int loop_count = 0;
	int found_flag = 0;
	while (loop_count < address_book->count) {
		int idx = search(userInput, address_book, loop_count, field, serialNumberInput);
		if (idx != e_no_match) {
			loop_count = idx + 1;
			found_flag = 1;
			ContactInfo *cI = &(address_book->list[idx]);
			print_contact(cI);
		} else if (found_flag == 0) {
			print_contact(NULL);
			break;
		} else {
			break;
		}
	}

	// user input to exit
	option = requestExitSearchContactDisplay();
	if(option == 's')
		goto SEARCH_START;
	
	return e_success;
}

Status edit_contact(AddressBook *address_book){
	/* Add the functionality for edit contacts here */
	menu_header("Search Contact to Edit by:\n");

	// menu
	printf("0. Back\n");
	printf("1. Name\n");
	printf("2. Phone No\n");
	printf("3. Email ID\n");
	printf("4. Serial No\n\n");

	// read input
	int option;
	option = get_option(NUM, "Please select an option: ");

	char userInput[NAME_LEN];
	int serialNumberInput = 0;
	Fields field;
	switch (option)
	{
		case e_first_opt: // back
			break;
		case e_second_opt: // name
			printf("Enter the Name: ");
			scanf("%s", userInput);
			field = e_name;
			break;
		case e_third_opt: // phone
			printf("Enter the Phone Number: ");
			scanf("%s", userInput);
			field = e_phone;
			break;
		case e_fourth_opt: // email
			printf("Enter the Email ID: ");
			scanf("%s", userInput);
			field = e_email;
			break;
		case e_fifth_opt: // serial number
			printf("Enter the Serial Number: ");
			scanf("%s", userInput);
			field = e_si_no;
			break;
		default: // bad inputs
			break;
	}

	print_header("Search Result:\n");
	// display all matching result
	int loop_count = 0;
	int found_flag = 0;
	while (loop_count < address_book->count) {
		int idx = search(userInput, address_book, loop_count, field, serialNumberInput);
		if (idx != e_no_match) {
			loop_count = idx + 1;
			found_flag = 1;
			ContactInfo *cI = &(address_book->list[idx]);
			print_contact(cI);
		} else if (found_flag == 0) {
			print_contact(NULL);
			break;
		} else {
			break;
		}
	}

	// select or quit
	option = get_option(CHAR, "Press: [s] = Select, [q] | Cancel: ");
	switch (option)
	{
		case 's':
			break;
		case 'q':
		default:
			return e_success;
			break;
	}

	// select serial number
	int si_no = get_option(NUM, "Select a Serial Number (S.No) to Edit: "); // todo validate serial number

	// edit person
	Status ret = edit_person(address_book, si_no);

	return ret;
}

Status edit_person(AddressBook *address_book, int si_no) {
	/**
	 * @brief display the menu to edit contact
	 */
	char temp[10];
	sprintf(temp, "%d", si_no);
	int idx = search(temp, address_book, 0, e_si_no, si_no);
	ContactInfo *cI = &(address_book->list[idx]);

	int running = 1;
	while (running) {
		menu_header("Edit Contact: \n");

		// print menu
		printf("0. Back\n");
		printf("1. Name      : %s\n", cI->name[0]);
		printf("2. Phone No 1: %s\n", cI->phone_numbers[0]);
		for (int i = 1; i < PHONE_NUMBER_COUNT; ++i) {
			if (strcmp(cI->phone_numbers[i], "") != 0)
				printf("            %d: %s\n", i + 1, cI->phone_numbers[i]);
		}
		printf("3. Email ID 1: %s\n", cI->email_addresses[0]);
		for (int i = 1; i < EMAIL_ID_COUNT; ++i) {
			if (strcmp(cI->email_addresses[i], "") != 0)
				printf("            %d: %s\n", i + 1, cI->email_addresses[i]);
		}
		printf("\n");

		// select option
		int option = get_option(NUM, "Please select an option: ");
		char userInput[NAME_LEN];
		switch (option)
		{
			case e_first_opt: // back
				running = 0;
				break;
			case e_second_opt: // name
				printf("Enter Name [Can NOT be empty]: ");
				scanf("%s", userInput);

				// todo name validation
				strcpy(cI->name[0], userInput);
				
				break;
			case e_third_opt: // phone
				printf("Enter Phone Number index to be changed [max 5]: ");
				scanf("%d", &idx);
				printf("Enter Phone Number %d: [Just enter removes the entry]: ", idx);
				scanf("%s", userInput);

				// phone and index validation
				strcpy(cI->phone_numbers[idx-1], userInput);
				break;
			case e_fourth_opt: // email
				printf("Enter Email ID index to be changed [max 5]: ");
				scanf("%d", &idx);
				printf("Enter Email Number %d: [Just enter removes the entry]: ", idx);
				scanf("%s", userInput);

				// email and index validation
				strcpy(cI->email_addresses[idx-1], userInput);
				break;
			default:
				break;
		}

	}

	return e_success;
}

Status delete_contact(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */
	menu_header("Search Contact to Delete by:\n");

	// menu
	printf("0. Back\n");
	printf("1. Name\n");
	printf("2. Phone No\n");
	printf("3. Email ID\n");
	printf("4. Serial No\n\n");

	// read input
	int option;
	option = get_option(NUM, "Please select an option: ");

	char userInput[NAME_LEN];
	int serialNumberInput = 0;
	Fields field;
	switch (option)
	{
		case e_first_opt: // back
			break;
		case e_second_opt: // name
			printf("Enter the Name: ");
			scanf("%s", userInput);
			field = e_name;
			break;
		case e_third_opt: // phone
			printf("Enter the Phone Number: ");
			scanf("%s", userInput);
			field = e_phone;
			break;
		case e_fourth_opt: // email
			printf("Enter the Email ID: ");
			scanf("%s", userInput);
			field = e_email;
			break;
		case e_fifth_opt: // serial number
			printf("Enter the Serial Number: ");
			scanf("%s", userInput);
			field = e_si_no;
			break;
		default: // bad inputs
			break;
	}

	print_header("Search Result:\n");
	// display all matching result
	int loop_count = 0;
	int found_flag = 0;
	while (loop_count < address_book->count) {
		int idx = search(userInput, address_book, loop_count, field, serialNumberInput);
		if (idx != e_no_match) {
			loop_count = idx + 1;
			found_flag = 1;
			ContactInfo *cI = &(address_book->list[idx]);
			print_contact(cI);
		} else if (found_flag == 0) {
			print_contact(NULL);
			break;
		} else {
			break;
		}
	}

	// select or quit
	option = get_option(CHAR, "Press: [s] = Select, [q] | Cancel: ");
	switch (option)
	{
		case 's':
			break;
		case 'q':
		default:
			return e_success;
			break;
	}

	// select serial number
	int si_no = get_option(NUM, "Select a Serial Number (S.No) to Delete: "); // todo validate serial number

	// delete person
	Status ret = delete_person(address_book, si_no);

	return ret;
}

Status delete_person(AddressBook *address_book, int si_no)
{
	/**
	 * @brief display the menu to edit delete
	 */
	char temp[10];
	sprintf(temp, "%d", si_no);
	int idx = search(temp, address_book, 0, e_si_no, si_no);
	ContactInfo *cI = &(address_book->list[idx]);

	menu_header("Edit Contact: \n");

	// print menu
	printf("0. Back\n");
	printf("1. Name      : %s\n", cI->name[0]);
	printf("2. Phone No 1: %s\n", cI->phone_numbers[0]);
	for (int i = 1; i < PHONE_NUMBER_COUNT; ++i) {
		if (strcmp(cI->phone_numbers[i], "") != 0)
			printf("            %d: %s\n", i + 1, cI->phone_numbers[i]);
	}
	printf("3. Email ID 1: %s\n", cI->email_addresses[0]);
	for (int i = 1; i < EMAIL_ID_COUNT; ++i) {
		if (strcmp(cI->email_addresses[i], "") != 0)
			printf("            %d: %s\n", i + 1, cI->email_addresses[i]);
	}
	printf("\n");

	// select option
	int option = get_option(CHAR, "Enter 'Y' to delete. [Press any key to ignore]: ");
	
	// delete person from contact, move other people up
	if (option == 'Y')
	{
		address_book->count -= 1;
		if (idx == address_book->count) // delete last item
		{
			cI->si_no = -1;
			strcpy(cI->name[0], "empty name");
			for (int i = 0; i < PHONE_NUMBER_COUNT; ++i) {
				strcpy(cI->phone_numbers[i], "empty phone");
				strcpy(cI->email_addresses[i], "empty email");
			}

		} else { // replace the deleted index with last
			ContactInfo *last = &(address_book->list[address_book->count]);
			// replace idx with contactinfo of last item
			strcpy(cI->name[0], last->name[0]);
			for (int i = 0; i < PHONE_NUMBER_COUNT; ++i) {
				strcpy(cI->phone_numbers[i], last->phone_numbers[i]);
				strcpy(cI->email_addresses[i], last->phone_numbers[i]);
			}
			
			// delete last item info
			last->si_no = -1;
			strcpy(last->name[0], "empty name");
			for (int i = 0; i < PHONE_NUMBER_COUNT; ++i) {
				strcpy(last->phone_numbers[i], "empty phone");
				strcpy(last->email_addresses[i], "empty email");
			}

		}
	}

	return e_success;
}

Status list_contact(AddressBook *address_book, int idx)
{
	/**
	 * @brief list contacts using paginator
	 */
	print_header("Search result:\n");

	char option;
	int newIndex;
	char newMsg[100];

	// if list empty
	if (address_book->count == 0)
	{
		print_contact(NULL);
		option = get_option(CHAR, "Press: [q] | Cancel: ");
		return e_success;
	} else { // list first ContactList
		print_contact(&(address_book->list[idx]));
		if (address_book->count == 1) {
			strcpy(newMsg, "Press: [q] | Cancel: ");
		} else if (idx > 0 && idx < address_book->count - 1) {
			strcpy(newMsg, "Press: [n] = Next, [p] = Previous, [q] | Cancel: ");
		} else if (idx == address_book->count - 1) {
			strcpy(newMsg, "Press: [p] = Previous, [q] | Cancel: ");
		} else {
			strcpy(newMsg, "Press: [n] = Next, [q] | Cancel: ");
		}
	}

	// user input, (n)ext, (p)revious, (q)uit
	option = get_option(CHAR, newMsg);
	switch (option)
	{
		case 'n':
			newIndex = idx + 1;
			if (newIndex == address_book->count - 1) {
				strcpy(newMsg, "Press: [p]=previous, [q] | Cancel: ");
			} else {
				strcpy(newMsg, "Press: [n]=next, [p]=previous, [q] | Cancel: ");
			}
			list_contact(address_book, newIndex);
			break;
		case 'p':
			newIndex = idx - 1;
			if (newIndex == 0) {
				strcpy(newMsg, "Press: [n]=next, [q] | Cancel: ");
			} else {
				strcpy(newMsg, "Press: [n]=next, [p]=previous, [q] | Cancel: ");
			}
			list_contact(address_book, newIndex);

			break;
		case 'q':
			break;
		default:
			// todo catch invalid input other than n, p, q
			break;

	}
}

void print_header(char *title)
{
	/**
	 * @brief print the header of the search result for search, edit, delete and list
	 */
	menu_header(title);

	// standard list contacts header format, 110 chars long
	printf("==============================================================================================================\n");
	printf(": S.No : Name                            : Phone No                        : Email ID                        :\n");
	printf("==============================================================================================================\n");

}

void print_contact(const ContactInfo* contactInfo)
{
	/**
	 * @brief print a contactInfo into the search result format
	 * if contactInfo is NULL, then print NA
	 */
	if (contactInfo == NULL) {
		printf(": %-5d: %-32s: %-32s: %-32s:\n", -1, "NA", "NA", "NA");
		for (int j = 1; j < PHONE_NUMBER_COUNT; ++j) {
			printf(": %*c: %*c: %-32s: %-32s:\n", 5, ' ', 32, ' ', "", "");
		}
		printf("==============================================================================================================\n");
	} else {
		printf(": %-5d: %-32s: %-32s: %-32s:\n",
			contactInfo->si_no, contactInfo->name[0], contactInfo->phone_numbers[0], contactInfo->email_addresses[0]
		);
		for (int j = 1; j < PHONE_NUMBER_COUNT; ++j) {
			printf(": %*c: %*c: %-32s: %-32s:\n", 5, ' ', 32, ' ', contactInfo->phone_numbers[j], contactInfo->email_addresses[j]);
		}
		printf("==============================================================================================================\n");
	}
}