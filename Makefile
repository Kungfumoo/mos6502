#dirs
SRC_DIR=src
BUILD_DIR=obj
TARGET_EXE=bin/mos6502

src=$(wildcard $(SRC_DIR)/*.cpp) \
	$(wildcard $(SRC_DIR)/MOS6502/*.cpp) \
	$(wildcard $(SRC_DIR)/MOS6502/CompilerAssets/*.cpp)

#compiler
CC=g++

#compiler options
CO=-std=c++11 -c -o $@

all: dir $(BUILD_DIR)/main.o mos6502

tests: $(BUILD_DIR)/CpuTest.o $(BUILD_DIR)/test.o mos6502

dir:
	mkdir obj bin

mos6502: $(BUILD_DIR)/BasicMemory.o $(BUILD_DIR)/Exceptions.o $(BUILD_DIR)/Memory.o $(BUILD_DIR)/MOS6502CPU.o $(BUILD_DIR)/OppCodeMap.o $(BUILD_DIR)/Stack.o $(BUILD_DIR)/StatusRegister.o $(BUILD_DIR)/Utility.o
	$(CC) $(BUILD_DIR)/*.o -o $(TARGET_EXE)

$(obj): $(src)
	$(CC) $(CO) $^

clean:
	rm -r obj bin
