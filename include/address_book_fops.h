#ifndef ABK_FILEOPS_H
#define ABK_FILEOPS_H

#include "address_book.h"

Status load_file(AddressBook *address_book);
Status save_file(AddressBook *address_book);

void parseCSVRow(AddressBook *address_book, int row);

#endif
