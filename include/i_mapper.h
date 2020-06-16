#pragma once
#include "yane.h"


class i_mapper
{
public:
	virtual ui16_t map_prg_address(ui16_t address) = 0;
	virtual ui16_t map_chr_address(ui16_t address) = 0;
};