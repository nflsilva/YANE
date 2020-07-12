#include "u2c02.hpp"

u2c02::u2c02(ppu_bus* b) : 

_ppuaddr(0),
_ppudata(0),

_ppudata_buffer(0),

_current_cycle(0),
_current_scanline(0),

_address_latch(false), 
_call_nmi_cpu(false),
_completed_frame(false),
_is_visible(false),

_bus(b)

{
	
	_ppuctrl.data = 0;
	_ppumask.data = 0;
	_ppustatus.data = 0;
	
}

u2c02::~u2c02(){
}


bool u2c02::call_nmi(){
	bool d = _call_nmi_cpu;
	_call_nmi_cpu = false;
	return d;
}
bool u2c02::completed_frame(){
	bool d = _completed_frame;
	_completed_frame = false;
	return d;
}

void u2c02::clock(){
	

	
	if(_current_scanline == 241 && _current_cycle == 1){
		_ppustatus.vertical_blank = true;
		if(_ppuctrl.generate_nmi){
			_call_nmi_cpu = true;
		}
	}
	else if(_current_scanline == 261 && _current_cycle == 1){
		_ppustatus.vertical_blank = false;
	}
	
	
	_current_cycle++;
	if(_current_cycle == 341){
		
		_current_cycle = 0;
		_current_scanline++;

		if(_current_scanline == 261){
			_current_scanline = 0;
			_completed_frame = true;
		}
		
	}
	
	
}

ui8_t u2c02::read(ui16_t address){

	switch(address){
		case 2: {
			ui8_t data = _ppustatus.data;
		
			_ppustatus.vertical_blank = false;
			_address_latch = false;
			
			return data;
		}	
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
			
		case 7: {
			_ppudata = _ppudata_buffer;
			_ppudata_buffer = _bus->read(_ppuaddr);
			if(_ppuaddr >= 0x3F00){
				_ppudata = _ppudata_buffer;
			}
			_ppuaddr += (_ppuctrl.vram_address_increment ? 32 : 1);
			return _ppudata;
		}
	}

	return 0;
}
void u2c02::write(ui16_t address, ui8_t data){

	switch(address){
		case 0:
			_ppuctrl.data = data;
			break;
		case 1:
			_ppumask.data = data;
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			if(_address_latch){
				//Low byte
				_ppuaddr = (_ppuaddr & 0xFF00) | data;
				_address_latch = false;
			}
			else {
				//High byte
				_ppuaddr = (_ppuaddr & 0x00FF) | (data << 8);
				_address_latch = true;
			}
			break;
		case 7:
			_bus->write(_ppuaddr, data);
			_ppuaddr += (_ppuctrl.vram_address_increment ? 32 : 1);
			break;
	}
	
}



bool u2c02::is_rendering(){
	return _ppumask.render_background || _ppumask.render_sprites;
}
