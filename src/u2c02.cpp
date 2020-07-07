#include "u2c02.hpp"

u2c02::u2c02(ppu_bus* b) : 

_ppuctrl(0),
_ppustatus(0), 
_address_latch(false), 

_current_cycle(0),
_current_scanline(0),
_should_nmi_cpu(false),

_coarse_x(0),
_coarse_y(0),
	
_tile_id(0),
_tile_attribute(0),
_pattern_low_bit(0),
_pattern_high_bit(0),

_completed_frame(false),

_is_visible(false),

_bus(b) {
	
	
	/*
	for(int i = 0; i < 30 * 32; i++){
		_bus->write(0x2000 + i, i);
	}
	for(_coarse_y = 0; _coarse_y < 30; _coarse_y++){
		for(_coarse_x = 0;  _coarse_x < 32; _coarse_x++){	
			int test = _bus->read(0x2000 + (_coarse_y * 32) + _coarse_x);
			cout << test << " ";
		}
		cout << endl;
	}
	
	
	
	for(int i = 0; i < 64; i++){
		_bus->write(0x23C0 + i, i);
	}
	
	for(_coarse_y = 0; _coarse_y < 30; _coarse_y++){
		for(_coarse_x = 0;  _coarse_x < 32; _coarse_x++){	
			int test = _bus->read(0x23C0 + (_coarse_x >> 2) + ((_coarse_y >> 2) << 3));
			cout << test << " ";
		}
		cout << endl;
	}*/
	

	
}

u2c02::~u2c02(){
}


bool u2c02::should_nmi(){
	return _should_nmi_cpu;
}

void u2c02::clock(){
	
	_is_visible = false;
	
	// vertical blank and nmi
	_should_nmi_cpu = false;
	if(_current_cycle == 1){
		if(_current_scanline == 241) {
		_ppustatus |= 0x80;
			if(_ppuctrl & 0x80){
				_should_nmi_cpu = true;
			}
		}
		else if(_current_scanline == 261){
			_ppustatus &= ~0x80;
		}
	}
	
	// rendering limits
	_current_cycle++;
	if(_current_cycle == 341){
		_current_cycle = 0;
		_current_scanline++;
		_coarse_y = _current_scanline / 8;
		if(_current_scanline == 262){
			_current_scanline = 0;
			_coarse_y = 0;
			_completed_frame = true;
		}
	}
	
	
	
	//debug
	ui8_t r = _bus->read(0x2300);
	if(r == 0x20){
		//printf("Hello");
	}
	
	
	if(_current_cycle > -1 && _current_cycle < 256 && _current_scanline > -1 && _current_scanline < 240){
		
		ui8_t cycle_phase = _current_cycle % 8;
		switch(cycle_phase){
			case 0:
			
				_coarse_x = _current_cycle / 8;
				
				ui16_t tile_id = _bus->read(0x2400 + _coarse_x + (_coarse_y * 32));
				_tile_attribute = _bus->read(0x23C0 + _coarse_x / 4 + ((_coarse_y / 4) * 8));
				
				ui16_t pattern_table = (_ppuctrl & 0x10) == 0 ? 0x0000 : 0x1000;
				_pattern_low_bit = _bus->read(pattern_table + tile_id * 16 + _current_scanline % 8);
				_pattern_high_bit = _bus->read(pattern_table + tile_id * 16 + 8 + _current_scanline % 8);
				
				break;
		}
		
		ui8_t low_pattern = _pattern_low_bit & 0x80 ? 0x1 : 0x0;
		ui8_t high_pattern = _pattern_high_bit & 0x80 ? 0x1 : 0x0;
		ui8_t pixel_value = high_pattern << 1 | low_pattern;
		

		ui8_t palette_value = 0x00;
		
		
		_color_index = _bus->read(0x3F00 + (palette_value << 2) + pixel_value);
		
		_pattern_low_bit <<= 1;
		_pattern_high_bit <<= 1;
		
		_is_visible = true;

	}
	
	
}

ui8_t u2c02::read(ui16_t address){

	switch(address){
		case 0:
			break;
		case 1:
			break;
		case 2: {
			ui8_t data = _ppustatus & 0xE0;
			_ppustatus &= ~0x80;
			_address_latch = false;
			//return 0xE0;
			return data;
		}	
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7: {
			break;
		}
	}

	return 0;
}
void u2c02::write(ui16_t address, ui8_t data){

	switch(address){
		case 0:
			_ppuctrl = data;
			break;
		case 1:
			_ppumask = data;
			break;

		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			if(_address_latch){
				_ppuaddr = (data) | (_ppuaddr & 0xFF00);
				_address_latch = false;
			}
			else {
				_ppuaddr = (data << 8) | (_ppuaddr & 0x00FF);
				_address_latch = true;
			}
			break;
		case 7:
			_bus->write(_ppuaddr, data);
			_ppuaddr += (_ppuctrl & 0x4 ? 32 : 1);
			break;
	}
	
}

