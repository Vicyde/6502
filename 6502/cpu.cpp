#include <cstdio>
#include <string>
#include "cpu.h"

// Opcode lookup table
opcode_table opcodes[] = {
    // Format:
    //  { Opcode, Function, Parameter, Cycles }
    // JMP
    { 0x4C, OP_JMP_ABS, REG_NONE },  // Absolute
//  { 0x6C, OP_JMP_IN, REG_NONE },   // Indirect

    // LDA
    { 0xA9, OP_LD_IMM, REG_A },      // Immediate
    { 0xA5, OP_LD_ZP, REG_A },       // ZeroPage
//  { 0xB5, OP_LD_ZPX, REG_A },      // ZeroPage, X
    { 0xAD, OP_LD_ABS, REG_A },      // Absolute
//  { 0xBD, OP_LD_ABX, REG_A },      // Absolute, X
//  { 0xB9, OP_LD_ABX, REG_A },      // Absolute, Y
//  { 0xA1, OP_LD_INX, REG_A },      // Indirect, X
//  { 0xB1, OP_LD_INY, REG_A },      // Indirect, Y

    // LDX
    { 0xA2, OP_LD_IMM, REG_X },      // Immediate
    { 0xA6, OP_LD_ZP, REG_X },       // ZeroPage
//  { 0xB6, OP_LD_ZPY, REG_X },      // ZeroPage, Y
    { 0xAE, OP_LD_ABS, REG_X },      // Absolute
//  { 0xBE, OP_LD_ABY, REG_X },      // Absolute, Y

    // LDY
    { 0xA0, OP_LD_IMM, REG_Y },      // Immediate
    { 0xA4, OP_LD_ZP, REG_Y },       // ZeroPage
//  { 0xB4, OP_LD_ZPX, REG_Y },      // ZeroPage, X
    { 0xAC, OP_LD_ABS, REG_Y },      // Absolute
//  { 0xBC, OP_LD_ABX, REG_Y },      // Absolute, X

    // PHA
    { 0x48, OP_PHA_IMP, REG_A },     // Implied

    // PLA
    { 0x68, OP_PLA_IMP, REG_A },     // Implied

    // STA
    { 0x85, OP_STA_ZP, REG_A },       // ZeroPage
};
 

CPU::CPU()
{
	ResetRegisters();
	ClearMemory();
}

CPU::~CPU()
{

}

// Print register values to standard output
void CPU::PrintRegisters()
{
    std::printf("A:\t 0x%.2X\n", reg_a);
    std::printf("X:\t 0x%.2X\n", reg_x);
    std::printf("Y:\t 0x%.2X\n", reg_y);
    std::printf("STATUS:\t 0x%.2X\n", status);
    std::printf("PC:\t 0x%.4X\n", pc);
    std::printf("SP:\t 0x%.4X\n", sp);
}

// Resets registers to startup conditions.
void CPU::ResetRegisters() {
    reg_a = 0;
    reg_x = 0;
    reg_y = 0;
    sp = 0x01FF; // Stack at 0x0100 - 0x01FF
    pc = 0x0600;
    status = 0x0000000b;
}

// Initialize the CPU memory to all 0x00.
void CPU::ClearMemory() {
    for (auto it = memory.begin(); it != memory.end(); ++it) {
        *it = 0x00;
    }
}

// Reads the next byte in memory and increases the program counter.
byte CPU::ReadNextByte() {
    byte data = memory[pc++];
    return data;
}

// Reads the next two bytes (word) in memory and increases the program counter twice.
word CPU::ReadNextWord() {
    byte hi = memory[pc++];
    byte lo = memory[pc++];

    // Little endian
    word value = (hi << 8) | lo;
    return value;
}

// Loads a program from an array into the CPU it's memory at <address> (default 0x0600).
void CPU::LoadProgramFromArray(byte* program, unsigned int size, int address) {
    if (address + size > MEM_SIZE) {
        char ex[50];
        std::snprintf(ex, 50, "program exceeds memory limit at %i. Max is %i.", address + size, MEM_SIZE);
        throw std::string(ex);
    }

    for (unsigned int i = 0; i < size; i++) {
        memory[address + i] = program[i];
    }
}

// Runs the program in memory for <cycles> cycles.
void CPU::Execute(unsigned int cycles) {
    while (cycles > 0) {
        byte instruction = ReadNextByte();
        // Find appropiate function
        for (int i = 0; i < sizeof(opcodes) / sizeof(opcode_table); i++)
        {
            if (opcodes[i].opcode == instruction) {
                cycles -= opcodes[i].func(this, opcodes[i].parameter);
            }
        }
    }
}

// Runs the program in memory until 0x00 is encountered.
void CPU::Execute() {
    while (true) {
        byte instruction = ReadNextByte();
        if (instruction == 0x00)
            return;

        // Find appropiate function
        for (int i = 0; i < sizeof(opcodes) / sizeof(opcode_table); i++)
        {
            if (opcodes[i].opcode == instruction) {
                opcodes[i].func(this, opcodes[i].parameter);
            }
        }
    }
}

//
// CPU Opcode Functions
//

// Jump
int OP_JMP_ABS(CPU* cpu, byte param) {
    word address = cpu->ReadNextWord();
    cpu->pc = address;
    return 3;
}

// Sets the statusregister as required by LDA, LDX and LDY
void LDSetStatus(CPU *cpu, byte &reg)
{
    if (reg == 0) cpu->status |= STATUS_BIT_Z;            // Register is 0? (Z bit)
    if ((reg & 0x40) > 0) cpu->status |= STATUS_BIT_N;    // Bit 7 set? (N bit)
}

// Since LDA, LDX and LDY do the same, basically, we could put them all in one function?
// TODO: Implement
int OP_LD_IMM(CPU* cpu, byte param)
{
    byte* reg = NULL;
    byte value = cpu->ReadNextByte();
    switch (param) {
    case REG_A: reg = &cpu->reg_a; break;
    case REG_X: reg = &cpu->reg_x; break;
    case REG_Y: reg = &cpu->reg_y; break;
    }

    *reg = value;

    LDSetStatus(cpu, *reg);

    return 2;
}

int OP_LD_ZP(CPU* cpu, byte param) {
    byte address = cpu->ReadNextByte();
    byte* reg = NULL;
    switch (param) {
    case REG_A: reg = &cpu->reg_a; break;
    case REG_X: reg = &cpu->reg_x; break;
    case REG_Y: reg = &cpu->reg_y; break;
    }

    *reg = cpu->memory[address];

    LDSetStatus(cpu, *reg);

    return 3;
}

int OP_LD_ABS(CPU* cpu, byte param) {
    word address = cpu->ReadNextWord();
    byte* reg = NULL;
    switch (param) {
    case REG_A: reg = &cpu->reg_a; break;
    case REG_X: reg = &cpu->reg_x; break;
    case REG_Y: reg = &cpu->reg_y; break;
    }

    *reg = cpu->memory[address];

    LDSetStatus(cpu, *reg);

    return 4;
}

int OP_PHA_IMP(CPU* cpu, byte param) {
    cpu->memory[cpu->sp--] = cpu->reg_a;
    return 3;
}


int OP_PLA_IMP(CPU* cpu, byte param) {
    cpu->reg_a = cpu->memory[++cpu->sp];
    LDSetStatus(cpu, cpu->reg_a);
    return 4;
}

int OP_STA_ZP(CPU* cpu, byte param) {
    byte address = cpu->ReadNextByte();
    cpu->memory[address] = cpu->reg_a;

    return 3;
}

