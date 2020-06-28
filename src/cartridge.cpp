#include "cartridge.hpp"

cartridge::cartridge(char* file_name){
	ifstream fd (file_name, std::ifstream::binary);
	
	if(fd.is_open()) {
		
		_header = new cartridge_header(&fd);
	
		//_unused_data = (ui8_t*) malloc(UNUSED_SIZE * sizeof(ui8_t));
		//fd.read((char*)_unused_data, UNUSED_SIZE);
		
		_prg_data = (ui8_t*) malloc(PRG_BANK_SIZE * sizeof(ui8_t));
		fd.read((char*)_prg_data, _header->get_n_prg_banks() * PRG_BANK_SIZE);
		
		_chr_data = (ui8_t*) malloc(CHR_BANK_SIZE * sizeof(ui8_t));
		fd.read((char*)_chr_data, _header->get_n_chr_banks() * CHR_BANK_SIZE);
		
		fd.close();
		
			
		_mapper = new mapper000(_header->get_n_prg_banks());
		
	}

	

}

cartridge::~cartridge(){
	free(_unused_data);
	free(_prg_data);
	free(_chr_data);
}

ui8_t cartridge::read(ui16_t address){
	return _prg_data[_mapper->map_prg_address(address)];
}

void cartridge::write(ui16_t address, ui8_t byte){
	// should we write on the rom?
	// empty for now
}
