#include "include/ram.h"

ram::ram(ui16_t size){
	_data = (ui8_t*)malloc(size * sizeof(ui8_t));
	memset(_data, 0, size * sizeof(ui8_t));
}

ram::~ram(){
	free(_data);
}


ui8_t ram::read(ui16_t address){
	ui8_t r = _data[address];
	//std::cout << std::hex << "read " << (int)r << " at " << (int)address << std::endl; 
	return r;
}

void ram::write(ui16_t address, ui8_t data){
	//std::cout << std::hex << "write " << (int)data << " at " << (int)address << std::endl;  
	_data[address] = data;
}


