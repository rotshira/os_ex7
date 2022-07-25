.PHONY: all run clean
CC = gcc -fPIC -g -w
OBJECTS = fs.o Mylibc.o 

all: test lib.so

main: main.o  fs.o  Mylibc.o 
	$(CC) main.o  Mylibc.o fs.o -o main

test: main.o  fs.o  Mylibc.o 
	$(CC) main.o  Mylibc.o fs.o -o test


lib.so: $(OBJECTS)
	$(CC) --shared -fPIC -g -pthread $(OBJECTS) -o lib.so

%.o: %.c 
	$(CC) -c $< -o $@

clean:
	rm -f *.o main TEST lib.so
