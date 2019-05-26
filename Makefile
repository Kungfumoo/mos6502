#Manual: https://www.gnu.org/software/make/manual/make.html
#Variable instantiation:
#	':=' - value is worked out as it is defined
#   '=' - value is worked out when it is called

#dirs
BASE_SRC_DIR := src

#needs to include all subfolders to compile too
SRC_DIRS := $(BASE_SRC_DIR)/MOS6502
TEST_DIRS := $(BASE_SRC_DIR)/MOS6502/Test
BUILD_DIR := obj
TARGET_EXE := bin/mos6502

#find all source files in the src_dirs
src = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.cpp))
testSrc = $(foreach dir, $(TEST_DIRS), $(wildcard $(dir)/*.cpp))

#create a new list based on the src list but replace .cpp with .o and the base directory
obj = $(patsubst $(BASE_SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(src))
testObj = $(patsubst $(BASE_SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(testSrc))

#compiler
CC := g++

#compiler options
CO := -std=c++11

mos6502: mkdir $(BUILD_DIR)/main.o $(obj)
	$(CC) $(BUILD_DIR)/main.o $(obj) -o $(TARGET_EXE)

tests: SRC_DIRS += $(TEST_DIRS) #target specific variable (Context is recipe only, rules cannot see this change)
tests: mkdir $(BUILD_DIR)/test.o $(obj) $(testObj)
	$(CC) $(BUILD_DIR)/test.o $(obj) -o $(TARGET_EXE)

mkdir:
	mkdir -p obj bin $(subst $(BASE_SRC_DIR), $(BUILD_DIR), $(SRC_DIRS))

#rule for compiling .cpp to .o
#target: dependencies
#	recipe
$(BUILD_DIR)/%.o: $(BASE_SRC_DIR)/%.cpp $(BASE_SRC_DIR)/%.h
	$(CC) $(CO) -c -o $@ $<

#main and test are seperate as they have no header file dependency
$(BUILD_DIR)/%.o: $(BASE_SRC_DIR)/%.cpp
	$(CC) $(CO) -c -o $@ $<

clean:
	rm -r -f obj bin
