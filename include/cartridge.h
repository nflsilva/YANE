#pragma once

#include "yane.h"
#include "cartridge_header.h"


#define UNUSED_SIZE 512
#define PRG_BANK_SIZE 16 * 1024
#define CHR_BANK_SIZE 8 * 1024

class cartridge
{
private: 
	cartridge_header* header;
	byte_t* unused_data;
	byte_t* prg_data;
	byte_t* chr_data;
	
	
public:
	cartridge(char* file_name);
	~cartridge();

};

