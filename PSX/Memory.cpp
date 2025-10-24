#include "Memory.h"
#include <memory>
#include <cstdint>

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
    return 0;
}

uint16_t Memory::read16(uint32_t addr)
{
    return 0;
}

uint8_t Memory::read8(uint32_t addr)
{
    return 0;
}


uint32_t Memory::write32(uint32_t addr, uint32_t val)
{
    return 0;
}

uint32_t Memory::write16(uint32_t addr, uint32_t val)
{
    return 0;
}

uint32_t Memory::write8(uint32_t addr, uint32_t val)
{
    return 0;
}
