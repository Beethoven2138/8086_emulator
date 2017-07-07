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

	SP(cpu) = 0x100;
	BP(cpu) = 0x100;

	fill_opcodes48();
}

void cycle(void)
{
	//Should be after executing the opcode to know how many bytes it is
	//lower_opcodes(1);
	if (cpu->opcode_index <= 3)
		fill_opcodes16();

	//ADD
	if (cpu->opcodes[0] & 0xFC == 0)
	{
		//if mod is 11
		if (cpu->opcodes[1] & 0xC0 == 0)
		{
			//If 16-bit operands
			if (cpu->opcodes[0] & 0x1 == 1)
			{
				//if d == 1: data moves from operand specified by R/M to operand specified by REG
				if (cpu->opcodes[0] & 0x2 == 2)
					cpu->registers[REG(cpu->opcodes[1])] += RM(cpu->opcodes[1]);
				else
					cpu->registers[RM(cpu->opcodes[1])] += REG(cpu->opcodes[1]);
			}
			else//8-bit operands
			{
				if (cpu->opcodes[0] & 0x2 == 2)
				{
					unsigned short tmp = cpu->registers[REG(cpu->opcodes[1])];
					if (REG(cpu->opcodes[1]) & 0x4 == 0)//Lower byte eg AL
					{
						if (RM(cpu->opcodes[1]) & 0x4 == 0)
							cpu->registers[REG(cpu->opcodes[1])] = (tmp & 0xFF00) | (((tmp & 0xFF) + (cpu->registers[RM(cpu->opcodes[1])] & 0xFF)) & 0xFF);
						else
							cpu->registers[REG(cpu->opcodes[1])] = (tmp & 0xFF00) | (((tmp & 0xFF) + ((cpu->registers[RM(cpu->opcodes[1])] & 0xFF00) >> 8)) & 0xFF);
					}
					else
					{ 
						if (RM(cpu->opcodes[1]) & 0x4 == 0)
							cpu->registers[REG(cpu->opcodes[1])] = (tmp & 0xFF) | ((((tmp & 0xFF00) >> 8) + (cpu->registers[RM(cpu->opcodes[1])] & 0xFF)) << 8);
						else
							cpu->registers[REG(cpu->opcodes[1])] = (tmp & 0xFF) | ((((tmp & 0xFF00) >> 8) + ((cpu->registers[RM(cpu->opcodes[1])] & 0xFF00) >> 8)) << 8);
					}
				}
				else
				{
					unsigned short tmp = cpu->registers[RM(cpu->opcodes[1])];
					if (RM(cpu->opcodes[1]) & 0x4 == 0)
					{
						if (REG(cpu->opcodes[1]) & 0x4 == 0)
							cpu->registers[RM(cpu->opcodes[1])] = (tmp & 0xFF00) | ((tmp & 0xFF) + (cpu->registers[REG(cpu->opcodes[1])] & 0xFF));
						else
							cpu->registers[RM(cpu->opcodes[1])] = (tmp & 0xFF00) | ((tmp & 0xFF) + ((cpu->registers[REG(cpu->opcodes[1])] & 0xFF00) >> 8));
					}
					else
					{
						if (REG(cpu->opcodes[1]) & 0x4 == 0)
							cpu->registers[RM(cpu->opcodes[1])] = (tmp & 0xFF) | ((((tmp & 0xFF00) >> 8) + (cpu->registers[REG(cpu->opcodes[1])] & 0xFF)) << 8);
						else
							cpu->registers[RM(cpu->opcodes[1])] = (tmp & 0xFF) | ((((tmp & 0xFF00) >> 8) + ((cpu->registers[REG(cpu->opcodes[1])] & 0xFF00) >> 8)) << 8);
					}
				}
			}
		}
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
