#include "include/ram.h"

ram::ram(ui16_t size){
	data = (byte_t*)malloc(size * sizeof(byte_t));
	memset(data, 0, size * sizeof(byte_t));
}

ram::~ram(){
	free(data);
}


byte_t ram::read(ui16_t address){
	return 0x0000;
}

void ram::write(ui16_t address, byte_t data){
	
}


