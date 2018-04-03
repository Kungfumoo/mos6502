#include <iostream>
#include <exception>
#include "MOS6502CPU.h"
#include "BasicMemory.h"

using namespace std;
using namespace MOS_6502;

int main()
{
    Memory* memory = new BasicMemory();
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);

    vector<byte> program = {0xA9, 0x0A, 0x69, 0x10, 0x00};

    cpu->execute(program);
    cpu->status(false);

#ifdef _WIN32
	system("pause");
#endif

    delete cpu;
    return 0;
}
