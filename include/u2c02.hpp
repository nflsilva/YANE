#pragma once

#include "ppu_bus.hpp"





class u2c02
{
private:

	//$2000 PPU CTRL
	union {
		struct {
			bool nametable_base_address_x : 1;
			bool nametable_base_address_y : 1;
			bool vram_address_increment : 1;
			bool sprite_pattern_base_address : 1;
			bool background_pattern_base_address : 1;
			bool sprite_size : 1;
			bool master_slave : 1;
			bool generate_nmi : 1;
		};
		ui8_t data;
	} _ppuctrl;
	//$2001 PPU MASK
	union {
		struct {
			bool grayscale : 1;
			bool render_leftmost_background : 1;
			bool render_leftmost_sprites : 1;
			bool render_background : 1;
			bool render_sprites : 1;
			
			bool emphasize_red : 1;
			bool emphasize_green : 1;
			bool emphasize_blue : 1;
		};
		ui8_t data;
	} _ppumask;
	//$2002 PPU STATUS
	union {
		struct {
			ui8_t previously_written : 5;
			bool sprite_overflow : 1;
			bool sprite_zero_hit : 1;
			bool vertical_blank : 1;
		};
		ui8_t data;
	} _ppustatus;
	//$2006 PPU ADDR
	ui16_t _ppuaddr;
	//$2007 PPU DATA
	ui8_t _ppudata;
	ui8_t _ppudata_buffer;
	
public:
	ui8_t _color_index;
	ui16_t _current_cycle;
	ui16_t _current_scanline;
private: 
	ui16_t _coarse_x;
	ui16_t _coarse_y;
	
	ui8_t _pattern_low_bit_shifter;
	ui8_t _pattern_high_bit_shifter;
	ui8_t _palette_value;
	
	
	
	bool _address_latch;
	bool _call_nmi_cpu;
	bool _completed_frame;
	bool _isVisible;


	ppu_bus* _bus;

	
public:
	u2c02(ppu_bus* b);
	~u2c02();
	
	
	
	bool is_visible();
	bool call_nmi();
	bool completed_frame();
	void debug();

	void clock();

	ui8_t read(ui16_t address);
	void write(ui16_t address, ui8_t _data);
	
private:

	//Helpers
	bool is_rendering();
	
	

};

