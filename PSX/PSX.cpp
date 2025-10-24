#include "PSX.h"
#include "CPU.h"
#include <cstdint>

PSX::PSX() : cpu(&memory)
{
	// i can load things to memory here in order to debug certain problems

	memory.write32(0, 0x27bdffe0);
	memory.write32(4, 0x27bdffe0);
}

void PSX::tick()
{
	int cycles = cpu.step();
}