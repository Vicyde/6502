#include <iostream>
#include "cpu.h" 


int main(int argc, char* argv[]) {
    // CPU needs to be put on the heap because of its huge 'memory' variable. Would be nice to separate this from
    // the class.
    CPU *cpu = new CPU();

    /*
    // Test program
    byte program[] = {
        0xA9, 0x42, // LDA #$42
        0x48,       // PHA
        0xA9, 0x00, // LDA #$00
        0x68        // PLA
    };
    */

    cpu->status = 0xFF;

    byte program[] = {
        0x18, // Clear Carry
        0x58, // Clear Interrupt
        0xB8, // Clear Overflow
        0xD8  // Clear Decimal
    };

    // At this point status should be
    // N V - B D I Z C
    // 1 0 1 1 0 0 1 0 (0xB2)

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
