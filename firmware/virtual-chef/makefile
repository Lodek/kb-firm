
main: main.o chef.o scan.o maps.h
	gcc main.o chef.o scan.o -o main

scan.o: scan.c
	gcc -c scan.c -o scan.o

main.o: main.c
	gcc -c main.c -o main.o

chef.o: chef.c
	gcc -c chef.c -o chef.o

