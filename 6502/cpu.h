#ifndef __CPU_H_
#define __CPU_H_

typedef unsigned char byte;
typedef unsigned int word;

// Status register bits
#define STATUS_BIT_N 1
#define STATUS_BIT_V 2
#define STATUS_BIT_B 4
#define STATUS_BIT_D 8
#define STATUS_BIT_I 16
#define STATUS_BIT_Z 32
#define STATUS_BIT_C 64

class CPU {
public:
    byte reg_a;
    byte reg_x;
    byte reg_y;
    byte status; // C Z I D B V N 
    word sp;
    word pc;

    byte memory[65536];

    CPU();
    ~CPU();

    void PrintRegisters();
    void ResetRegisters();
    void ClearMemory();

    byte ReadNextByte();
    word ReadNextWord();

    void LoadProgramFromArray(byte* program, unsigned int size);

    void Execute(unsigned int cycles);
    void Execute();
};


// CPU Operations
// LDA
int OP_LDA_IMM(CPU* cpu);
int OP_LDA_ZP(CPU* cpu);
// PHA
int OP_PHA_IMM(CPU* cpu);
// STA
int OP_STA_ZP(CPU* cpu);
// LDX
int OP_LDX_IMM(CPU* cpu);
// JMP
int OP_JMP_ABS(CPU* cpu);


struct opcode_table {
    byte opcode;
    int (*func)(CPU*);
    unsigned int cycles;
};

#endif //__CPU_H_