#pragma once

#include "yane.h"
#include "cartridge_header.h"
#include "i_mapper.h"
#include "mapper000.h"


#define UNUSED_SIZE 512
#define PRG_BANK_SIZE 16 * 1024
#define CHR_BANK_SIZE 8 * 1024

class cartridge{
private: 

	cartridge_header* _header;
	byte_t* _unused_data;
	byte_t* _prg_data;
	byte_t* _chr_data;
	
	i_mapper* _mapper;
	
public:
	cartridge(char* file_name);
	~cartridge();
	
	byte_t read(ui16_t address);
	void write(ui16_t address, byte_t byte);
	

};

