#pragma once

#include "yane.hpp"

#include "cartridge.hpp"
#include "ram.hpp"

class ppu_bus
{
private:
	
	cartridge* _cartridge;
	ram* _vram_palette_table;
	ram* _vram_name_table_0;
	ram* _vram_name_table_1;
	
public:
	ppu_bus(cartridge* c, ram* palette_table, ram* name_table_0, ram* name_table_1);
	~ppu_bus();

	void write(ui16_t address, ui8_t byte);
	ui8_t read(ui16_t address);

};

