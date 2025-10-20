#include "PSX.h"
#include "CPU.h"
#include <cstdint>

PSX::PSX() : cpu(&memory)
{

}

void PSX::tick()
{
	int cycles = cpu.step();
}