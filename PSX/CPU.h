#pragma once
#include "Memory.h"
#include <cstdint>

class CPU
{
	uint32_t reg[32];
	uint32_t pc;
	uint32_t nextPc; // this is for pipeline branching
	uint32_t hi;
	uint32_t lo;

	//helpers for load delay

	uint32_t load_delay_reg; // remembers which one
	uint32_t load_delay_val; // remembers val

	//instructions

	void decode(uint32_t opcode); // decode / execute
	void execute_r_type(uint32_t instr);
	void execute_j_type(uint32_t instr, uint8_t opcode);
	void execute_i_type(uint32_t instr, uint8_t opcode);



	public:

		Memory* memory;

		CPU(Memory* memory);
		void reset();
		int step();

		int cycles;

		enum class Exception
		{
			Interrupt = 0,
			AddressErrorLoad = 4,
			AddressErrorStore = 5,
			BusErrorInstruction = 6,
			BusErrorData = 7,
			Syscall = 8,
			Breakpoint = 9,
			ReservedInstruction = 10,
			CoprocessorUnusable = 11,
			Overflow = 12
		};
};

