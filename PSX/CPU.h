#pragma once
#include "Memory.h"
#include <cstdint>
class CPU
{
	public:
		Memory* memory;
		CPU(Memory* memory);
		int step();

		int cycles;
};

