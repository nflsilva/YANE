#include "include/cartridge_header.h"

cartridge_header::cartridge_header(ifstream* file)
{	
	file->read((char*)name, 4);
	file->read((char*)n_prg_banks, 1);
	file->read((char*)n_chr_banks, 1);
	file->read((char*)mapper1, 1);
	file->read((char*)mapper2, 1);
	file->read((char*)prg_ram_size, 1);
	file->read((char*)tv_system1, 1);
	file->read((char*)tv_system2, 1);
	file->read((char*)unused, 5);
}

cartridge_header::~cartridge_header()
{
}

ui8_t cartridge_header::get_mapper1() {
	return (ui8_t)mapper1[0];
}
ui8_t cartridge_header::get_mapper2(){
	return (ui8_t)mapper2[0];
}
ui8_t cartridge_header::get_n_chr_banks(){
	return (ui8_t)n_chr_banks[0];
}
ui8_t cartridge_header::get_n_prg_banks(){
	return (ui8_t)n_prg_banks[0];
}

