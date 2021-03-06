#pragma once

#include "yane.hpp"

#include "cartridge.hpp"
#include "u2c02.hpp"
#include "ram.hpp"

class cpu_bus
{
private:
	
	cartridge* _cartridge;
	u2c02* _ppu;
	ram* _ram;

public:
	cpu_bus(cartridge* c, ram* r, u2c02* p);
	~cpu_bus();

	void write(ui16_t address, ui8_t byte);
	ui8_t read(ui16_t address);

};

