#include "CPU.h"
#include <cstdint>
CPU::CPU(Memory* memory) : memory(memory)
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


int CPU::step()
{
	return 0;
}