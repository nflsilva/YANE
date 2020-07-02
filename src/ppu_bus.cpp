#include "ppu_bus.hpp"

ppu_bus::ppu_bus(cartridge* c, ram* r) : _cartridge(c), _vram(r){
	
	
	
	
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
		//_vram->write(address, byte);
	}
	else if(address >= 0x3F00 && address <= 0x3FFF){
		//Palette Memory
		address &= 0x001F;
		if(address == 0x0010) address = 0x0000;
		else if(address == 0x0014) address = 0x0004;
		else if(address == 0x0018) address = 0x0008;
		else if(address == 0x001C) address = 0x000C;
		
		_vram->write(address, byte);
	}
}
ui8_t ppu_bus::read(ui16_t address){
	
	if(address >= 0x0000 && address <= 0x1FFF){
		//Pattern Memory
		return _cartridge->read(address);
	}
	else if(address >= 0x2000 && address <= 0x2FFF) {
		//Name Table
		//return _vram->read(address);
	}
	else if(address >= 0x3F00 && address <= 0x3FFF){
		//Palette Memory
		address &= 0x001F;
		if(address == 0x0010) address = 0x0000;
		else if(address == 0x0014) address = 0x0004;
		else if(address == 0x0018) address = 0x0008;
		else if(address == 0x001C) address = 0x000C;
		
		return _vram->read(address);
	}
	
	return 0;
}

