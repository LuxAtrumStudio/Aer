CPP_FILES = $(wildcard *.cpp)
OBJ_FILES = $(CPP_FILES:.cpp=.o)
TOTAL_OBJ_FILES = $(wildcard */*.o) $(wildcard */*/*.o) $(wildcard */*/*/*.o)
CC = g++
COMPILER_FLAGS = -MMD -std=c++11 -w -c
LINKER_FLAGS = -laequus -lpessum -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
PROGRAM_NAME = aer

all: subsystem top_obj $(PROGRAM_NAME)
	clear
	@echo Compleated compiling $(PROGRAM_NAME)

$(PROGRAM_NAME): $(OBJ_FILES) $(wildcard */*.o) $(wildcard */*/*.o) $(wildcard */*/*/*.o)
	setterm -foreground red
	$(CC) $(OBJ_FILES) $(TOTAL_OBJ_FILES) -o $(PROGRAM_NAME) $(LINKER_FLAGS)
	setterm -default

%.o: %.cpp
	g++ $(COMPILER_FLAGS) -o $(notdir $*).o $*.cpp

.PHONY : top_obj
top_obj:$(OBJ_FILES)
	@echo $(OBJ_FILES)
	@echo $(CPP_FILES)

.PHONY : subsystem
subsystem:
	setterm -foreground blue
	cd aer_files && $(MAKE)
	setterm -default

.PHONY : clean
clean:
	rm -f *.o
	rm -f *.d
	rm -f */*.o
	rm -f */*.d
	rm -f */*/*.o
	rm -f */*/*.d
	rm -f */*/*/*.o
	rm -f */*/*/*.d
	clear
	@echo Cleared all '.o' and '.d' files

.PHONY : run
run: subsystem top_obj $(PROGRAM_NAME)
	./$(PROGRAM_NAME)

.PHONY : compress
compress:
	cd ../
	tar -zcvf $(PROGRAM_NAME).tar.gz $(PROGRAM_NAME)

.PHONY : lib
lib:
	ar rcs lib$(PROGRAM_NAME).a $(TOTAL_OBJ_FILES)

.PHONY : log
log:
	less log_output.log
