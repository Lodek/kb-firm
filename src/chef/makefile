libchef.a: chef.o
	ar -q libchef.a chef.o

chef.o: chef.c chef.h
	gcc -c chef.c -o chef.o

clean:
	rm *.o *.a
