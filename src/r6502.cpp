#include "include/r6502.h"


r6502::r6502(cpu_bus* bus) : _register_PC(1), _bus(bus){
}

r6502::~r6502(){
}

void r6502::clock(){
	std::cout << "Ticka" << std::endl;
}

ui8_t r6502::read(ui16_t address) {
	//return _bus->read(address);
	return -0x01;
}
void r6502::write(ui16_t address, byte_t data){
	_bus->write(address, data);
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



ui8_t r6502::adc_operation(){
	ui16_t result = _register_A + _operand_value + (_flag_C ? 1 : 0);
	_flag_C = result > 0xFF;
	
	bool msb_A = _register_A & 0x80;
	bool msb_M = _operand_value & 0x80;
	bool msb_R = result & 0x80;
	
	_flag_V = (msb_A ^ msb_R) && ~(msb_A ^ msb_M);

	_register_A = result;
	update_flag_N(_register_A);
	update_flag_Z(_register_A);

	return 0;
}
ui8_t r6502::and_operation(){
	_register_A &= _operand_value;
	update_flag_Z(_register_A);
	update_flag_N(_register_A);
	return 0;
}
ui8_t r6502::asl_operation(){
	_flag_C = _operand_value & 0x80;
	_operand_value <<= 1;
	update_flag_N(_operand_value);
	update_flag_Z(_operand_value);
	
	if(is_implicit){
		_register_A = _operand_value;
	}
	else {
		write(_operand_address, _operand_value);
	}
	return 0;
}

ui8_t r6502::bcc_operation(){
	if(!_flag_C)
		return perform_branch();
	return 0;
}
ui8_t r6502::bcs_operation(){
	if(_flag_C)
		return perform_branch();
	return 0;
}
ui8_t r6502::beq_operation(){
	if(_flag_Z)
		return perform_branch();
	return 0;
}
ui8_t r6502::bit_operation(){
	byte_t result = _register_A & _operand_value;
	update_flag_Z(result);
	_flag_N = (_operand_value & (1 << 7)) != 0;
	_flag_V = (_operand_value & (1 << 6)) != 0;
	return 0;
}
ui8_t r6502::bmi_operation(){
	if(_flag_N)
		return perform_branch();
	return 0;
}
ui8_t r6502::bne_operation(){
	if(!_flag_Z)
		return perform_branch();
	return 0;
}
ui8_t r6502::bpl_operation(){
	if(!_flag_N)
		return perform_branch();
	return 0;
}
ui8_t r6502::brk_operation(){
	_flag_B = true;
	_flag_U = true;
	return 0;
}
ui8_t r6502::bvc_operation(){
	if(!_flag_V)
		return perform_branch();
	return 0;
}
ui8_t r6502::bvs_operation(){
	if(_flag_V)
		return perform_branch();
	return 0;
}

ui8_t r6502::clc_operation(){
	_flag_C = false;
	return 0;
}
ui8_t r6502::cld_operation(){
	_flag_D = false;
	return 0;
}
ui8_t r6502::cli_operation(){
	_flag_I = false;
	return 0;
}
ui8_t r6502::clv_operation(){
	_flag_V = false;
	return 0;
}
ui8_t r6502::cmp_operation(){
	byte_t result = _register_A - _operand_value;
	update_flag_N(result);
	update_flag_Z(result);
	_flag_C = _register_A >= _operand_value;
	return 0;
}
ui8_t r6502::cpx_operation(){
	byte_t result = _register_X - _operand_value;
	update_flag_N(result);
	update_flag_Z(result);
	_flag_C = _register_X >= _operand_value;
	return 0;
}
ui8_t r6502::cpy_operation(){
	byte_t result = _register_Y - _operand_value;
	update_flag_N(result);
	update_flag_Z(result);
	_flag_C = _register_Y >= _operand_value;
	return 0;
}

ui8_t r6502::dec_operation(){
	_operand_value--;
	write(_operand_address, _operand_value);
	update_flag_N(_operand_value);
	update_flag_Z(_operand_value);
	return 0;
}
ui8_t r6502::dex_operation(){
	_register_X--;
	update_flag_N(_register_X);
	update_flag_Z(_register_X);
	return 0;
}
ui8_t r6502::dey_operation(){
	_register_Y--;
	update_flag_N(_register_Y);
	update_flag_Z(_register_Y);
	return 0;
}

ui8_t r6502::eor_operation(){
	_register_A |= _operand_value;
	update_flag_N(_register_A);
	update_flag_Z(_register_A);
	return 0;
}
ui8_t r6502::inc_operation(){
	_operand_value++;
	write(_operand_address, _operand_value);
	update_flag_N(_operand_value);
	update_flag_Z(_operand_value);
	return 0;
}
ui8_t r6502::inx_operation(){
	_register_X++;
	update_flag_N(_register_X);
	update_flag_Z(_register_X);
	return 0;
}
ui8_t r6502::iny_operation(){
	_register_Y++;
	update_flag_N(_register_Y);
	update_flag_Z(_register_Y);
	return 0;
}

ui8_t r6502::jmp_operation(){
	_register_PC = _operand_address;
	return 0;
}
ui8_t r6502::jsr_operation(){
	_register_PC--;
	
	push(_register_PC >> 8);
	push(_register_PC);

	_register_PC = _operand_address;
	
	return 0;
}

ui8_t r6502::lda_operation(){
	_register_A = _operand_value;
	update_flag_N(_register_A);
	update_flag_Z(_register_A);
	return 0;
}
ui8_t r6502::ldx_operation(){
	_register_X = _operand_value;
	update_flag_N(_register_X);
	update_flag_Z(_register_X);
	return 0;
}
ui8_t r6502::ldy_operation(){
	_register_Y = _operand_value;
	update_flag_N(_register_Y);
	update_flag_Z(_register_Y);
	return 0;
}
ui8_t r6502::lsr_operation(){
	_flag_C = _operand_value & 0x01;
	_operand_value >>= 1;
	update_flag_N(_operand_value);
	update_flag_Z(_operand_value);
	
	if(is_implicit){
		_register_A = _operand_value;
	}
	else {
		write(_operand_address, _operand_value);
	}
	
	return 0;
}

ui8_t r6502::nop_operation(){
	return 0;
}

ui8_t r6502::ora_operation(){
	_register_A |= _operand_value;
	update_flag_N(_register_A);
	update_flag_Z(_register_A);
	return 0;
}

ui8_t r6502::pha_operation(){
	push(_register_A);
	return 0;
}
ui8_t r6502::php_operation(){
	byte_t state = 
	(_flag_N & 0x80) | 
	(_flag_V & 0x40) | 
	0x20   			| 
	0x10   			| 
	(_flag_D & 0x08) |
	(_flag_I & 0x04) |
	(_flag_Z & 0x02) |
	(_flag_C & 0x01);
	
	push(state);
	return 0;
}
ui8_t r6502::pla_operation(){
	_register_A = pop();
	update_flag_N(_register_A);
	update_flag_Z(_register_A);
	return 0;
}
ui8_t r6502::plp_operation(){
	byte_t state = pop();
	_flag_N = state & 0x80; 
	_flag_V = state & 0x40;
	//_flag_B = state & 0x20;
	//_flag_U = state & 0x10;
	_flag_D = state & 0x08;
	_flag_I = state & 0x04;
	_flag_Z = state & 0x02;
	_flag_C = state & 0x01;
	return 0;
}

ui8_t r6502::rol_operation(){
	_flag_C = _operand_value & 0x80;
	_operand_value <<= 1;
	_operand_value |= (_flag_C ? 0x01 : 0x00);

	update_flag_N(_operand_value);
	update_flag_Z(_operand_value);
	
	if(is_implicit){
		_register_A = _operand_value;
	}
	else {
		write(_operand_address, _operand_value);
	}
	return 0;
}
ui8_t r6502::ror_operation(){
	_flag_C = _operand_value & 0x01;
	_operand_value >>= 1;
	_operand_value |= (_flag_C ? 0x80 : 0x00);
	
	update_flag_N(_operand_value);
	update_flag_Z(_operand_value);
	
	if(is_implicit){
		_register_A = _operand_value;
	}
	else {
		write(_operand_address, _operand_value);
	}
	return 0;
}
ui8_t r6502::rti_operation(){
	plp_operation();
	
	byte_t lower_byte = pop();
	byte_t higher_byte = pop();
	
	_register_PC = lower_byte | (higher_byte << 8);
	
	return 0;
}
ui8_t r6502::rts_operation(){
	
	byte_t lower_byte = pop();
	byte_t higher_byte = pop();
	
	_register_PC = lower_byte | (higher_byte << 8);
	_register_PC++;
	
	return 0;
}

ui8_t r6502::sbc_operation(){
	_operand_value = ~_operand_value;
	adc_operation();
	return 0;
}
ui8_t r6502::sec_operation(){
	_flag_C = true;
	return 0;
}
ui8_t r6502::sed_operation(){
	_flag_D = true;
	return 0;
}
ui8_t r6502::sei_operation(){
	_flag_I = true;
	return 0;
}
ui8_t r6502::sta_operation(){
	write(_operand_address, _register_A);
	return 0;
}
ui8_t r6502::stx_operation(){
	write(_operand_address, _register_X);
	return 0;
}
ui8_t r6502::sty_operation(){
	write(_operand_address, _register_Y);
	return 0;
}

ui8_t r6502::tax_operation(){
	_register_X = _register_A;
	update_flag_N(_register_X);
	update_flag_Z(_register_X);
	return 0;
}
ui8_t r6502::tay_operation(){
	_register_Y = _register_A;
	update_flag_N(_register_Y);
	update_flag_Z(_register_Y);
	return 0;
}
ui8_t r6502::tsx_operation(){
	_register_A = _register_X;
	update_flag_N(_register_A);
	update_flag_Z(_register_A);
	return 0;
}
ui8_t r6502::txa_operation(){
	_register_A = _register_X;
	update_flag_N(_register_A);
	update_flag_Z(_register_A);
	return 0;
}
ui8_t r6502::txs_operation(){
	_register_S = _register_X;
	return 0;
}
ui8_t r6502::tya_operation(){
	_register_A = _register_Y;
	update_flag_N(_register_A);
	update_flag_Z(_register_A);
	return 0;
}


void r6502::update_flag_N(byte_t data){
	_flag_N = data & 0x80;
}
void r6502::update_flag_Z(byte_t data){
	_flag_Z = data == 0x00;
}

ui8_t r6502::cross_pages_cicles(ui16_t base_address, ui16_t indexed_address){
	return 0;
}

void r6502::push(byte_t data){
	write(0x0100 + _register_S, data);
	_register_S--;
}
byte_t r6502::pop() {
	_register_S++;
	return read(0x0100 + _register_S);
}

ui8_t r6502::perform_branch(){
	ui8_t cycles = cross_pages_cicles(_register_PC, _operand_address);
	_register_PC = _operand_address;
	return cycles + 1;
}

