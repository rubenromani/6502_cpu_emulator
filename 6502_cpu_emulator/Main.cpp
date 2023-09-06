#include "main_6502.h"

int main()
{
	m6502::Mem mem;
	m6502::CPU cpu;
	cpu.Reset(mem);
	return 0;
}

