#pragma once
#include <cstdint>
#include <memory>
class Memory
{

	std::unique_ptr<uint8_t[]> ram;  
	std::unique_ptr<uint8_t[]> bios;
	std::unique_ptr<uint8_t[]> cache;


public:

	Memory();

	uint32_t read32(uint32_t addr);
	uint32_t write32(uint32_t addr, uint32_t val);
	bool load_bios(const char* file); // bool to check if bios loads

};

