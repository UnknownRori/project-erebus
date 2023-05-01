CC = g++
FLAG = -Wall -Werror -g -std=c++2a
STATIC_LINK_STD = -static -static-libgcc

MAIN_SRC = main.cpp
MAIN_OUT = erebus

EREBUS_SRC = ./src/erebus.cpp
EREBUS_OBJ = erebus.o
EREBUS_OUT_STATIC_LIB = liberebus.a
EREBUS_OUT_SHARED_LIB = liberebus.so
EREBUS_SHARED_FLAG = -lerebus

LINKER_PATH = -L./dist

all: build
	./$(MAIN_OUT)

debug: build
	gdb ./$(MAIN_OUT)

clean:
	rm ./$(MAIN_OUT)
	rm ./dist/*.a

build: erebus-build-staticlib
	$(CC) $(MAIN_SRC) -o $(MAIN_OUT) $(FLAG) $(LINKER_PATH) $(EREBUS_SHARED_FLAG)

build-static: erebus-build-staticlib
	$(CC) $(MAIN_SRC) -o $(MAIN_OUT) $(FLAG) $(LINKER_PATH) $(EREBUS_SHARED_FLAG) $(STATIC_LINK_STD)

erebus-build-lib:
	make erebus-build-staticlib

erebus-build-staticlib:
	$(CC) $(EREBUS_SRC) -c -o ./$(EREBUS_OBJ) $(FLAG)
	[ ! -e ./dist ] && mkdir ./dist
	ar rcs ./dist/$(EREBUS_OUT_STATIC_LIB) ./$(EREBUS_OBJ)
	rm ./$(EREBUS_OBJ)

erebus-clean:
	rm ./$(EREBUS_OBJ)
