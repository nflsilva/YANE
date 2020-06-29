#pragma once

#include "ppu_bus.hpp"


class u2c02
{
private:


	ui8_t ppuctrl;
	ui8_t ppumask;
	ui8_t ppustatus;
	ui8_t oamaddr;
	ui8_t oamdata;
	ui8_t ppuscroll;
	ui8_t ppuaddr;
	ui8_t ppudata;
	ui8_t oamdma;


	ppu_bus* _bus;


	

	
public:
	u2c02(ppu_bus* b);
	~u2c02();

	ui8_t read(ui16_t address);
	void write(ui16_t address, ui8_t _data);

};

