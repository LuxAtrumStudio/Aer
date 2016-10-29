CPP_FILES = $(wildcard *.cpp)
OBJ_FILES = $(notdir $(CPP_FILES:.cpp=.o))
CC = g++
COMPILER_FLAGS = -MMD -std=c++11 -w -c
main:$(OBJ_FILES)
%.o: %.cpp %.h
	g++ $(COMPILER_FLAGS) -o $(notdir $*).o $*.cpp
