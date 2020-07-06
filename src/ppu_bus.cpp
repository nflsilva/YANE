#include "ppu_bus.hpp"

ppu_bus::ppu_bus(cartridge* c, ram* palette_table, ram* name_table_0, ram* name_table_1) : 
_cartridge(c), 
_vram_palette_table(palette_table),
_vram_name_table_0(name_table_0),
_vram_name_table_1(name_table_1){
	
	
	
	
}

ppu_bus::~ppu_bus(){
}


void ppu_bus::write(ui16_t address, ui8_t byte){
	
	if(address >= 0x0000 && address <= 0x1FFF){
		//Pattern Memory
		_cartridge->write(address, byte);
	}
	else if(address >= 0x2000 && address <= 0x2FFF) {
		
		//Name Table
		ram* target_name_table;
		if(_cartridge->get_header()->get_mapper1() & 0x01){
			// Vertical Mirror
			if((address >= 0x2000 && address < 0x2400) || (address >= 0x2800 && address < 0x2C00))
				target_name_table = _vram_name_table_0;
			else
				target_name_table = _vram_name_table_1;
		}
		else {
			//Horizontal Mirror
			if((address >= 0x2000 && address < 0x2800))
				target_name_table = _vram_name_table_0;
			else
				target_name_table = _vram_name_table_1;
		}
		target_name_table->write(address & 0x03FF, byte);
	}
	else if(address >= 0x3F00 && address <= 0x3FFF){
		//Palette Memory
		address &= 0x001F;
		if(address == 0x0010) address = 0x0000;
		else if(address == 0x0014) address = 0x0004;
		else if(address == 0x0018) address = 0x0008;
		else if(address == 0x001C) address = 0x000C;
		
		_vram_palette_table->write(address, byte);
	}
}
ui8_t ppu_bus::read(ui16_t address){
	
	if(address >= 0x0000 && address <= 0x1FFF){
		//Pattern Memory
		return _cartridge->read(address & 0x0FFF);
	}
	else if(address >= 0x2000 && address <= 0x2FFF) {
		//Name Table
		ram* target_name_table;
		if(_cartridge->get_header()->get_mapper1() & 0x01){
			// Vertical Mirror
			if((address >= 0x2000 && address < 0x2400) || (address >= 0x2800 && address < 0x2C00))
				target_name_table = _vram_name_table_0;
			else
				target_name_table = _vram_name_table_1;
		}
		else {
			//Horizontal Mirror
			if((address >= 0x2000 && address < 0x2800))
				target_name_table = _vram_name_table_0;
			else
				target_name_table = _vram_name_table_1;
			
		}
		return target_name_table->read(address & 0x03FF);
	}
	else if(address >= 0x3F00 && address <= 0x3FFF){
		//Palette Memory
		address &= 0x001F;
		if(address == 0x0010) address = 0x0000;
		else if(address == 0x0014) address = 0x0004;
		else if(address == 0x0018) address = 0x0008;
		else if(address == 0x001C) address = 0x000C;
		
		return _vram_palette_table->read(address);
	}
	
	return 0;
}

