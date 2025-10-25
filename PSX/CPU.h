#pragma once
#include "Memory.h"
#include <cstdint>

class CPU
{

	public:

	enum class Exception
		{
			Interrupt = 0,
			AddressErrorLoad = 4, // for loadWord errors (word not divisible by 4)
			AddressErrorStore = 5, // same but for saveWord
			BusErrorInstruction = 6,
			BusErrorData = 7,
			Syscall = 8,
			Breakpoint = 9,
			ReservedInstruction = 10,
			CoprocessorUnusable = 11,
			Overflow = 12
		};


	Memory* memory;

	CPU(Memory* memory);
	void reset();
	int step();

	int cycles;


	bool interrupts_enabled; // helper bool to check if interrupts are enabled

	bool jump_just_executed = false;
	bool in_branch_delay = false; // helper bool for knowing if were in the op after the branch op / before branch takes place


	//CPU REGS

	uint32_t reg[32];
	uint32_t pc;
	uint32_t nextPc; // this is for pipeline branching
	uint32_t hi;
	uint32_t lo;

	//COP0 REGS

	struct COP0
	{
		uint32_t reg[32] = {}; 

		uint32_t& Status = reg[12];
		uint32_t& Cause = reg[13];
		uint32_t& EPC = reg[14];
		uint32_t& PRId = reg[15];

		uint32_t& BadVAddr = reg[8];
		uint32_t& Count = reg[9];
		uint32_t& Compare = reg[11];

		//ps1 only
		uint32_t& BPC = reg[3];
		uint32_t& BDA = reg[5];
		uint32_t& DCIC = reg[7];
		uint32_t& JumpDest = reg[10];

	};

	COP0 cop0;

	void incrementCOP0Count();

	//helpers for load delay

	uint32_t load_delay_reg; // remembers which one
	uint32_t load_delay_val; // remembers val

	//instructions

	void decode(uint32_t opcode); // decode / execute
	void execute_r_type(uint32_t instr);
	void execute_j_type(uint32_t instr, uint8_t opcode);
	void execute_i_type(uint32_t instr, uint8_t opcode);

	//valid word checker
	inline bool validWord(uint32_t word,  Exception exc);
	inline bool validHalfWord(uint16_t hword , Exception exc);

	void trigger_exception(Exception exc, uint32_t badAddr);

};

