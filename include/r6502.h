#pragma once

#include "include/cpu_bus.h"

class r6502
{
private:

	ui8_t _register_A;		//Accumulator
	ui8_t _register_Y;		//Y Register
	ui8_t _register_X;		//X Register
	ui16_t _register_PC;	//Program Counter
	ui8_t _register_S;		//Stack Pointer
	
	bool _flag_N;			//Negative
	bool _flag_V;			//Overflow
	bool _flag_U;			//Unused
	bool _flag_B;			//Break
	bool _flag_D;			//Decimal / Unused
	bool _flag_I;			//Disable Interrupts
	bool _flag_Z;			//Zero
	bool _flag_C;			//Cary


	cpu_bus* _bus;
	
	
	ui8_t _operand_value;
	ui16_t _operand_address;
	
	
	ui32_t _cycles;
	
	
public:
	r6502(cpu_bus* bus);
	~r6502();
	

	
	void clock();
	void reset();
	void nmi();
	

public:
	ui8_t read(ui16_t address);


	//Addressing
	//Non-Indexed, non-memory
	ui8_t accumulator_addressing();
	ui8_t implied_addressing();
	ui8_t immediate_addressing();
	
	//Non-Indexed, memory
	ui8_t relative_addressing();
	ui8_t absolute_addressing();
	ui8_t zeropage_addressing();
	ui8_t indirect_addressing();
	

	//Indexed
	ui8_t absolute_x_indexed_addressing();
	ui8_t absolute_y_indexed_addressing();
	
	ui8_t zeropage_x_indexed_addressing();
	ui8_t zeropage_y_indexed_addressing();
	
	ui8_t indirect_indexed_addressing();
	ui8_t indexed_indirect_addressing();


};

