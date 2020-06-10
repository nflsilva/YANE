#pragma once
#include "yane.h"

class ibus
{
	
	
public:
	virtual void write(ui16_t address, byte_t byte) = 0;
	virtual byte_t read(ui16_t address) = 0;
};

