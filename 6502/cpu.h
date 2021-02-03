#ifndef __CPU_H_
#define __CPU_H_

#include <array>

typedef unsigned char byte;
typedef unsigned int word;

// Status register bits
// TODO: Convert to enum
#define STATUS_BIT_N 1
#define STATUS_BIT_V 2
#define STATUS_BIT_B 4
#define STATUS_BIT_D 8
#define STATUS_BIT_I 16
#define STATUS_BIT_Z 32
#define STATUS_BIT_C 64

// Register identifier, for later use
// TODO: Convert to enum
#define REG_NONE    0
#define REG_A       1
#define REG_X       2
#define REG_Y       4
#define REG_STATUS  8
#define REG_SP      16
#define REG_PC      32

#define MEM_SIZE 65536

class CPU {
public:
    byte reg_a;
    byte reg_x;
    byte reg_y;
    byte status; // C Z I D B V N 
    word sp;
    word pc;

    std::array<byte, MEM_SIZE> memory;

    CPU();
    ~CPU();

    void PrintRegisters();
    void ResetRegisters();
    void ClearMemory();

    byte ReadNextByte();
    word ReadNextWord();

    void LoadProgramFromArray(byte* program, unsigned int size, int address = 0x0600);

    void Execute(unsigned int cycles);
    void Execute();
};


// CPU Operations
int OP_LD_IMM(CPU* cpu, byte param);
int OP_LD_ZP(CPU* cpu, byte param);

// PHA
int OP_PHA_IMM(CPU* cpu, byte param);
// STA
int OP_STA_ZP(CPU* cpu, byte param);

// JMP
int OP_JMP_ABS(CPU* cpu, byte param);


struct opcode_table {
    byte opcode;
    int (*func)(CPU*, byte );
    byte parameter;
    unsigned int cycles;
};

#endif //__CPU_H_