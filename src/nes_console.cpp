#include "nes_console.hpp"

nes_console::nes_console(cartridge* c) : _cartridge(c)
{
	
	_vram = new ram(2 * 1024);
	_ppu_bus = new ppu_bus(_cartridge, _vram);
	_ppu = new u2c02(_ppu_bus); 
	
	_ram = new ram(2 * 1024);
	_cpu_bus = new cpu_bus(_cartridge, _ram, _ppu);
	_cpu = new r6502(_cpu_bus);
	
}

nes_console::~nes_console()
{
}

r6502* nes_console::get_cpu(){
	return _cpu;
}

bool nes_console::clock(){
	
	return _cpu->clock();
	
	
}


