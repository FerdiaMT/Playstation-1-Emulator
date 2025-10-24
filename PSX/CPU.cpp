#include "CPU.h"
#include <cstdint>
CPU::CPU(Memory* memory) : memory(memory) ,cycles(1)
{

}

void CPU::reset()
{
}

// So theres 3 types of cpu instructions

// R type - register to register ~ 3 registers of data
// example , r2 = r0+ r1

// I type- immmediate, involves memory ~ 2 registers of data + constant
// example , r2 = r0+ 100 (constant)
// example , r2 = memory[r1]
// example , if r1 == r2, jump (this is now called branching)

// J TYPE - Unconditional Jump, no registers, normally far distance ~ just adress needed
// PC = 0x400000000000
// save return, then jump

////// ONCE WE HAVE WHAT OPCODE

//R  splits it like this
//000000 01001 01010 01000 00000 100000
//opcode   rs    rt    rd   shamt funct

// I splits like this
//001000 01001 01000 0000000001100100
//opcode   rs    rt        immediate

// J splits like this
//001000 01001010000000000001100100
//opcode       adress

////////////////////////////////////////
//// BREAKING DOWN R TYPE FURTHER
////////////////////////////////////////

// funct - tells us what operation to perform, eg: ADD, AND, OR, SUB
// shamt - if shift instruction, tells us how much

// rs - register source
// rt - register target
// rd - register destination

// 32 registers numbered 0-31
// PC is seperate
// 2 special regs HI/LO , used to make a 64 bit reg, normally for MULT or DIV


/// / PIPELINING

// fetch, decode, execute, mem write/read(if needed) , writeBack(into reg)

//the second a fetch is done, next cycle its put into decode
//that cycle, a new fetch is also done, so its 1 fetch per cycle


void CPU::execute_r_type(uint32_t instr)
{
}

void CPU::execute_i_type(uint32_t instr)
{
}

void CPU::execute_j_type(uint32_t instr)
{
}


inline void CPU::decode(uint32_t instr) // decide what type the opcode is
{
	uint8_t opcode = (instr >> 26) & 0x3F;
	if(opcode == 0x00) execute_r_type(instr);
	else if (opcode == 0x02 || opcode == 0x03) execute_j_type(instr);
	else execute_i_type(instr);
}

int CPU::step()
{
	// PRE PIPELINE ~ we must apply the previous cycles reg vals
	// the ps1 reg vals can only get written into after the cycle
	if (load_delay_reg != 0)
	{
		reg[load_delay_reg] = load_delay_val;
		load_delay_reg = 0;
	}

	//fetch first instruction
	uint32_t opcode = memory->read32(pc);
	decode(opcode); // this also executes it

	pc = nextPc;
	nextPc += 4;

	reg[0] = 0;

	return 1;
}