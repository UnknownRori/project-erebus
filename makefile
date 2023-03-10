CC = g++
FLAG = -Wall -Werror -g -std=c++2a

MAIN = main.cpp
OUT = main

all: build
	./$(OUT)

debug: build
	gdb ./$(OUT)

clean: build
	rm ./$(OUT)

build:
	$(CC) $(MAIN) -o $(OUT) $(FLAG)


