#include "include/cartridge.h"

cartridge::cartridge(char* file_name) {
	ifstream fd (file_name);
	
	if(fd.is_open()) {
		
		header = new cartridge_header(&fd);
	
		//unused_data = (byte_t*) malloc(UNUSED_SIZE * sizeof(byte_t));
		//fd.read((char*)unused_data, UNUSED_SIZE);
		
		prg_data = (byte_t*) malloc(PRG_BANK_SIZE * sizeof(byte_t));
		fd.read((char*)prg_data, header->get_n_prg_banks() * PRG_BANK_SIZE);
		
		chr_data = (byte_t*) malloc(CHR_BANK_SIZE * sizeof(byte_t));
		fd.read((char*)chr_data, header->get_n_chr_banks() * CHR_BANK_SIZE);
	
	
		fd.close();
	}
	

}

cartridge::~cartridge()
{
	free(unused_data);
	free(prg_data);
	free(chr_data);
	
}