#compiler
CC=g++

#compiler options
CO=-std=c++11 -c

all: main.o mos6502

tests: CpuTest.o test.o mos6502

mos6502: BasicMemory.o Exceptions.o Memory.o MOS6502CPU.o OppCodeMap.o Stack.o StatusRegister.o Utility.o
	$(CC) *.o -o mos6502

BasicMemory.o: BasicMemory.cpp
	$(CC) $(CO) BasicMemory.cpp

CommandMap.o: CommandMap.cpp
	$(CC) $(CO) CommandMap.cpp

Compiler.o: Compiler.cpp
	$(CC) $(CO) Compiler.cpp

CpuTest.o: CpuTest.cpp
	$(CC) $(CO) CpuTest.cpp

Exceptions.o: Exceptions.cpp
	$(CC) $(CO) Exceptions.cpp

main.o: main.cpp
	$(CC) $(CO) main.cpp

test.o: test.cpp
	$(CC) $(CO) test.cpp

Memory.o: Memory.cpp
	$(CC) $(CO) Memory.cpp

MOS6502CPU.o: MOS6502CPU.cpp
	$(CC) $(CO) MOS6502CPU.cpp

OppCodeMap.o: OppCodeMap.cpp
	$(CC) $(CO) OppCodeMap.cpp

Stack.o: Stack.cpp
	$(CC) $(CO) Stack.cpp

StatusRegister.o: StatusRegister.cpp
	$(CC) $(CO) StatusRegister.cpp

Utility.o: Utility.cpp
	$(CC) $(CO) Utility.cpp

clean:
	rm *.o mos6502
