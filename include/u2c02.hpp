#pragma once

#include "ppu_bus.hpp"





class u2c02
{
public:

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
			bool show_leftmost_background : 1;
			bool show_leftmost_sprites : 1;
			bool show_background : 1;
			bool show_sprites : 1;
			
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
	
	
	bool _address_latch;
	bool _should_nmi_cpu;
	bool _completed_frame;
	bool _is_visible;


	ppu_bus* _bus;

	
	ui8_t _current_r;
	ui8_t _current_c;
	ui8_t _nt_buffer[30][32];

	
public:
	u2c02(ppu_bus* b);
	~u2c02();
	
	
	void clock();
	bool should_nmi();

	ui8_t read(ui16_t address);
	void write(ui16_t address, ui8_t _data);
	

};

