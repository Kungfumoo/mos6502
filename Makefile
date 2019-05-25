#Manual: https://www.gnu.org/software/make/manual/make.html
#dirs
BASE_SRC_DIR = src

#needs to include all subfolders to compile too
SRC_DIRS = $(BASE_SRC_DIR)/MOS6502
BUILD_DIR = obj
TARGET_EXE = bin/mos6502

#find all source files in the src_dirs
src := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.cpp))

#create a new list based on the src list but replace .cpp with .o and the base directory
obj := $(patsubst $(BASE_SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(src))

#compiler
CC = g++

#compiler options
CO = -std=c++11

#rule for compiling .cpp to .o
#target: dependencies
#	recipe
$(BUILD_DIR)/%.o: $(BASE_SRC_DIR)/%.cpp
	$(CC) $(CO) -c -o $@ $<

mos6502: mkdir $(BUILD_DIR)/main.o $(obj)
	$(CC) $(BUILD_DIR)/main.o $(obj) -o $(TARGET_EXE)

tests: mkdir $(BUILD_DIR)/test.o $(obj)
	$(CC) $(BUILD_DIR)/test.o $(obj) -o $(TARGET_EXE)

mkdir:
	mkdir -p obj bin $(subst $(BASE_SRC_DIR), $(BUILD_DIR), $(SRC_DIRS))

clean:
	rm -r obj bin
