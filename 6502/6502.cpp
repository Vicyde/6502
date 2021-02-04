#include <iostream>
#include "cpu.h" 


int main(int argc, char* argv[]) {
    // CPU needs to be put on the heap because of its huge 'memory' variable. Would be nice to separate this from
    // the class.
    CPU *cpu = new CPU();


    // Test program
    byte program[] = {
        0xA9, 0x42, // LDA #$42
        0x48,       // PHA
        0xA9, 0x00, // LDA #$00
        0x68        // PLA
    };

    try
    {
        cpu->LoadProgramFromArray(program, sizeof(program) / sizeof(byte));

        cpu->Execute();
        cpu->PrintRegisters();
    }
    catch (std::string str)
    {
        std::cout << str;
    }

    delete cpu;

    return 0;
}
