#pragma once

#include "ppu_bus.hpp"


class u2c02
{
public:


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
	
	ui16_t _current_cycle;
	ui16_t _current_scanline;
	bool _should_nmi_cpu;
	
	ui8_t _coarse_x;
	ui8_t _coarse_y;
	
	ui8_t _tile_id;
	ui8_t _tile_attribute;
	ui8_t _pattern_low_bit;
	ui8_t _pattern_high_bit;
	
	
	ui16_t _shifter_pattern_low;
	ui16_t _shifter_pattern_high;
	
	ui16_t _shifter_attribute_low;
	ui16_t _shifter_attribute_high;
	
	ui8_t _color_index;

	bool _completed_frame;

	ppu_bus* _bus;


	

	
public:
	u2c02(ppu_bus* b);
	~u2c02();
	
	
	void clock();
	bool should_nmi();

	ui8_t read(ui16_t address);
	void write(ui16_t address, ui8_t _data);
	

};

