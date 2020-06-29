#pragma once

#include "yane.hpp"

#include "cartridge.hpp"
#include "ram.hpp"

class ppu_bus
{
private:
	
	cartridge* _cartridge;
	ram* _vram;
	
public:
	ppu_bus(cartridge* c, ram* r);
	~ppu_bus();

	void write(ui16_t address, ui8_t byte);
	ui8_t read(ui16_t address);

};

