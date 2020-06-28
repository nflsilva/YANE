#pragma once
#include "yane.hpp"

class ram
{
private:

	ui8_t* _data;
	
	
public:
	ram(ui16_t size);
	~ram();
	
	ui8_t read(ui16_t address);
	void write(ui16_t address, ui8_t _data);
	
};

