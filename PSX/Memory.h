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
	void write32(uint32_t addr, uint32_t val);

	uint16_t read16(uint32_t addr);
	void write16(uint32_t addr, uint32_t val);

	uint8_t read8(uint32_t addr);
	void write8(uint32_t addr, uint32_t val);


	bool load_bios(const char* file); // bool to check if bios loads

};

