#ifndef CPU_H
#define CPU_H

#define AX(cpu) (cpu->registers[0])
#define BX(cpu) (cpu->registers[3])
#define CX(cpu) (cpu->registers->[1])
#define DX(cpu) (cpu->registers->[2])
#define SI(cpu) (cpu->registers[6])
#define DI(cpu) (cpu->registers[7])
#define BP(cpu) (cpu->registers[5])
#define SP(cpu) (cpu->registers[4])
#define CS(cpu) (cpu->CS)
#define DS(cpu) (cpu->DS)
#define SS(cpu) (cpu->SS)
#define ES(cpu) (cpu->ES)
#define IP(cpu) (cpu->IP)
#define EFLAGS(cpu) (cpu->EFLAGS)

#define REG(a) ((a & 0x1C) >> 3)
#define RM(a) (a & 0x7)

typedef struct CPU
{
	unsigned short registers[8];

	unsigned short CS;
	unsigned short DS;
	unsigned short SS;
	unsigned short ES;
	unsigned short IP;
	unsigned short EFLAGS;

	//It's pipelined
	unsigned char opcodes[6];
	unsigned char opcode_index;
	//1 Mb of RAM
	unsigned char ram[0x100000];
} CPU;

extern CPU *cpu;

void init_cpu(CPU *cpu, const char *rom);

void cycle(void);

static void fill_opcodes16(void);
static void fill_opcodes48(void);

static void lower_opcodes(int a);

#endif
