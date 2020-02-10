CHEF_DIR = ../chef
CC = gcc
CFLAGS = -I$(CHEF_DIR)
TARGET = main
OUTPUT = main

$TARGET: main.o scan.o maps.h libchef.a
	gcc main.o scan.o -L$(CHEF_DIR) -lchef $(CFLAGS)  -o $(OUTPUT)

scan.o: scan.c
	gcc $(CFLAGS) -c scan.c -o scan.o

main.o: main.c 
	gcc $(CFLAGS) -c main.c -o main.o

libchef.a:
	cd $(CHEF_DIR) && make

clean:
	rm *.o maps.h
