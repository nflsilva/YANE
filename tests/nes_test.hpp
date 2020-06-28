#pragma once
#include "i_test.hpp"


#include "r6502.hpp"
#include "cpu_bus.hpp"
#include "ram.hpp"
#include "cartridge.hpp"

class nes_test : public i_test
{
private:
	
	r6502* _cpu;
	
	
public:
	nes_test();
	~nes_test();
	bool run_test();
};

