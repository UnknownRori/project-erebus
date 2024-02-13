CC = g++
FLAG = -Wall -Werror -g -std=c++2a
STATIC_LINK_STD = -static -static-libgcc

MAIN_SRC = main.cpp
MAIN_OUT = erebus
UI_SRC = ui.cpp
UI_OUT = erebus-ui

EREBUS_SRC = ./src/erebus.cpp
EREBUS_OBJ = erebus.o
EREBUS_OUT_STATIC_LIB = liberebus.a
EREBUS_OUT_SHARED_LIB = liberebus.so
EREBUS_SHARED_FLAG = -lerebus
EREBUS_UI_FLAG = -lraylib -lopengl32 -lgdi32 -lwinmm

LINKER_PATH = -L./dist

all: build
	./$(MAIN_OUT)

# Create dist directory if not exists
mkdir-dist:
	([ ! -e ./dist ] && mkdir dist) || [ -e ./dist ]

debug: build
	gdb ./$(MAIN_OUT)

clean:
	rm ./$(MAIN_OUT)
	rm ./$(UI_OUT)
	rm ./dist/*.a

build-ui: erebus-build-staticlib
	$(CC) $(UI_SRC) -o $(UI_OUT) $(EREBUS_UI_FLAG) $(LINKER_PATH) $(EREBUS_SHARED_FLAG) $(STATIC_LINK_STD)

build: erebus-build-staticlib
	$(CC) $(MAIN_SRC) -o $(MAIN_OUT) $(FLAG) $(LINKER_PATH) $(EREBUS_SHARED_FLAG)

build-static: erebus-build-staticlib
	$(CC) $(MAIN_SRC) -o $(MAIN_OUT) $(FLAG) $(LINKER_PATH) $(EREBUS_SHARED_FLAG) $(STATIC_LINK_STD)

# Make sure you have emscripten
# Also put wasm compiled of raylib to the libs directory
# build-web: erebus-build-weblib
# Probably also need to use raylib.h too
build-web:
	em++ ./ui.cpp ./src/erebus.cpp ./dist/liberebusweb.a ./libs/libraylib.a -o ./web/index.html -s USE_GLFW=3 -DPLATFORM_WEB -s ASSERTIONS=2 -Wall -sEXPORT_EXCEPTION_HANDLING_HELPERS -fwasm-exceptions --profiling-funcs

# em++ ./ui.cpp ./src/erebus.cpp ./dist/liberebusweb.a ./libs/libraylib.a -o ./web/index.html -s USE_GLFW=3 -DPLATFORM_WEB -s ASSERTIONS=2 -Os

erebus-build-lib:
	make erebus-build-staticlib

erebus-build-staticlib: mkdir-dist
	$(CC) $(EREBUS_SRC) -c -o ./$(EREBUS_OBJ) $(FLAG)
	ar rcs ./dist/$(EREBUS_OUT_STATIC_LIB) ./$(EREBUS_OBJ)
	rm ./$(EREBUS_OBJ)

erebus-clean:
	rm ./$(EREBUS_OBJ)

# erebus-build-weblib:
# 	em++ ./src/erebus.cpp -c dist/erebusweb.o
# 	emar rcs dist/liberebusweb.a dist/erebusweb.o
