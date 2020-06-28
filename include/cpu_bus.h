#pragma once
#include "yane.h"

#include "cartridge.h"
#include "ram.h"

class cpu_bus
{
private:
	
	cartridge* _cartridge;
	ram* _ram;

	
public:
	cpu_bus(cartridge* c, ram* r);
	~cpu_bus();

	void write(ui16_t address, ui8_t byte);
	ui8_t read(ui16_t address);

};

