#pragma once
#include "i_mapper.h"

class mapper000 : public i_mapper
{
private:
	ui8_t n_prg_banks;
	
public:
	mapper000(ui8_t n_prg_banks);
	~mapper000();
	
	ui16_t map_prg_address(ui16_t address);
	ui16_t map_chr_address(ui16_t address);
};

