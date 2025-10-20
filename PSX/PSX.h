#pragma once
#include <cstdint>
#include "Memory.h"
#include "CPU.h"
class PSX
{
public:
	Memory memory;
	CPU cpu;
	PSX();
	void tick();
};

