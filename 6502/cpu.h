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


//ADC	....add with carry
//AND	.... and (with accumulator)
//ASL	....arithmetic shift left
//BCC	....branch on carry clear
//BCS	....branch on carry set
//BEQ	....branch on equal(zero set)
//BIT	....bit test
//BMI	....branch on minus(negative set)
//BNE	....branch on not equal(zero clear)
//BPL	....branch on plus(negative clear)
//BRK	....	break / interrupt
//BVC	....branch on overflow clear
//BVS	....branch on overflow set
//CLC	....clear carry
//CLD	....clear decimal
//CLI	....clear interrupt disable
//CLV	....clear overflow
//CMP	....compare(with accumulator)
//CPX	....compare with X
//CPY	....compare with Y
//DEC	....decrement
//DEX	....decrement X
//DEY	....decrement Y
//EOR	....exclusive or (with accumulator)
//INC	....increment
//INX	....increment X
//INY	....increment Y
//JMP	....jump
int OP_JMP_ABS(CPU* cpu, byte param);

//JSR	....jump subroutine

//LDA	....load accumulator
//LDX	....load X
//LDY	....load Y
int OP_LD_IMM(CPU* cpu, byte param);
int OP_LD_ZP(CPU* cpu, byte param);
int OP_LD_ABS(CPU* cpu, byte param);

//LSR	....logical shift right
//NOP	....no operation
//ORA	.... or with accumulator

//PHA	....push accumulator
int OP_PHA_IMP(CPU* cpu, byte param);

//PHP	....push processor status(SR)
//PLA	....pull accumulator
int OP_PLA_IMP(CPU* cpu, byte param);

//PLP	....pull processor status(SR)
//ROL	....rotate left
//ROR	....rotate right
//RTI	....	return from interrupt
//RTS	....	return from subroutine
//SBC	....subtract with carry
//SEC	....set carry
//SED	....set decimal
//SEI	....set interrupt disable
//STA	....store accumulator
int OP_STA_ZP(CPU* cpu, byte param);

//STX	....store X
//STY	....store Y
//TAX	....transfer accumulator to X
//TAY	....transfer accumulator to Y
//TSX	....transfer stack pointer to X
//TXA	....transfer X to accumulator
//TXS	....transfer X to stack pointer
//TYA	....transfer Y to accumulator

struct opcode_table {
    byte opcode;
    int (*func)(CPU*, byte );
    byte parameter;
};

#endif //__CPU_H_