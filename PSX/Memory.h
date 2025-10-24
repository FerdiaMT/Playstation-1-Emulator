#pragma once
#include <cstdint>
class Memory
{
	uint8_t ram[2 * 1024 * 1024];
	uint8_t bios[512 * 1024];
	uint8_t cache[1024];
public:
	uint32_t read32(uint32_t addr);
	uint32_t write32(uint32_t addr, uint32_t val);
	bool load_bios(const char* file); // bool to check if bios loads

};

