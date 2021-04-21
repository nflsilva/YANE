#include "cpu_bus.hpp"

cpu_bus::cpu_bus(cartridge* c, ram* r, u2c02* p) : _cartridge(c), _ppu(p), _ram(r)  {
}

cpu_bus::~cpu_bus(){
}


void cpu_bus::write(ui16_t address, ui8_t byte){
	
	if(address >= 0x0000 && address <= 0x1FFF){
		_ram->write(address & 0x1FFF, byte);
	}
	else if(address >= 0x2000 && address <= 0x3FFF) {
		_ppu->write(address & 0x0007, byte);
	}
	else if(address >= 0x4020 && address <= 0xFFFF){
		_cartridge->write(address, byte);
	}
	
}
ui8_t cpu_bus::read(ui16_t address){
	
	if(address >= 0x0000 && address <= 0x1FFF){
		return _ram->read(address & 0x1FFF);
	}
	else if(address >= 0x2000 && address <= 0x3FFF) {
		return _ppu->read(address & 0x0007);
	}
	else if(address >= 0x4020 && address <= 0xFFFF){
		return _cartridge->read(address);
	}
	
	return 0;
}


