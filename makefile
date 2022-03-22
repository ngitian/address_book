# makefile is for windows at the moment, need to update for linux and mac

CC=gcc

all: program

program: src/main.c src/address_book_fops.c src/address_book_menu.c
	$(CC) src/main.c src/address_book_fops.c src/address_book_menu.c -o bin/main
	./bin/main.exe

debug: src/main.c src/address_book_fops.c src/address_book_menu.c
	$(CC) src/main.c src/address_book_fops.c src/address_book_menu.c -g -O0 -o bin/d_main

clear: 
	rm bin/main.exe