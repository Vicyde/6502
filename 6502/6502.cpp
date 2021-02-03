#include <iostream>

typedef unsigned char byte;
typedef unsigned int word;

class CPU;

#define STATUS_BIT_N 1
#define STATUS_BIT_V 2
#define STATUS_BIT_B 4
#define STATUS_BIT_D 8
#define STATUS_BIT_I 16
#define STATUS_BIT_Z 32
#define STATUS_BIT_C 64

struct opcode_table {
    byte opcode;
    int (*func)(CPU *);
    unsigned int cycles;
};

int OP_LDA_IMM(CPU* cpu);
int OP_LDA_ZP(CPU* cpu);

int OP_PHA_IMM(CPU* cpu);

int OP_STA_ZP(CPU* cpu);

int OP_LDX_IMM(CPU* cpu);

int OP_JMP_ABS(CPU* cpu);

int OP_JMP_ABS(CPU* cpu) {
    word address = cpu->ReadNextWord();
    cpu->pc = address;
    return 3;
}

opcode_table opcodes[] = {
    // LDA
    { 0xA9, OP_LDA_IMM, 2 },
    { 0xA5, OP_LDA_ZP, 2 },

    { 0x48, OP_PHA_IMM, 3 },

    { 0x85, OP_STA_ZP, 3},

    // LDX
    { 0xA2, OP_LDX_IMM, 2 },

    { 0x4C, OP_JMP_ABS, 3 },
};

class CPU {
public:
    byte reg_a;
    byte reg_x;
    byte reg_y;
    byte status; // C Z I D B V N 
    word sp;
    word pc;

    byte memory[65536];

    CPU() {
        ResetRegisters();
        ClearMemory();
    }

    void PrintRegisters() {
        printf("A:\t 0x%.2X\n", reg_a);
        printf("X:\t 0x%.2X\n", reg_x);
        printf("Y:\t 0x%.2X\n", reg_y);
        printf("STATUS:\t 0x%.2X\n", status);
        printf("PC:\t 0x%.4X\n", pc);
        printf("SP:\t 0x%.4X\n", sp);
    }

    void ResetRegisters() {
        reg_a = 0;
        reg_x = 0;
        reg_y = 0;
        sp = 0x01FF;
        pc = 0x0600;
        status = 0;
    }

    void ClearMemory() {
        for (int i = 0; i < sizeof(memory) / sizeof(byte); i++) {
            memory[i] = 0x00;
        }
    }

    byte ReadNextByte() {
        byte data = memory[pc];
        pc++;
        return data;
    }

    word ReadNextWord() {
        byte hi = memory[pc];
        pc++;
        byte lo = memory[pc];
        pc++;

        word value = (hi << 8) | lo;
        return value;
    }

    void LoadProgramFromArray(byte *program, unsigned int size) {
        for (int i = 0; i < size; i++) {
            memory[0x0600 + i] = program[i];
        }
    }

    void Execute(unsigned int cycles) {
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

    void Execute() {
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
};

void LDASetStatus(CPU *cpu) {
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


int main(int argc, char* argv[]) {
    CPU cpu;

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
