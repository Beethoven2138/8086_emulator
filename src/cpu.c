#include <cpu.h>

#include <stdio.h>
#include <stdlib.h>

void init_cpu(CPU *cpu, const char *rom)
{
	FILE *rom_file = fopen(rom, "r");

	if (rom_file == NULL)
		exit(1);

	else
	{
		int c;
		int i = 0;

		while (1)
		{
			c = fgetc(rom_file);
			if (feof(rom_file))
				break;
			cpu->ram[i] = (unsigned char)c;
			i++;
		}
	}

	cpu->registers = (REGS*)malloc(sizeof(REGS));

	cpu->registers->SP = 0x100;
	cpu->registers->BP = 0x100;

	fill_opcodes48();
}

void cycle(void)
{
	char opcode = cpu->opcodes[0];
	lower_opcodes(1);
	if (cpu->opcode_index <= 3)
		fill_opcodes16();

	//ADD Byte + Byte
	if (opcode == 0)
	{
		
	}
}


static void fill_opcodes16(void)
{
	cpu->opcodes[++cpu->opcode_index] = cpu->ram[CS(cpu) * 16 + IP(cpu) + cpu->opcode_index];
	cpu->opcodes[++cpu->opcode_index] = cpu->ram[CS(cpu) * 16 + IP(cpu) + cpu->opcode_index];
}

static void fill_opcodes48(void)
{
	for (int i = 0; i < 6; i++)
	{
		cpu->opcodes[i] = cpu->ram[CS(cpu) * 16 + IP(cpu) + i];
	}
	cpu->opcode_index = 5;
}

static void lower_opcodes(int a)
{
	for (int i = 0; i + a < 6; i++)
	{
		cpu->opcodes[i] = cpu->opcodes[i+a];
	}
	for (int i = 5; 5 - i >= a; i--)
		cpu->opcodes[i] = 0;
	cpu->opcode_index -= a;
}
