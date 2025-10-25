#define _CRT_SECURE_NO_WARNINGS

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
    uint32_t masked_addr = addr & 0x1FFFFFFF;

    if (masked_addr < 0x00200000)
    {
        uint32_t offset = masked_addr & 0x001FFFFF;
        return *(uint32_t*)&ram[offset];
    }

    else if (masked_addr >= 0x1FC00000 && masked_addr < 0x1FC80000)
    {
        uint32_t offset = masked_addr - 0x1FC00000;
        return *(uint32_t*)&bios[offset];
    }
    else if (masked_addr >= 0x1F800000 && masked_addr < 0x1F800400)
    {
        uint32_t offset = masked_addr - 0x1F800000;
        return *(uint32_t*)&cache[offset];
    }

    else
    {
        printf("Unhandled read32 at 0x%08X\n", addr);
        return 0xFFFFFFFF;
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
    uint32_t masked_addr = addr & 0x1FFFFFFF;

    if (masked_addr < 0x00200000)
    {
        uint32_t offset = masked_addr & 0x001FFFFF;
        *(uint32_t*)&ram[offset] = val;
        return;
    }

    else if (masked_addr >= 0x1FC00000 && masked_addr < 0x1FC80000)
    {
        printf("Attempted write to BIOS ROM at 0x%08X\n", addr);
        return;
    }

    else if (masked_addr >= 0x1F800000 && masked_addr < 0x1F800400)
    {
        uint32_t offset = masked_addr - 0x1F800000;
        *(uint32_t*)&cache[offset] = val;
        return;
    }
    else
    {
        printf("Unhandled write32 at 0x%08X = 0x%08X\n", addr, val);
    }
}

void Memory::write16(uint32_t addr, uint32_t val)
{
    if (addr >= 0x000000 && addr <= 0x1FFFFF)
    {
        ram[addr] = val & 0xFF;
        ram[addr + 1] = (val >> 8) & 0xFF;
    }
    else if (addr >= 0xBFC00000 && addr <= 0xBFC7FFFF)
    {
        uint32_t offset = addr - 0xBFC00000;
        bios[offset] = val & 0xFF;
        bios[offset + 1] = (val >> 8) & 0xFF;
    }
}

void Memory::write8(uint32_t addr, uint32_t val)
{
    if (addr >= 0x000000 && addr <= 0x1FFFFF)
    {
        ram[addr] = val & 0xFF;
    }
    else if (addr >= 0xBFC00000 && addr <= 0xBFC7FFFF)
    {
        uint32_t offset = addr - 0xBFC00000;
        bios[offset] = val & 0xFF;
    }
}

void Memory::load_bios(const char* file)
{

    FILE* fp = fopen(file, "rb");
    if (!fp)
    {
        fprintf(stderr, "Failed to open BIOS file: %s\n", file);
        return;
    }


    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);


    if (size != 512 * 1024)
    {
        fprintf(stderr, "Warning: BIOS size is %zu bytes, expected 524288\n", size);
    }
    fread(&bios[0], 1, size, fp);

    fclose(fp);

    printf("Loaded BIOS: %s (%zu bytes)\n", file, size);

}