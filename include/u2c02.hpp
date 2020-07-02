#pragma once

#include "ppu_bus.hpp"


class u2c02
{
private:


	ui8_t _ppuctrl;
	ui8_t _ppumask;
	ui8_t _ppustatus;
	ui8_t _oamaddr;
	ui8_t _oamdata;
	ui8_t _ppuscroll;
	ui16_t _ppuaddr;
	ui8_t _ppudata;
	ui8_t _oamdma;

	bool _address_latch;

	ppu_bus* _bus;


	

	
public:
	u2c02(ppu_bus* b);
	~u2c02();

	ui8_t read(ui16_t address);
	void write(ui16_t address, ui8_t _data);
	

};

