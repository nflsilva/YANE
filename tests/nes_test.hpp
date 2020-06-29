#pragma once
#include "i_test.hpp"

#include "nes_console.hpp"


class nes_test : public i_test
{
private:
	
	nes_console* _console;
	
	
public:
	nes_test();
	~nes_test();
	bool run_test();
};

