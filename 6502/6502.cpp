#include <iostream>
#include "cpu.h" 


int main(int argc, char* argv[]) {
    // CPU needs to be put on the heap because of its huge 'memory' variable. Would be nice to separate this from
    // the class.
    CPU *cpu = new CPU();

    // Test program
    byte program[] = {
        0xA9, 0x0A, // LDA #$0A -- Load 0x0A in accumulator                 (LDA = 0x0A)
        0x85, 0x05, // STA $05  -- Store accumulator at memory address 05   (LDA = 0x0A)
        0xA9, 0x00, // LDA #$01 -- Load 0x01 in accumulator                 (LDA = 0x01)
        0xA6, 0x05, // LDX $05  -- Load memory address 05 into X            (LDA = 0x01, LDX = 0x0A)
        0xA0, 0x02, // LDY #$02 -- Load 0x02 into Y                         (LDA = 0x0z, LDX = 0x0A, LDY = 0x02)
        0x48        // PHA      -- Push accumulator to the stack            (LDA = 0x0A, $05 = 0x0A, stack[0] = 0x0A)
    };

    cpu->LoadProgramFromArray(program, sizeof(program) / sizeof(byte));

    cpu->Execute();
    cpu->PrintRegisters();

    delete cpu;

    return 0;
}
