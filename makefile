CC = g++
FLAG = -Wall -Werror -g -std=c++2a -save-temps=obj

MAIN = main.cpp
OUT = main

all: build
	./$(OUT)

debug: build
	gdb ./$(OUT)

clean:
	rm ./$(OUT)
	rm ./$(OUT).ii
	rm ./$(OUT).s
	rm ./$(OUT).o

build:
	$(CC) $(MAIN) -o $(OUT) $(FLAG)


