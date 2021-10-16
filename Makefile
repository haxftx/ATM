#!/bin/bash
CC=gcc
CFLAGS= -Wall -lm
 
build:atm

atm.o:atm.c struct.h
	$(CC) $(CFLAGS) -o atm.o -c atm.c
atm:atm.o
	$(CC) $(CFLAGS) -o atm atm.o
run:atm
	./atm 
clean: 
	rm -f atm *.o

