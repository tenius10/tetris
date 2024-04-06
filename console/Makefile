FLAGS = -Werror -std=c++11 -g
CC = g++

all: main

main: template.o console.o
	$(CC) $(FLAGS) -o main template.o console.o -lncurses

template.o: template.cpp console.h
	$(CC) $(FLAGS) -c template.cpp

console.o: console.cpp console.h
	$(CC) $(FLAGS) -c console.cpp

clean:
	rm -f *.o
