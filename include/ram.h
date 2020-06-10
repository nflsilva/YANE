#pragma once
#include "yane.h"

class ram
{
private:

	byte_t* data;
	
	
public:
	ram(ui16_t size);
	~ram();
	
	byte_t read(ui16_t address);
	void write(ui16_t address, byte_t data);
	
};

