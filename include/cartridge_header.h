#pragma once
#include "yane.h"

using namespace std;

class cartridge_header
{
private:
	byte_t name[4];
	byte_t n_prg_banks[1];
	byte_t n_chr_banks[1];
	byte_t mapper1[1];
	byte_t mapper2[1];
	byte_t prg_ram_size[1];
	byte_t tv_system1[1];
	byte_t tv_system2[1];
	byte_t unused[5];
	
	
	
public:
	cartridge_header(ifstream* file);
	~cartridge_header();
	
	ui8_t get_n_prg_banks();
	ui8_t get_n_chr_banks();
	ui8_t get_mapper1();
	ui8_t get_mapper2();
	
};

