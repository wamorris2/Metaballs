CC = g++
CCFLAGS = -W -Wall -Wpedantic
INC = -I./include -I./include/imgui
LIB = -L./lib -lsfml-graphics -lsfml-window -lsfml-system -lGL
CPP_FILES = $(wildcard ./src/*cpp) $(wildcard ./include/imgui/*.cpp)
OUTPUT = ./bin/main

all: clean compile

compile:
	if [ ! -d "./bin" ]; then\
		mkdir bin;\
	fi
	$(CC) $(CCFLAGS) $(CPP_FILES) -o $(OUTPUT)  $(INC) $(LIB)

clean:
	rm $(OUTPUT)

run:
	$(OUTPUT)


