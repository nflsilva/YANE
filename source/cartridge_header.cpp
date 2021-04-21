#include "cartridge_header.hpp"

cartridge_header::cartridge_header(ifstream* file){	
	file->read((char*)_name, 4);
	file->read((char*)_n_prg_banks, 1);
	file->read((char*)_n_chr_banks, 1);
	file->read((char*)_mapper1, 1);
	file->read((char*)_mapper2, 1);
	file->read((char*)_prg_ram_size, 1);
	file->read((char*)_tv_system1, 1);
	file->read((char*)_tv_system2, 1);
	file->read((char*)_unused, 5);
}

cartridge_header::~cartridge_header(){
}

ui8_t cartridge_header::get_mapper1(){
	return (ui8_t)_mapper1[0];
}
ui8_t cartridge_header::get_mapper2(){
	return (ui8_t)_mapper2[0];
}
ui8_t cartridge_header::get_n_chr_banks(){
	return (ui8_t)_n_chr_banks[0];
}
ui8_t cartridge_header::get_n_prg_banks(){
	return (ui8_t)_n_prg_banks[0];
}

