#include "CPU.h"
#include <cstdint>
#include <iostream>
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

void trigger_exception(CPU::Exception exc)
{
	std::cout<<"EXCEPTION, ";
}

void CPU::execute_r_type(uint32_t instr)
{
	//000000 01001 01010 01000 00000 100000
	//opcode   rs    rt    rd   shamt funct

	uint8_t rs =    (instr >> 21) & 0b11111; // source
	uint8_t rt =    (instr >> 16) & 0b11111; // target
	uint8_t rd =    (instr >> 11) & 0b11111; // dest
	uint8_t shamt = (instr >> 6 ) & 0b11111;
	uint8_t funct = (instr      ) & 0b111111;

	switch (funct)
	{
	case(0x00): break;//SSL
	case(0x02): break;//SRL
	case(0x03): break;//SRA
	case(0x04): break;//SLLV
	case(0x06): break;//SRLV
	case(0x07): break;//SRAV
	case(0x08): break;//JR
	case(0x09): break;//JALR
	case(0x0C): trigger_exception(Exception::Syscall); break;//SYSCALL
	case(0x0D): trigger_exception(Exception::Breakpoint);break;//BREAK

	case(0x10): reg[rd] = hi; break;//MFHI
	case(0x11): hi = reg[rs];//MTHI TODO CHECK
	case(0x12):  reg[rd] = lo; break;//MFLO
	case(0x13): hi = reg[rt];//MTLO TODO CHECK

	case(0x18): int64_t res = (int64_t)(int32_t)reg[rs] * (int64_t)(int32_t)reg[rt]; lo = (uint32_t)(res & 0xFFFFFFFF); hi = (uint32_t)((res >> 0xFF) & 0xFFFFFFFF); break;//MULT
	case(0x19): uint64_t res = (uint64_t)reg[rs] * (uint64_t)reg[rt]; lo = (uint32_t)(res & 0xFFFFFFFF); hi = (uint32_t)((res >> 32) & 0xFFFFFFFF); break;//MULTU

	case(0x1A): // DIV
	if (reg[rt] == 0)break;
	else if (reg[rs] == INT32_MIN && reg[rt] == -1) { lo = (uint32_t)INT32_MIN; hi = 0; }
	else lo = (uint32_t)(reg[rs] / reg[rt]);hi = (uint32_t)(reg[rs] % reg[rt]);break;

	case(0x1B): if( reg[rt] != 0 ) lo = reg[rs] / reg[rt]; hi = reg[rs]%reg[rt] ;break;//DIVU

	case(0x20): if ((int64_t)reg[rs] + (int64_t)reg[rt] > INT32_MAX) { trigger_exception(Exception::Overflow); }else { reg[rd] = reg[rs] + reg[rt]; } break; //ADD
	case(0x21): reg[rd] = reg[rs] + reg[rt]; break;//ADDU

	case(0x22): if ((int64_t)reg[rs] - (int64_t)reg[rt] < INT32_MIN) { trigger_exception(Exception::Overflow); }else { reg[rd] = reg[rs] - reg[rt]; } break; //SUB
	case(0x23): reg[rd] = reg[rs] - reg[rt];//SUBU

	case(0x24): break;//AND
	case(0x25): break;//OR
	case(0x26): break;//XOR
	case(0x27): break;//NOR
	case(0x2A): break;//SLT
	case(0x2B): break;//SLTU

	default:
	printf("SPECIAL OXOOh OPCODE FAIL: did not implement function 0x%02X at PC 0x%08X\n", funct, pc); break;
	}


}

void CPU::execute_i_type(uint32_t instr , uint8_t opcode)
{
	//001000 01001 01000 0000000001100100
	//opcode   rs    rt        immediate
	uint8_t rs = (instr >> 21) & 0b11111;
	uint8_t rt = (instr >> 16) & 0b11111;
	uint8_t immed = (instr)    & 0b1111111111111111;

	switch (opcode)
	{
	case(0x01): break; //BcondZ  
	case(0x04): break; //BEQ     
	case(0x05): break; //BNE     
	case(0x06): break; //BLEZ    
	case(0x07): break; //BGTZ    
		
	case(0x08)://ADDI
		uint32_t immedRead = memory->read32(immed);
		if ((int64_t)reg[rs] + (int64_t)immedRead > INT32_MAX) { trigger_exception(Exception::Overflow); }
		else { reg[rt] = reg[rs] + immedRead; } break;
	case(0x09): reg[rt] = reg[rs] + memory->read32(immed); break;  //ADDIU

	case(0x0A): break; //SLTI  
	case(0x0B): break; //SLTIU 
	case(0x0C): break; //ANDI  
	case(0x0D): break; //ORI   
	case(0x0E): break; //XORI  
	case(0x0F): break; //LUI   

	case(0x10): break; //COP0 
	case(0x11): break; //COP1 
	case(0x12): break; //COP2 
	case(0x13): break; //COP3 
	
	//THE LOAD INSTRUCTIONS (REGISTER DELAY)
	case(0x20): break; //LB   
	case(0x21): break; //LH
	case(0x22): break; //LWL
	case(0x23): break; //LW
	case(0x24): break; //LBU
	case(0x25): break; //LHU
	case(0x26): break; //LWR

	case(0x28): break; //SB
	case(0x29): break; //SH
	case(0x2A): break; //SWL
	case(0x2B): break; //SW
	case(0x2E): break; //SWR

	case(0x30): break; //LWC0
	case(0x31): break; //LWC1
	case(0x32): break; //LWC2
	case(0x33): break; //LWC3
	
	case(0x38): break; //SWC0
	case(0x39): break; //SWC1
	case(0x3A): break; //SWC2
	case(0x3B): break; //SWC3

	default:
	printf("Reserved Instruction: opcode 0x%02X at PC 0x%08X\n", opcode, pc);
	trigger_exception(Exception::ReservedInstruction);
	break;
	}

}


void CPU::execute_j_type(uint32_t instr, uint8_t opcode)
{
	//001000 01001010000000000001100100
	//opcode       immediate

	uint8_t immed = (instr) & 0b11111111111111111111111111;

	// this can only be J or JAL

	//000010 01001010000000000001100100 0x02 J
	//000011 01001010000000000001100100 0x03 JAL

	
	if (opcode & 0x1) // checks odd bit (JAL)
	{

	}
	else // J type
	{

	}

}


inline void CPU::decode(uint32_t instr) // decide what type the opcode is
{
	uint8_t opcode = (instr >> 26) & 0x3F;
	if(opcode == 0x00) execute_r_type(instr);
	else if (opcode == 0x02 || opcode == 0x03) execute_j_type(instr, opcode);
	else execute_i_type(instr, opcode);
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