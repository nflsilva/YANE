#include "include/mapper000.h"


mapper000::mapper000(ui8_t n_prg_banks) : n_prg_banks(n_prg_banks){
}

mapper000::~mapper000(){
}

ui16_t mapper000::map_prg_address(ui16_t address){
	ui16_t address_mask = 0x7FFF;
	if(n_prg_banks == 0x01) {
		address_mask = 0x3FFF;
	}
	return address & address_mask;
}

ui16_t mapper000::map_chr_address(ui16_t address){
	return address;
}

