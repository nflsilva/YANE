#include "cartridge.hpp"

cartridge::cartridge(char* file_name){
	ifstream fd (file_name, std::ifstream::binary);
	
	if(fd.is_open()) {
		
		_header = new cartridge_header(&fd);
	
		ui8_t n_prg_banks = _header->get_n_prg_banks();
		ui8_t n_chr_banks = _header->get_n_chr_banks();
	
		if(_header->get_mapper1() & 0x04){
			_unused_data = (ui8_t*) malloc(UNUSED_SIZE * sizeof(ui8_t));
			fd.read((char*)_unused_data, UNUSED_SIZE);
		}

		_prg_data = (ui8_t*) malloc(n_prg_banks * PRG_BANK_SIZE * sizeof(ui8_t));
		fd.read((char*)_prg_data, n_prg_banks * PRG_BANK_SIZE);
		
		if(n_chr_banks > 0){
			_chr_data = (ui8_t*) malloc(n_chr_banks * CHR_BANK_SIZE * sizeof(ui8_t));
			fd.read((char*)_chr_data, n_chr_banks * CHR_BANK_SIZE);
		}
		
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
	if(address >= 0x0000 && address <= 0x1FFF){
		return _chr_data[_mapper->map_chr_address(address)];
	}
	else {
		return _prg_data[_mapper->map_prg_address(address)];
	}
}

void cartridge::write(ui16_t address, ui8_t byte){
	// should we write on the rom?
	// empty for now
}


cartridge_header* cartridge::get_header() {
	return _header;
}
