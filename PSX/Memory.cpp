#include "Memory.h"
#include <memory>
#include <cstdint>
#include <iostream>
Memory::Memory()
{
    ram = std::make_unique<uint8_t[]>(2 * 1024 * 1024);
    bios = std::make_unique<uint8_t[]>(512 * 1024);
    cache = std::make_unique<uint8_t[]>(1024);

    memset(ram.get(), 0, 2 * 1024 * 1024);
    memset(bios.get(), 0, 512 * 1024);
    memset(cache.get(), 0, 1024);
}

uint32_t Memory::read32(uint32_t addr)
{
    if (addr >= 0x000000 && addr <= 0x1FFFFF)
    {
        return ram[addr] | (ram[addr + 1] << 8) | (ram[addr + 2] << 16) | (ram[addr + 3] << 24);
    }
    else if (addr >= 0xBFC00000 && addr <= 0xBFC7FFFF)
    {
        uint32_t offset = addr - 0xBFC00000;
        return bios[offset] | (bios[offset + 1] << 8) | (bios[offset + 2] << 16) | (bios[offset + 3] << 24);
    }
}

uint16_t Memory::read16(uint32_t addr)
{
    if (addr >= 0x000000 && addr <= 0x1FFFFF)
    {
        return ram[addr] | (ram[addr + 1] << 8);
    }
    else if (addr >= 0xBFC00000 && addr <= 0xBFC7FFFF)
    {
        uint32_t offset = addr - 0xBFC00000;
        return bios[offset] | (bios[offset + 1] << 8);
    }
}

uint8_t Memory::read8(uint32_t addr)
{
    if (addr >= 0x000000 && addr <= 0x1FFFFF)
    {
        return ram[addr];
    }
    else if (addr >= 0xBFC00000 && addr <= 0xBFC7FFFF)
    {
        uint32_t offset = addr - 0xBFC00000;
        return bios[offset];
    }
}


void Memory::write32(uint32_t addr, uint32_t val)
{
    
    if (addr >= 0x000000 && addr <= 0x1FFFFF)
    {
        std::cout << "write to RAM " << ram << "with val " << std::hex << val <<" | "<<std::dec << val << std::endl;
        ram[addr] = val & 0xFF;
        ram[addr + 1] = (val >> 8) & 0xFF;
        ram[addr + 2] = (val >> 16) & 0xFF;
        ram[addr + 3] = (val >> 24) & 0xFF;
    }
    else if (addr >= 0xBFC00000 && addr <= 0xBFC7FFFF)
    {
        
        uint32_t offset = addr - 0xBFC00000;
        std::cout << "write to " << offset << "with val " <<std::hex<< val << std::endl;
        bios[offset] = val & 0xFF;
        bios[offset + 1] = (val >> 8) & 0xFF;
        bios[offset + 2] = (val >> 16) & 0xFF;
        bios[offset + 3] = (val >> 24) & 0xFF;
    }
}


void Memory::write16(uint32_t addr, uint32_t val)
{
    if (addr >= 0x000000 && addr <= 0x1FFFFF)
    {
        std::cout << "write to RAM " << ram << "with val " << std::hex << val << " | " << std::dec << val << std::endl;
        ram[addr] = val & 0xFF;
        ram[addr + 1] = (val >> 8) & 0xFF;
    }
    else if (addr >= 0xBFC00000 && addr <= 0xBFC7FFFF)
    {
        uint32_t offset = addr - 0xBFC00000;
        std::cout << "write to " << offset << "with val " << std::hex << val << std::endl;
        bios[offset] = val & 0xFF;
        bios[offset + 1] = (val >> 8) & 0xFF;
    }
}

void Memory::write8(uint32_t addr, uint32_t val)
{
    if (addr >= 0x000000 && addr <= 0x1FFFFF)
    {
        std::cout << "write to RAM " << ram << "with val " << std::hex << val << " | " << std::dec << val << std::endl;
        ram[addr] = val & 0xFF;
    }
    else if (addr >= 0xBFC00000 && addr <= 0xBFC7FFFF)
    {
        uint32_t offset = addr - 0xBFC00000;
        std::cout << "write to " << offset << "with val " << std::hex << val << std::endl;
        bios[offset] = val & 0xFF;
    }
}
