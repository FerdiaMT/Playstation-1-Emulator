#include "PSX.h"
#include "CPU.h"
#include <cstdint>
#include <iostream>


void memoryDebug(Memory* memory)
{
	std::cout << "===========================" << std::endl;
	std::cout << " MEMR WRITES FOR DEBUGGING" << std::endl;
	std::cout << "===========================" << std::endl << std::endl;

	memory->write32(0xBFC00000, 0x20080005); // ADDI $t0, $zero, 5
	memory->write32(0xBFC00004, 0x2009000A); // ADDI $t1, $zero, 10
	memory->write32(0xBFC00008, 0x01095020); // ADD $t2, $t0, $t1
	memory->write32(0xBFC0000C, 0xAD0A1000); // SW $t2, 0x1000($zero)
	memory->write32(0xBFC00010, 0x08000010); // J to self (loop)

	std::cout << "===========================" << std::endl;
	std::cout << "ORIGINAL MEMORY WRITES DONE" << std::endl;
	std::cout << "===========================" << std::endl << std::endl;

}

PSX::PSX() : cpu(&memory)
{

	//memory.load_bios("CPUTest/CPU/ADD/CPUADD.bin");
	memory.load_bios("SCPH1001.bin");


	// i can load things to memory here in order to debug certain problems
	//memoryDebug(&memory);

	cpu.reset();

}

void PSX::tick()
{
	int cycles = cpu.step();
}

