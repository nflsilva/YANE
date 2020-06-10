#pragma once

#include "include/cpu_bus.h"

class r6502
{
private:

	cpu_bus* _bus;
	
	
public:
	r6502();
	~r6502();

};

