#include "CPU.h"
#include <cstdint>
#include <iostream>
CPU::CPU(Memory* memory) : memory(memory) ,cycles(1)
{

}

void CPU::reset()
{
	memset(reg, 0 , 32);
	hi = 0;
	lo = 0;
	pc = 0xBFC00000;
	nextPc = pc + 4;
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
	std::cout << "EXCEPTION, ";
}

inline bool CPU::validWord(uint32_t word , Exception exc) // divisable by 4
{
	if (word & 0b00)
	{
		return true;
	}

	trigger_exception(exc);
}

inline bool CPU::validHalfWord(uint32_t word , Exception exc) // divisable by 2 
{
	if (word & 0b0)
	{
		return true;
	}

	trigger_exception(exc);
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

	int64_t res{};
	uint64_t bres{};
	switch (funct)
	{ //homestuck 
	case(0x00): reg[rd] = reg[rt] << shamt;break;//SLL UNSIGNED INTS USE LOGICAL SHIFT
	case(0x02): reg[rd] = reg[rt] >> shamt;break;//SRL
	case(0x03): reg[rd] = (uint32_t)((int32_t)reg[rt] >> shamt);break;//SRA  SIGNED INTS USE ARITHMETIC SHIFT
	case(0x04): reg[rd] = reg[rt] << reg[rs];break;//SLLV
	case(0x06): reg[rd] = reg[rt] >> reg[rs];break;//SRLV
	case(0x07): reg[rd] = (uint32_t)((int32_t)reg[rt] >> reg[rs]);break;//SRAV
	case(0x08): break;//JR
	case(0x09): break;//JALR
	case(0x0C): trigger_exception(Exception::Syscall); break;//SYSCALL
	case(0x0D): trigger_exception(Exception::Breakpoint);break;//BREAK

	case(0x10): reg[rd] = hi; break;//MFHI
	case(0x11): hi = reg[rs];//MTHI TODO CHECK
	case(0x12):  reg[rd] = lo; break;//MFLO
	case(0x13): hi = reg[rt];//MTLO TODO CHECK

	case(0x18): 
		res = (int64_t)(int32_t)reg[rs] * (int64_t)(int32_t)reg[rt]; 
		lo = (uint32_t)(res & 0xFFFFFFFF); hi = (uint32_t)((res >> 0xFF) & 0xFFFFFFFF); break;//MULT
	case(0x19): 
		bres = (uint64_t)reg[rs] * (uint64_t)reg[rt]; 
		lo = (uint32_t)(bres & 0xFFFFFFFF); hi = (uint32_t)((bres >> 32) & 0xFFFFFFFF); break;//MULTU

	case(0x1A): // DIV
	if (reg[rt] == 0)break;
	else if (reg[rs] == INT32_MIN && reg[rt] == -1) { lo = (uint32_t)INT32_MIN; hi = 0; }
	else lo = (uint32_t)(reg[rs] / reg[rt]);hi = (uint32_t)(reg[rs] % reg[rt]);break;

	case(0x1B): if( reg[rt] != 0 ) lo = reg[rs] / reg[rt]; hi = reg[rs]%reg[rt] ;break;//DIVU

	case(0x20): if ((int64_t)reg[rs] + (int64_t)reg[rt] > INT32_MAX) { trigger_exception(Exception::Overflow); }else { reg[rd] = reg[rs] + reg[rt]; } break; //ADD
	case(0x21): reg[rd] = reg[rs] + reg[rt]; break;//ADDU

	case(0x22): if ((int64_t)reg[rs] - (int64_t)reg[rt] < INT32_MIN) { trigger_exception(Exception::Overflow); }else { reg[rd] = reg[rs] - reg[rt]; } break; //SUB
	case(0x23): reg[rd] = reg[rs] - reg[rt];//SUBU

	case(0x24): reg[rd] = reg[rs] & reg[rt]; break;//AND
	case(0x25): reg[rd] = reg[rs] | reg[rt]; break;//OR
	case(0x26): reg[rd] = reg[rs] ^ reg[rt];break;//XOR
	case(0x27): reg[rd] = ~(reg[rs] ^ reg[rt]);break;//NOR TODO, DOUBLE CHECK THIS
	case(0x2A): reg[rs] < reg[rt] ? reg[rd] = 1 : reg[rd] = 0; break;//SLT
	case(0x2B): (uint32_t)reg[rs] < (uint32_t)reg[rt] ? reg[rd] = 1 : reg[rd] = 0;break;//SLTU

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

	uint32_t addr{};

	//SOME WRITING ON BRANCHING
		// use relative adressing, 
		// if branch not taken, nextpc will just +=4 as per usual
		// if it is, we want it to +4 AND + (immed*4) , which is <<2
	//ALWAYS USE NEXTPC INSTEAD OF PC, AS THE NEXT INSTRUCTION MUST BE EXECUTED FIRST

	switch (opcode)
	{
	case(0x01): break; //BcondZ  
	case(0x04): if (rs == rt) nextPc += ((static_cast<int16_t>(immed)) << 2); break; //BEQ   
	case(0x05): if (rs != rt) nextPc += ((static_cast<int16_t>(immed)) << 2); break; //BNE     
	case(0x06): break; //BLEZ    
	case(0x07): break; //BGTZ    
		
	case(0x08)://ADDI

	{
		int32_t a = static_cast<int32_t>(reg[rs]);
		int32_t imm = static_cast<int16_t>(immed);
		int32_t res = a+imm;

		if (((a ^ imm) >= 0) && ((a ^ res) < 0))
		{
			trigger_exception(Exception::Overflow);
		}

		reg[rt] = static_cast<uint32_t>(res);
		break;
	}

	case(0x09): reg[rt] = static_cast<uint32_t>(static_cast<int32_t>(reg[rs]) + static_cast<int16_t>(immed)); break;  //ADDIU

	case(0x0A): reg[rs] < immed ? reg[rt] = 1 : reg[rt] = 0; break; //SLTI  
	case(0x0B): (int32_t)reg[rs] < (int32_t)immed ? reg[rt] = 1 : reg[rt] = 0; break; //SLTIU 
	case(0x0C): reg[rt] = reg[rs] & immed; break; //ANDI  
	case(0x0D): reg[rt] = reg[rs] | immed;break; //ORI   
	case(0x0E): reg[rt] = reg[rs] ^ immed;break; //XORI  
	case(0x0F): reg[rt] = (immed << 16) & 0xFF00;break; //LUI   

	case(0x10): break; //COP0 
	case(0x11): break; //COP1 
	case(0x12): break; //COP2 
	case(0x13): break; //COP3 

	
	//THE LOAD INSTRUCTIONS (REGISTER DELAY)
	case(0x20): break; //LB   
		addr = reg[rs] + immed;
		load_delay_reg = rt;
		load_delay_val = memory->read8(addr);break;
	case(0x21): break; //LH
		addr = reg[rs] + immed;
		if (validHalfWord(addr , Exception::AddressErrorLoad))
		{
			load_delay_reg = rt;
			load_delay_val = memory->read16(addr);
		};break;

	case(0x22): break; //LWL
	{
		addr = reg[rs] + (int16_t)immed;
		uint32_t value = memory->read32(addr & ~3); // read the 4 divisible 32 bit word that addr is in
		uint32_t shift = (addr & 3) * 8; // 
		uint32_t mask = 0xFFFFFFFF >> shift;

		load_delay_val = (reg[rt] & mask) | (value << (24 - shift * 8));
		load_delay_reg = rt;

		break;
	}

	case(0x23): //LW
	{
		addr = reg[rs] + immed;
		if (validWord(addr, Exception::AddressErrorLoad))
		{
			load_delay_reg = rt;
			load_delay_val = memory->read32(addr);
		};break;
	}
	case(0x24): break; //LBU
	case(0x25): break; //LHU
	case(0x26):
		{		//LWR
			addr = reg[rs] + immed;
			uint32_t value = memory->read32(addr & ~3);
			uint32_t shift = (addr & 3 * 8);
			uint32_t mask = 0xFFFFFFFF << ((3 - (addr & 3) * 8));
			load_delay_val = (reg[rt] & mask) | (value >> shift);
			load_delay_reg = rt;

			break;
		}
	case(0x28): break; //SB
	case(0x29): break; //SH
	case(0x2A): break; //SWL
	case(0x2B): memory->write32(static_cast<int16_t>(immed & 0xFFFF), reg[rt]);break; // SW
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

	pc = nextPc;
	nextPc += 4;

	decode(opcode); // this also executes it



	reg[0] = 0;

	return 1;
}