#compiler
CC=g++

#compiler options
CO=-std=c++11 -c

all: main.o mos6502

tests: CpuTest.o test.o mos6502

mos6502: BasicMemory.o Exceptions.o Memory.o MOS6502CPU.o OppCodeMap.o Stack.o StatusRegister.o Utility.o
	$(CC) *.o -o mos6502

BasicMemory.o: MOS6502/BasicMemory.cpp
	$(CC) $(CO) MOS6502/BasicMemory.cpp

CommandMap.o: MOS6502/CommandMap.cpp
	$(CC) $(CO) MOS6502/CommandMap.cpp

Compiler.o: MOS6502/Compiler.cpp
	$(CC) $(CO) MOS6502/Compiler.cpp

CpuTest.o: MOS6502/CpuTest.cpp
	$(CC) $(CO) MOS6502/CpuTest.cpp

Exceptions.o: MOS6502/Exceptions.cpp
	$(CC) $(CO) MOS6502/Exceptions.cpp

main.o: main.cpp
	$(CC) $(CO) main.cpp

test.o: test.cpp
	$(CC) $(CO) test.cpp

Memory.o: MOS6502/Memory.cpp
	$(CC) $(CO) MOS6502/Memory.cpp

MOS6502CPU.o: MOS6502/MOS6502CPU.cpp
	$(CC) $(CO) MOS6502/MOS6502CPU.cpp

OppCodeMap.o: MOS6502/OppCodeMap.cpp
	$(CC) $(CO) MOS6502/OppCodeMap.cpp

Stack.o: MOS6502/Stack.cpp
	$(CC) $(CO) MOS6502/Stack.cpp

StatusRegister.o: MOS6502/StatusRegister.cpp
	$(CC) $(CO) MOS6502/StatusRegister.cpp

Utility.o: MOS6502/Utility.cpp
	$(CC) $(CO) MOS6502/Utility.cpp

clean:
	rm *.o mos6502
