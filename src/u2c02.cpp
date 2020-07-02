#include "u2c02.hpp"

u2c02::u2c02(ppu_bus* b) : _ppustatus(0xE0), _address_latch(false), _bus(b) {
}

u2c02::~u2c02(){
}


ui8_t u2c02::read(ui16_t address){

	switch(address){
		case 0:
			break;
		case 1:
			break;
		case 2: {
			
			/*ui8_t data = _ppustatus & 0xE0;
			_ppustatus &= 0x60;
			_address_latch = false;
			return data;*/
			return 0xE0;
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
			break;
		case 1:
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
			_ppuaddr++;
			break;
	}
	
}

