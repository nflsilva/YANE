#include "include/r6502.h"

ui8_t cross_pages_cicles(ui16_t base_address, ui16_t indexed_address){
	return 0;
}

r6502::r6502(cpu_bus* bus) : _register_PC(1), _bus(bus){
}

r6502::~r6502(){
}

void r6502::clock(){
	std::cout << "Ticka" << std::endl;
}

ui8_t r6502::read(ui16_t address) {
	//return _bus.read(address);
	return -0x01;
}



ui8_t r6502::accumulator_addressing(){
	return 0;
}
ui8_t r6502::implied_addressing(){
	return 0;
}
ui8_t r6502::immediate_addressing(){
	_operand_address = _register_PC++;
	return 0;
}

ui8_t r6502::relative_addressing(){
	i8_t relative_byte = read(_register_PC++);
	_operand_address = _register_PC + relative_byte;
	return 0;
}
ui8_t r6502::absolute_addressing(){
	ui8_t lower_byte = read(_register_PC++);
	ui8_t higher_byte = read(_register_PC++);
	_operand_address = lower_byte | (higher_byte << 8);
	return 0;
}
ui8_t r6502::zeropage_addressing(){
	ui8_t address_byte = read(_register_PC++);
	_operand_address = address_byte;
	return 0;
}
ui8_t r6502::indirect_addressing(){
	ui8_t prt_lower_byte = read(_register_PC++);
	ui8_t prt_higher_byte = read(_register_PC++);
	ui16_t prt_address = prt_lower_byte | (prt_higher_byte << 8);
	
	ui8_t lower_byte = read(prt_address);
	ui8_t higher_byte = read(prt_address + 1);
	
	_operand_address = lower_byte | (higher_byte << 8);
	
	return 0;
}

ui8_t r6502::absolute_x_indexed_addressing(){
	ui8_t cycles = absolute_addressing();
	ui16_t base_address = _operand_address;
	_operand_address += _register_X;
	return cycles + cross_pages_cicles(base_address, _operand_address);
}
ui8_t r6502::absolute_y_indexed_addressing(){
	ui8_t cycles = absolute_addressing();
	ui16_t base_address = _operand_address;
	_operand_address += _register_Y;
	return cycles + cross_pages_cicles(base_address, _operand_address);
}

ui8_t r6502::zeropage_x_indexed_addressing(){
	ui8_t cycles = zeropage_addressing();
	_operand_address += _register_X;
	_operand_address &= 0xFF;
	return cycles;
}
ui8_t r6502::zeropage_y_indexed_addressing(){
	ui8_t cycles = zeropage_addressing();
	_operand_address += _register_Y;
	_operand_address &= 0xFF;
	return cycles;
}

ui8_t r6502::indexed_indirect_addressing(){
	ui8_t prt_lower_byte = read(_register_PC++);
	ui8_t prt_higher_byte = read(_register_PC++);
	ui16_t prt_address = prt_lower_byte | (prt_higher_byte << 8);
	prt_address += _register_X;
	
	ui8_t lower_byte = read(prt_address);
	ui8_t higher_byte = read(prt_address + 1);
	_operand_address = lower_byte | (higher_byte << 8);
	
	return 0;
}

ui8_t r6502::indirect_indexed_addressing(){
	ui8_t cycles = indirect_addressing();
	ui16_t base_address = _operand_address;
	_operand_address += _register_Y;
	
	
	return cycles + cross_pages_cicles(base_address, _operand_address);
}


