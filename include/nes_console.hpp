#pragma once

#include "cartridge.hpp"
#include "ram.hpp"

#include "cpu_bus.hpp"
#include "ppu_bus.hpp"

#include "r6502.hpp"
#include "u2c02.hpp"

class nes_console
{
private:

	cartridge* _cartridge;

	r6502* _cpu;
	cpu_bus* _cpu_bus;
	ram* _ram;
	
	u2c02* _ppu;
	ppu_bus* _ppu_bus;
	ram* _vram;
	
	
public:
	nes_console(cartridge* c);
	~nes_console();
	
	r6502* get_cpu();
	
	bool clock();
	void reset();

};

