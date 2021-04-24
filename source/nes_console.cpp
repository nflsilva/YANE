#include "nes_console.hpp"

nes_console::nes_console(cartridge* c) : _cartridge(c), _clock_cycles(0), instructionNeedsPrint(true) {
	
	_vram_pt = new ram(32);
	_vram_nt_0 = new ram(1 * 1024);
	_vram_nt_1 = new ram(1 * 1024);
	_ppu_bus = new ppu_bus(_cartridge, _vram_pt, _vram_nt_0, _vram_nt_1);
	_ppu = new u2c02(_ppu_bus); 
	
	_ram = new ram(2 * 1024);
	_cpu_bus = new cpu_bus(_cartridge, _ram, _ppu);
	_cpu = new r6502(_cpu_bus);
	
}

nes_console::~nes_console(){
}

r6502* nes_console::get_cpu(){
	return _cpu;
}

bool nes_console::clock(){
	
	
	

	if((_clock_cycles % 3) == 0){
		
		if(_cpu->clock()) {
			//_ppu->debug();
		}
			
		
	}

	_ppu->clock();
	
	if(_ppu->call_nmi()){
		_cpu->nmi();
	}
	

	_clock_cycles++;
	
	
	return true;
}





