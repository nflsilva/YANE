#include "u2c02.hpp"

u2c02::u2c02(ppu_bus* b) : _bus(b)
{
}

u2c02::~u2c02()
{
}


ui8_t u2c02::read(ui16_t address){

	switch(address){
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
	}

	return 0;
}
void u2c02::write(ui16_t address, ui8_t _data){

	switch(address){
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
	}
	
}

