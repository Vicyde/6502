#include <cstdio>
#include "cpu.h"

// Opcode lookup table
opcode_table opcodes[] = {
    // Opcode, Function, Cycles

    // LDA
    { 0xA9, OP_LDA_IMM, 2 },
    { 0xA5, OP_LDA_ZP, 2 },

    // PHA
    { 0x48, OP_PHA_IMM, 3 },

    // STA
    { 0x85, OP_STA_ZP, 3},

    // LDX
    { 0xA2, OP_LDX_IMM, 2 },

    // JMP
    { 0x4C, OP_JMP_ABS, 3 },
};


CPU::CPU()
{
	ResetRegisters();
	ClearMemory();
}

CPU::~CPU()
{

}

void CPU::PrintRegisters()
{
    std::printf("A:\t 0x%.2X\n", reg_a);
    std::printf("X:\t 0x%.2X\n", reg_x);
    std::printf("Y:\t 0x%.2X\n", reg_y);
    std::printf("STATUS:\t 0x%.2X\n", status);
    std::printf("PC:\t 0x%.4X\n", pc);
    std::printf("SP:\t 0x%.4X\n", sp);
}

void CPU::ResetRegisters() {
    reg_a = 0;
    reg_x = 0;
    reg_y = 0;
    sp = 0x01FF;
    pc = 0x0600;
    status = 0;
}

void CPU::ClearMemory() {
    for (int i = 0; i < sizeof(memory) / sizeof(byte); i++) {
        memory[i] = 0x00;
    }
}

byte CPU::ReadNextByte() {
    byte data = memory[pc];
    pc++;
    return data;
}

word CPU::ReadNextWord() {
    byte hi = memory[pc];
    pc++;
    byte lo = memory[pc];
    pc++;

    word value = (hi << 8) | lo;
    return value;
}

void CPU::LoadProgramFromArray(byte* program, unsigned int size) {
    for (int i = 0; i < size; i++) {
        memory[0x0600 + i] = program[i];
    }
}

void CPU::Execute(unsigned int cycles) {
    while (cycles > 0) {
        byte instruction = ReadNextByte();
        // Find appropiate function
        for (int i = 0; i < sizeof(opcodes) / sizeof(opcode_table); i++)
        {
            if (opcodes[i].opcode == instruction) {
                cycles -= opcodes[i].func(this);
            }
        }
    }
}

void CPU::Execute() {
    while (true) {
        // We run until we encountar 0x00
        byte instruction = ReadNextByte();
        if (instruction == 0x00)
            return;

        // Find appropiate function
        for (int i = 0; i < sizeof(opcodes) / sizeof(opcode_table); i++)
        {
            if (opcodes[i].opcode == instruction) {
                opcodes[i].func(this);
            }
        }
    }
}


//
// CPU Opcode Functions
//
void LDASetStatus(CPU* cpu) {
    if (cpu->reg_a == 0) cpu->status |= STATUS_BIT_Z;
    if ((cpu->reg_a & 0x40) > 0) cpu->status |= STATUS_BIT_N;    // Bit 7 set?
}

void LDXSetStatus(CPU* cpu) {
    if (cpu->reg_x == 0) cpu->status |= STATUS_BIT_Z;
    if ((cpu->reg_x & 0x40) > 0) cpu->status |= STATUS_BIT_N;    // Bit 7 set?
}

void LDYSetStatus(CPU* cpu) {
    if (cpu->reg_y == 0) cpu->status |= STATUS_BIT_Z;
    if ((cpu->reg_y & 0x40) > 0) cpu->status |= STATUS_BIT_N;    // Bit 7 set?
}

int OP_LDA_IMM(CPU* cpu) {
    byte value = cpu->ReadNextByte();
    cpu->reg_a = value;
    LDASetStatus(cpu);
    return 2;
}

int OP_LDA_ZP(CPU* cpu) {
    byte address = cpu->ReadNextByte();
    cpu->reg_a = cpu->memory[address];
    LDASetStatus(cpu);
    return 2;
}

int OP_PHA_IMM(CPU* cpu) {
    cpu->memory[cpu->sp] = cpu->reg_a;
    cpu->sp--;
    return 3;
}

int OP_STA_ZP(CPU* cpu) {
    byte address = cpu->ReadNextByte();
    cpu->memory[address] = cpu->reg_a;

    return 3;
}


int OP_LDX_IMM(CPU* cpu) {
    byte value = cpu->ReadNextByte();
    cpu->reg_x = value;
    LDXSetStatus(cpu);
    return 2;
}


int OP_JMP_ABS(CPU* cpu) {
    word address = cpu->ReadNextWord();
    cpu->pc = address;
    return 3;
}
