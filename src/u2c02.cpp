#include "u2c02.hpp"

u2c02::u2c02(ppu_bus* b) : 

_ppuaddr(0),
_ppudata(0),

_ppudata_buffer(0),

_color_index(0),
_current_cycle(0),
_current_scanline(0),
	
_coarse_x(0),
_coarse_y(0),
	
_pattern_low_bit_shifter(0),
_pattern_high_bit_shifter(0),



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

bool u2c02::is_visible(){
	return _is_visible;
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
	

	
	if(_current_cycle >= 0 && _current_cycle < 257 && _current_scanline >=0 && _current_scanline < 240){


		ui8_t cycle_phase = _current_cycle % 8;
		switch(cycle_phase){
			case 0:

				_coarse_x = _current_cycle / 8;
				
				ui8_t nametable_number = 
				((ui8_t)_ppuctrl.nametable_base_address_y) << 1 | 
				((ui8_t)_ppuctrl.nametable_base_address_x);
				
				ui16_t nametable_address = 0x2000 + nametable_number * 0x0400;

				ui8_t tile_id = _bus->read(nametable_address + _coarse_x + (_coarse_y * 32));
				ui8_t tile_attribute = _bus->read(nametable_address + 0x03C0 + _coarse_x / 4 + ((_coarse_y / 4) * 8));

				ui8_t attribute_x = (_coarse_x % 4) > 1 ? 0x1 : 0x0;
				ui8_t attribute_y = (_coarse_y % 4) > 1 ? 0x1 : 0x0;
				ui8_t attribute_xy = attribute_y << 1 | attribute_x;
				_palette_value = (tile_attribute >> (attribute_xy * 2)) & 0x03;
				
				ui16_t pattern_address = _ppuctrl.background_pattern_base_address ? 0x1000 : 0x0000;
				_pattern_low_bit_shifter = _bus->read(pattern_address + tile_id * 16 + _current_scanline % 8);
				_pattern_high_bit_shifter = _bus->read(pattern_address + tile_id * 16 + 8 + _current_scanline % 8);

				break;
		}

		ui8_t low_pattern = _pattern_low_bit_shifter & 0x80 ? 0x1 : 0x0;
		ui8_t high_pattern = _pattern_high_bit_shifter & 0x80 ? 0x1 : 0x0;
		ui8_t pixel_value = high_pattern << 1 | low_pattern;


		_color_index = _bus->read(0x3F00 + (_palette_value * 4) + pixel_value);

		_pattern_low_bit_shifter <<= 1;
		_pattern_high_bit_shifter <<= 1;
		_is_visible = true;

	}
	else {
		_is_visible = false;
	}
	
	
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
		_coarse_y = _current_scanline / 8;

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
