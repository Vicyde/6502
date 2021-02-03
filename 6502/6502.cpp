#include <iostream>
#include "cpu.h" 

int main(int argc, char* argv[]) {
    CPU cpu;

    // Test program
    byte program[] = {
        0xA9, 0x0A, // LDA #$0A
        0x85, 0x05, // STA $05
        0xA9, 0x00, // LDA #$00
        0xA5, 0x05, // LDA $05 
        0x48        // PHA
    };

    cpu.LoadProgramFromArray(program, sizeof(program) / sizeof(byte));

    cpu.Execute();
    cpu.PrintRegisters();
    return 0;
}
