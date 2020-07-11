#include "u2c02.hpp"

u2c02::u2c02(ppu_bus* b) : 

_ppuaddr(0),
_ppudata(0),

_ppudata_buffer(0),

_address_latch(false), 
_should_nmi_cpu(false),
_completed_frame(false),
_is_visible(false),

_bus(b),

_current_r(0),
_current_c(0)

{
	
	_ppuctrl.data = 0;
	_ppumask.data = 0;
	_ppustatus.data = 0;
	
}

u2c02::~u2c02(){
}


bool u2c02::should_nmi(){
	return _should_nmi_cpu;
}

void u2c02::clock(){
	

	
	_nt_buffer[_current_r][_current_c] = _bus->read(0x2000 + _current_c + _current_r * 32);
	_current_c++;
	if(_current_c == 32){
		_current_c = 0;
		_current_r++;
		if(_current_r == 30){
			_current_r = 0;
			_completed_frame = true;
		}
	}
	
}

ui8_t u2c02::read(ui16_t address){

	address &= 0x3FFF;
	
	switch(address){
		case 2: {
			
			_ppustatus.vertical_blank = true;
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
			
			//_ppuaddr += (_ppuctrl.vram_address_increment ? 31 : 1);
			return _ppudata;
		}
	}

	return 0;
}
void u2c02::write(ui16_t address, ui8_t data){

	address &= 0x3FFF;
	
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

