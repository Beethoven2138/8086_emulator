#include <stdlib.h>
#include <stdio.h>

#include <cpu.h>

CPU *cpu;

int main(int argc, char **argv)
{
	cpu = (CPU*)malloc(sizeof(CPU));

	init_cpu(cpu, argv[1]);

	free(cpu->registers);
	free(cpu);

	return 0;
}
