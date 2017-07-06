#ifndef CPU_H
#define CPU_H

#define AX(cpu) (cpu->registers->AX)
#define BX(cpu) (cpu->registers->BX)
#define CX(cpu) (cpu->registers->CX)
#define DX(cpu) (cpu->registers->DX)
#define AL(cpu) (cpu->registers->AX & 0x00FF)
#define BL(cpu) (cpu->registers->BX & 0x00FF)
#define CL(cpu) (cpu->registers->CX & 0x00FF)
#define DL(cpu) (cpu->registers->DX & 0x00FF)
#define AH(cpu) ((cpu->registers->AX & 0xFF00) >> 8)
#define BH(cpu) ((cpu->registers->BX & 0xFF00) >> 8)
#define CH(cpu) ((cpu->registers->CX & 0xFF00) >> 8)
#define DH(cpu) ((cpu->registers->DX & 0xFF00) >> 8)
#define SI(cpu) (cpu->registers->SI)
#define DI(cpu) (cpu->registers->DI)
#define BP(cpu) (cpu->registers->BP)
#define SP(cpu) (cpu->registers->SP)
#define CS(cpu) (cpu->registers->CS)
#define DS(cpu) (cpu->registers->DS)
#define SS(cpu) (cpu->registers->SS)
#define ES(cpu) (cpu->registers->ES)
#define IP(cpu) (cpu->registers->IP)
#define EFLAGS(cpu) (cpu->registers->EFLAGS)

typedef struct REGS
{
	//General purpose
	//Acculumator (arithmetic and logic operations)
	unsigned short AX;
	//Memory pointer into DS
	unsigned short BX;
	//Counter register used in loops
	unsigned short CX;
	//Division and multiplication
	unsigned short DX;

	//Index registers for string ops
	unsigned short SI;
	unsigned short DI;

	//Stack base pointer
	unsigned short BP;
	//Stack pointer
	unsigned short SP;

	//Code segment
	unsigned short CS;
	//Data segment
	unsigned short DS;
	//Stack segment
	unsigned short SS;
	//Extra segment
	unsigned short ES;
	//Instruction pointer
	unsigned short IP;

	//Flags
	unsigned short EFLAGS;
} REGS;

typedef struct CPU
{
	REGS *registers;
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
