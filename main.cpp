#include <iostream>
#include <exception>
#include <stdexcept>
#include <string>
#include <fstream>
#include "MOS6502CPU.h"
#include "BasicMemory.h"

using namespace std;
using namespace MOS_6502;

vector<byte> fetchProgram(string filename)
{
    ifstream ifile(filename);

    if(!ifile.is_open())
        throw new logic_error("Could not open " + filename);

    vector<byte> program;

    ifile.seekg(1); //avoid colon

    while(!ifile.eof())
    {
        byte size = 2;
        char* buffer = new char[size];

        ifile.read(buffer, size);

        //TODO: probably a better way of doing this...
        string hex(buffer);

        if(hex.size() == 2)
        {
            byte oppcode = (byte)stoi(hex, nullptr, 16);
            program.push_back(oppcode);
        }
    }

    ifile.close();

    return program;
}

int main(int argc, char* argv[])
{
    BasicMemory* memory = new BasicMemory();
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);

    if(argc == 1)
    {
        cout << "No program to run provided! Please provide a hex file to run" << endl;
        return 1;
    }

    string filename(argv[1]);

    //vector<byte> program = {0xA9, 0x2F, 0xC9, 0x2F, 0xD0, 0x03, 0x8D, 0x22, 0x00, 0x00};
    vector<byte> program = fetchProgram(filename);

    for(unsigned int i = 0; i < program.size(); i++)
        cout << "0x" << std::hex << (int)program[i] << endl;

    cpu->execute(program);
    cpu->status(false);

#ifdef _WIN32
	system("pause");
#endif

    delete cpu;
    return 0;
}
