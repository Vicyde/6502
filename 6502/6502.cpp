#include <iostream>
#include "cpu.h" 


int main(int argc, char* argv[]) {
    // CPU needs to be put on the heap because of its huge 'memory' variable. Would be nice to separate this from
    // the class.
    CPU *cpu = new CPU();

    // Inject a variable to test absolute addressing
    cpu->memory[0x1337] = 0x42;

    // Test program
    byte program[] = {
        0xA9, 0x0A,         // LDA #$0A -- Load 0x0A in accumulator, immediate                  (LDA = 0x0A)
        0x85, 0x05,         // STA $05  -- Store accumulator at memory address 05, zeropge      (LDA = 0x0A)
        0xAD, 0x13, 0x37,   // LDA #$01 -- Load memory address 0x1337 in accumulator, absolute  (LDA = 0x42)
        0xA6, 0x05,         // LDX $05  -- Load memory address 05 into X, zeropage              (LDA = 0x01, LDX = 0x0A)
        0xA0, 0x02,         // LDY #$02 -- Load 0x02 into Y, immediate                          (LDA = 0x0z, LDX = 0x0A, LDY = 0x02)
        0x48                // PHA      -- Push accumulator to the stack                        (LDA = 0x0A, $05 = 0x0A, stack[0] = 0x0A)
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
