#include "r6502.hpp"


r6502::r6502(cpu_bus* bus) : 
_register_A(0), 
_register_Y(0), 
_register_X(0), 
_register_PC(0), 
_register_SP(0), 

_bus(bus),

_is_implied(false),
_operand_value(0),
_operand_address(0),

_operation_cycles(0),
_total_cycles(0)


{
	
	_register_P.data = 0;
	
	using a = r6502;
	r6502_instruction ins[16][16] = {
		/*0*/
		{ 	
			{ &a::brk_operation,	&a::implied_addressing},
			{ &a::ora_operation,	&a::x_indexed_indirect_addressing},
			{ &a::stp_operation,	&a::implied_addressing},
			{ &a::slo_operation,	&a::x_indexed_indirect_addressing},
			{ &a::nop_operation,	&a::zeropage_addressing},
			{ &a::ora_operation,	&a::zeropage_addressing},
			{ &a::asl_operation,	&a::zeropage_addressing},
			{ &a::slo_operation,	&a::zeropage_addressing},
			{ &a::php_operation,	&a::implied_addressing},
			{ &a::ora_operation,	&a::immediate_addressing},
			{ &a::asl_operation,	&a::implied_addressing},
			{ &a::anc_operation,	&a::immediate_addressing},
			{ &a::nop_operation,	&a::absolute_addressing},
			{ &a::ora_operation,	&a::absolute_addressing},
			{ &a::asl_operation,	&a::absolute_addressing},
			{ &a::slo_operation,	&a::absolute_addressing}}, 
		/*1*/
		{
			{ &a::bpl_operation, 	&a::relative_addressing},
			{ &a::ora_operation,	&a::indirect_y_indexed_addressing},
			{ &a::stp_operation,	&a::implied_addressing},
			{ &a::slo_operation,	&a::indirect_y_indexed_addressing},
			{ &a::nop_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::ora_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::asl_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::slo_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::clc_operation,	&a::implied_addressing},
			{ &a::ora_operation,	&a::absolute_y_indexed_addressing},
			{ &a::nop_operation,	&a::implied_addressing},
			{ &a::slo_operation,	&a::absolute_y_indexed_addressing},
			{ &a::nop_operation,	&a::absolute_x_indexed_addressing},
			{ &a::ora_operation,	&a::absolute_x_indexed_addressing},
			{ &a::asl_operation,	&a::absolute_x_indexed_addressing},
			{ &a::slo_operation,	&a::absolute_x_indexed_addressing}},
		/*2*/
		{
			{ &a::jsr_operation, 	&a::absolute_addressing},
			{ &a::and_operation, 	&a::x_indexed_indirect_addressing},
			{ &a::stp_operation, 	&a::implied_addressing},
			{ &a::rla_operation, 	&a::x_indexed_indirect_addressing},
			{ &a::bit_operation, 	&a::zeropage_addressing},
			{ &a::and_operation, 	&a::zeropage_addressing},
			{ &a::rol_operation, 	&a::zeropage_addressing},
			{ &a::rla_operation, 	&a::zeropage_addressing},
			{ &a::plp_operation, 	&a::implied_addressing},
			{ &a::and_operation, 	&a::immediate_addressing},
			{ &a::rol_operation, 	&a::implied_addressing},
			{ &a::anc_operation, 	&a::immediate_addressing},
			{ &a::bit_operation, 	&a::absolute_addressing},
			{ &a::and_operation, 	&a::absolute_addressing},
			{ &a::rol_operation, 	&a::absolute_addressing},
			{ &a::rla_operation, 	&a::absolute_addressing}},
		/*3*/
		{
			{ &a::bmi_operation, 	&a::relative_addressing},
			{ &a::and_operation,	&a::indirect_y_indexed_addressing},
			{ &a::stp_operation,	&a::implied_addressing},
			{ &a::rla_operation,	&a::indirect_y_indexed_addressing},
			{ &a::nop_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::and_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::rol_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::rla_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::sec_operation,	&a::implied_addressing},
			{ &a::and_operation,	&a::absolute_y_indexed_addressing},
			{ &a::nop_operation,	&a::implied_addressing},
			{ &a::rla_operation,	&a::absolute_y_indexed_addressing},
			{ &a::nop_operation,	&a::absolute_x_indexed_addressing},
			{ &a::and_operation,	&a::absolute_x_indexed_addressing},
			{ &a::rol_operation,	&a::absolute_x_indexed_addressing},
			{ &a::rla_operation,	&a::absolute_x_indexed_addressing}},
		/*4*/
		{
			{ &a::rti_operation,	&a::implied_addressing},
			{ &a::eor_operation,	&a::x_indexed_indirect_addressing},
			{ &a::stp_operation,	&a::implied_addressing},
			{ &a::sre_operation,	&a::x_indexed_indirect_addressing},
			{ &a::nop_operation,	&a::zeropage_addressing},
			{ &a::eor_operation,	&a::zeropage_addressing},
			{ &a::lsr_operation,	&a::zeropage_addressing},
			{ &a::sre_operation,	&a::zeropage_addressing},
			{ &a::pha_operation,	&a::implied_addressing},
			{ &a::eor_operation,	&a::immediate_addressing},
			{ &a::lsr_operation,	&a::implied_addressing},
			{ &a::alr_operation,	&a::immediate_addressing},
			{ &a::jmp_operation,	&a::absolute_addressing},
			{ &a::eor_operation,	&a::absolute_addressing},
			{ &a::lsr_operation,	&a::absolute_addressing},
			{ &a::sre_operation,	&a::absolute_addressing}}, 
		/*5*/
		{
			{ &a::bvc_operation, 	&a::relative_addressing},
			{ &a::eor_operation,	&a::indirect_y_indexed_addressing},
			{ &a::stp_operation,	&a::implied_addressing},
			{ &a::sre_operation,	&a::indirect_y_indexed_addressing},
			{ &a::nop_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::eor_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::lsr_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::sre_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::cli_operation,	&a::implied_addressing},
			{ &a::eor_operation,	&a::absolute_y_indexed_addressing},
			{ &a::nop_operation,	&a::implied_addressing},
			{ &a::sre_operation,	&a::absolute_y_indexed_addressing},
			{ &a::nop_operation,	&a::absolute_x_indexed_addressing},
			{ &a::eor_operation,	&a::absolute_x_indexed_addressing},
			{ &a::lsr_operation,	&a::absolute_x_indexed_addressing},
			{ &a::sre_operation,	&a::absolute_x_indexed_addressing}},
		/*6*/
		{
			{ &a::rts_operation,	&a::implied_addressing},
			{ &a::adc_operation,	&a::x_indexed_indirect_addressing},
			{ &a::stp_operation,	&a::implied_addressing},
			{ &a::rra_operation,	&a::x_indexed_indirect_addressing},
			{ &a::nop_operation,	&a::zeropage_addressing},
			{ &a::adc_operation,	&a::zeropage_addressing},
			{ &a::ror_operation,	&a::zeropage_addressing},
			{ &a::rra_operation,	&a::zeropage_addressing},
			{ &a::pla_operation,	&a::implied_addressing},
			{ &a::adc_operation,	&a::immediate_addressing},
			{ &a::ror_operation,	&a::implied_addressing},
			{ &a::arr_operation,	&a::immediate_addressing},
			{ &a::jmp_operation,	&a::indirect_addressing},
			{ &a::adc_operation,	&a::absolute_addressing},
			{ &a::ror_operation,	&a::absolute_addressing},
			{ &a::rra_operation,	&a::absolute_addressing}},
		/*7*/
		{
			{ &a::bvs_operation, 	&a::relative_addressing},
			{ &a::adc_operation,	&a::indirect_y_indexed_addressing},
			{ &a::stp_operation,	&a::implied_addressing},
			{ &a::rra_operation,	&a::indirect_y_indexed_addressing},
			{ &a::nop_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::adc_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::ror_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::rra_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::sei_operation,	&a::implied_addressing},
			{ &a::adc_operation,	&a::absolute_y_indexed_addressing},
			{ &a::nop_operation,	&a::implied_addressing},
			{ &a::rra_operation,	&a::absolute_y_indexed_addressing},
			{ &a::nop_operation,	&a::absolute_x_indexed_addressing},
			{ &a::adc_operation,	&a::absolute_x_indexed_addressing},
			{ &a::ror_operation,	&a::absolute_x_indexed_addressing},
			{ &a::rra_operation,	&a::absolute_x_indexed_addressing}},
		/*8*/
		{
			{ &a::nop_operation,	&a::immediate_addressing},
			{ &a::sta_operation,	&a::x_indexed_indirect_addressing},
			{ &a::nop_operation,	&a::immediate_addressing},
			{ &a::sax_operation,	&a::x_indexed_indirect_addressing},
			{ &a::sty_operation,	&a::zeropage_addressing},
			{ &a::sta_operation,	&a::zeropage_addressing},
			{ &a::stx_operation,	&a::zeropage_addressing},
			{ &a::sax_operation,	&a::zeropage_addressing},
			{ &a::dey_operation,	&a::implied_addressing},
			{ &a::nop_operation,	&a::immediate_addressing},
			{ &a::txa_operation,	&a::implied_addressing},
			{ &a::xaa_operation,	&a::immediate_addressing},
			{ &a::sty_operation,	&a::absolute_addressing},
			{ &a::sta_operation,	&a::absolute_addressing},
			{ &a::stx_operation,	&a::absolute_addressing},
			{ &a::sax_operation,	&a::absolute_addressing}},
		/*9*/
		{
			{ &a::bcc_operation, 	&a::relative_addressing},
			{ &a::sta_operation,	&a::indirect_y_indexed_addressing},
			{ &a::stp_operation,	&a::implied_addressing},
			{ &a::ahx_operation,	&a::indirect_y_indexed_addressing},
			{ &a::sty_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::sta_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::stx_operation,	&a::zeropage_y_indexed_addressing},
			{ &a::sax_operation,	&a::zeropage_y_indexed_addressing},
			{ &a::tya_operation,	&a::implied_addressing},
			{ &a::sta_operation,	&a::absolute_y_indexed_addressing},
			{ &a::txs_operation,	&a::implied_addressing},
			{ &a::tas_operation,	&a::absolute_y_indexed_addressing},
			{ &a::shy_operation,	&a::absolute_x_indexed_addressing},
			{ &a::sta_operation,	&a::absolute_x_indexed_addressing},
			{ &a::shx_operation,	&a::absolute_y_indexed_addressing},
			{ &a::ahx_operation,	&a::absolute_y_indexed_addressing}},
		/*A*/
		{
			{ &a::ldy_operation,	&a::immediate_addressing},
			{ &a::lda_operation,	&a::x_indexed_indirect_addressing},
			{ &a::ldx_operation,	&a::immediate_addressing},
			{ &a::lax_operation,	&a::x_indexed_indirect_addressing},
			{ &a::ldy_operation,	&a::zeropage_addressing},
			{ &a::lda_operation,	&a::zeropage_addressing},
			{ &a::ldx_operation,	&a::zeropage_addressing},
			{ &a::lax_operation,	&a::zeropage_addressing},
			{ &a::tay_operation,	&a::implied_addressing},
			{ &a::lda_operation,	&a::immediate_addressing},
			{ &a::tax_operation,	&a::implied_addressing},
			{ &a::lax_operation,	&a::immediate_addressing},
			{ &a::ldy_operation,	&a::absolute_addressing},
			{ &a::lda_operation,	&a::absolute_addressing},
			{ &a::ldx_operation,	&a::absolute_addressing},
			{ &a::lax_operation,	&a::absolute_addressing}},
		/*B*/
		{
			{ &a::bcs_operation, 	&a::relative_addressing},
			{ &a::lda_operation,	&a::indirect_y_indexed_addressing},
			{ &a::stp_operation,	&a::implied_addressing},
			{ &a::lax_operation,	&a::indirect_y_indexed_addressing},
			{ &a::ldy_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::lda_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::ldx_operation,	&a::zeropage_y_indexed_addressing},
			{ &a::lax_operation,	&a::zeropage_y_indexed_addressing},
			{ &a::clv_operation,	&a::implied_addressing},
			{ &a::lda_operation,	&a::absolute_y_indexed_addressing},
			{ &a::tsx_operation,	&a::implied_addressing},
			{ &a::las_operation,	&a::absolute_y_indexed_addressing},
			{ &a::ldy_operation,	&a::absolute_x_indexed_addressing},
			{ &a::lda_operation,	&a::absolute_x_indexed_addressing},
			{ &a::ldx_operation,	&a::absolute_y_indexed_addressing},
			{ &a::lax_operation,	&a::absolute_y_indexed_addressing}},
		/*C*/
		{
			{ &a::cpy_operation,	&a::immediate_addressing},
			{ &a::cmp_operation,	&a::x_indexed_indirect_addressing},
			{ &a::nop_operation,	&a::immediate_addressing},
			{ &a::dcp_operation,	&a::x_indexed_indirect_addressing},
			{ &a::cpy_operation,	&a::zeropage_addressing},
			{ &a::cmp_operation,	&a::zeropage_addressing},
			{ &a::dec_operation,	&a::zeropage_addressing},
			{ &a::dcp_operation,	&a::zeropage_addressing},
			{ &a::iny_operation,	&a::implied_addressing},
			{ &a::cmp_operation,	&a::immediate_addressing},
			{ &a::dex_operation,	&a::implied_addressing},
			{ &a::axs_operation,	&a::immediate_addressing},
			{ &a::cpy_operation,	&a::absolute_addressing},
			{ &a::cmp_operation,	&a::absolute_addressing},
			{ &a::dec_operation,	&a::absolute_addressing},
			{ &a::dcp_operation,	&a::absolute_addressing}},
		/*D*/
		{
			{ &a::bne_operation, 	&a::relative_addressing},
			{ &a::cmp_operation,	&a::indirect_y_indexed_addressing},
			{ &a::stp_operation,	&a::implied_addressing},
			{ &a::dcp_operation,	&a::indirect_y_indexed_addressing},
			{ &a::nop_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::cmp_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::dec_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::dcp_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::cld_operation,	&a::implied_addressing},
			{ &a::cmp_operation,	&a::absolute_y_indexed_addressing},
			{ &a::nop_operation,	&a::implied_addressing},
			{ &a::dcp_operation,	&a::absolute_y_indexed_addressing},
			{ &a::nop_operation,	&a::absolute_x_indexed_addressing},
			{ &a::cmp_operation,	&a::absolute_x_indexed_addressing},
			{ &a::dec_operation,	&a::absolute_x_indexed_addressing},
			{ &a::dcp_operation,	&a::absolute_x_indexed_addressing}},
		/*E*/
		{
			{ &a::cpx_operation,	&a::immediate_addressing},
			{ &a::sbc_operation,	&a::x_indexed_indirect_addressing},
			{ &a::nop_operation,	&a::immediate_addressing},
			{ &a::isc_operation,	&a::x_indexed_indirect_addressing},
			{ &a::cpx_operation,	&a::zeropage_addressing},
			{ &a::sbc_operation,	&a::zeropage_addressing},
			{ &a::inc_operation,	&a::zeropage_addressing},
			{ &a::isc_operation,	&a::zeropage_addressing},
			{ &a::inx_operation,	&a::implied_addressing},
			{ &a::sbc_operation,	&a::immediate_addressing},
			{ &a::nop_operation,	&a::implied_addressing},
			{ &a::sbc_operation,	&a::immediate_addressing},
			{ &a::cpx_operation,	&a::absolute_addressing},
			{ &a::sbc_operation,	&a::absolute_addressing},
			{ &a::inc_operation,	&a::absolute_addressing},
			{ &a::isc_operation,	&a::absolute_addressing}},
		/*F*/
		{
			{ &a::beq_operation, 	&a::relative_addressing},
			{ &a::sbc_operation,	&a::indirect_y_indexed_addressing},
			{ &a::stp_operation,	&a::implied_addressing},
			{ &a::isc_operation,	&a::indirect_y_indexed_addressing},
			{ &a::nop_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::sbc_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::inc_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::isc_operation,	&a::zeropage_x_indexed_addressing},
			{ &a::sed_operation,	&a::implied_addressing},
			{ &a::sbc_operation,	&a::absolute_y_indexed_addressing},
			{ &a::nop_operation,	&a::implied_addressing},
			{ &a::isc_operation,	&a::absolute_y_indexed_addressing},
			{ &a::nop_operation,	&a::absolute_x_indexed_addressing},
			{ &a::sbc_operation,	&a::absolute_x_indexed_addressing},
			{ &a::inc_operation,	&a::absolute_x_indexed_addressing},
			{ &a::isc_operation,	&a::absolute_x_indexed_addressing}},
	};
	for(int r = 0; r < 16; r++){
		for(int c = 0; c < 16; c++){
			instructions[r][c] = ins[r][c];
		}
	}
	
	cross_pages_instructions.insert(0x1E);
	cross_pages_instructions.insert(0x3E);
	cross_pages_instructions.insert(0x5E);
	cross_pages_instructions.insert(0x7E);
	cross_pages_instructions.insert(0xDE);
	cross_pages_instructions.insert(0xFE);
	cross_pages_instructions.insert(0x91);
	cross_pages_instructions.insert(0x99);
	cross_pages_instructions.insert(0x9D);
	
	__print = false;
	
}

r6502::~r6502(){
}



bool r6502::clock(){

	bool r = false;
	
	if(_register_PC == 0xF208){
		__print = true;
	}
	
	if(_operation_cycles == 0) {
		_operation_opcode = read(_register_PC++);
		
		ui8_t (r6502::* addressing)(void) = NULL;
		ui8_t (r6502::* operation)(void) = NULL;
		
		ui8_t row_index = (_operation_opcode >> 4) & 0x0F;
		ui8_t col_index = _operation_opcode & 0x0F;
		
		r6502_instruction current_instruction = instructions[row_index][col_index];
		addressing = current_instruction.addressing;
		operation = current_instruction.operation;
		
		_is_implied = false;
		
		_operation_cycles += (this->*addressing)();
	
		_operation_cycles += (this->*operation)();
		
		
		_total_cycles += max(2, (_operation_cycles - 1));
		r = true;
	}
	_operation_cycles--;
	
	return r;
}

void r6502::reset(){
	
	ui16_t vector = 0xFFFC;
	ui8_t low_pc = read(vector) & 0x00FF;
	ui8_t high_pc = read(vector + 1) & 0x00FF;
	_register_PC = (high_pc << 8) | low_pc;
	
	
	_operand_value = 0;
	_operand_address = 0;
	
	_operation_cycles = 8;
	_total_cycles = 0;
	
}
void r6502::nmi(){

	push(_register_PC >> 8);
	push(_register_PC);
	
	_register_P.flag_B = false;
	_register_P.flag_U = true;
	_register_P.flag_I = true;
	
	push(_register_P.data);
	
	ui16_t vector = 0xFFFA;
	ui8_t low_pc = read(vector) & 0x00FF;
	ui8_t high_pc = read(vector + 1) & 0x00FF;
	_register_PC = (high_pc << 8) | low_pc;
	
	_operation_cycles = 8;
	
}
void r6502::irq(){
	
	if(!_register_P.flag_I){
		
		push(_register_PC >> 8);
		push(_register_PC);
		
		_register_P.flag_B = false;
		_register_P.flag_U = true;
		_register_P.flag_I = true;
		
		push(_register_P.data);
		
		ui16_t vector = 0xFFFE;
		ui8_t low_pc = read(vector) & 0x00FF;
		ui8_t high_pc = read(vector + 1) & 0x00FF;
		_register_PC = (high_pc << 8) | low_pc;
		
		_operation_cycles = 7;
	}

}




ui8_t r6502::read(ui16_t address) {
	_operation_cycles++;
	return _bus->read(address);
}
void r6502::write(ui16_t address, ui8_t data){
	_operation_cycles++;
	_bus->write(address, data);
}


ui8_t r6502::implied_addressing(){
	_is_implied = true;
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
	_operand_address = read(_register_PC++);
	return 0;
}
ui8_t r6502::indirect_addressing(){
	
	ui8_t prt_lower_byte = read(_register_PC++);
	ui8_t prt_higher_byte = read(_register_PC++);
	
	ui16_t prt_address = prt_lower_byte | (prt_higher_byte << 8);
	
	ui8_t lower_byte = read(prt_address);
	ui8_t higher_byte = 0x00;
	
	//simulates hardware bug
	if(prt_lower_byte == 0xFF){
		higher_byte = read(prt_address & 0xFF00);
	}
	else {
		higher_byte = read(prt_address + 1);
	}
	
	_operand_address = lower_byte | (higher_byte << 8);
	
	return 0;
}

ui8_t r6502::absolute_x_indexed_addressing(){
	ui8_t cycles = absolute_addressing();
	ui16_t base_address = _operand_address;
	_operand_address += _register_X;
	return cycles + cross_pages_cycles(base_address, _operand_address);
}
ui8_t r6502::absolute_y_indexed_addressing(){
	ui8_t cycles = absolute_addressing();
	ui16_t base_address = _operand_address;
	_operand_address += _register_Y;
	return cycles + cross_pages_cycles(base_address, _operand_address);
}
ui8_t r6502::zeropage_x_indexed_addressing(){
	_operand_address = (read(_register_PC++) + _register_X) & 0xFF;
	return 1;
}
ui8_t r6502::zeropage_y_indexed_addressing(){
	_operand_address = (read(_register_PC++) + _register_Y) & 0xFF;
	return 1;
}

ui8_t r6502::x_indexed_indirect_addressing(){
	ui8_t prt_zp = read(_register_PC++);
	ui8_t lower_byte = read((prt_zp + _register_X) & 0xFF);
	ui8_t higher_byte = read((prt_zp + _register_X + 1) & 0xFF);
	_operand_address = lower_byte | (higher_byte << 8);
	return 1;
}
ui8_t r6502::indirect_y_indexed_addressing(){

	ui8_t prt_zp = read(_register_PC++);
	ui8_t lower_byte = read(prt_zp & 0xFF);
	ui8_t higher_byte = read((prt_zp + 1) & 0xFF);
	
	ui16_t base_address = lower_byte | (higher_byte << 8);
	_operand_address = base_address + _register_Y;
	
	return cross_pages_cycles(base_address, _operand_address);
}

ui8_t r6502::adc_operation(){
	asdc_operation(false);
	return 1;
}
ui8_t r6502::and_operation(){
	fetch_operand();
	_register_A &= _operand_value;
	_register_P.flag_N = _register_A & 0x80;
	_register_P.flag_Z = _register_A == 0;
	return 1;
}
ui8_t r6502::asl_operation(){
	
	fetch_operand();
	
	ui16_t result = _operand_value << 1;
	_register_P.flag_C = (result & 0xFF00) > 0;
	
	_operand_value = result & 0x00FF;
	_register_P.flag_N = _operand_value & 0x80;
	_register_P.flag_Z = _operand_value == 0;
	
	if(_is_implied){
		_register_A = _operand_value;
	}
	else {
		write(_operand_address, _operand_value);
		_operation_cycles += 2;
	}
	return 0;
}

ui8_t r6502::bcc_operation(){
	if(!_register_P.flag_C)
		return perform_branch() + 1;
	return 1;
}
ui8_t r6502::bcs_operation(){
	if(_register_P.flag_C)
		return perform_branch() + 1;
	return 1;
}
ui8_t r6502::beq_operation(){
	if(_register_P.flag_Z)
		return perform_branch() + 1;
	return 1;
}
ui8_t r6502::bit_operation(){
	fetch_operand();
	ui8_t result = _register_A & _operand_value;
	_register_P.flag_Z = result == 0;
	_register_P.flag_N = _operand_value & 0x80;
	_register_P.flag_V = _operand_value & 0x40;
	return 1;
}
ui8_t r6502::bmi_operation(){
	if(_register_P.flag_N)
		return perform_branch() + 1;
	return 1;
}
ui8_t r6502::bne_operation(){
	if(!_register_P.flag_Z)
		return perform_branch() + 1;
	return 1;
}
ui8_t r6502::bpl_operation(){
	if(!_register_P.flag_N)
		return perform_branch() + 1;
	return 1;
}
ui8_t r6502::brk_operation(){

	_register_PC++;
	
	_register_P.flag_I = true;
	push(_register_PC >> 8);
	push(_register_PC);
		
	_register_P.flag_B = true;
	push(_register_P.data);
	_register_P.flag_B = false;
	
	ui16_t vector = 0xFFFE;
	ui8_t low_pc = read(vector) & 0x00FF;
	ui8_t high_pc = read(vector + 1) & 0x00FF;
	_register_PC = (high_pc << 8) | low_pc;
	
	_operation_cycles = 7;
	
	
	return 0;
}
ui8_t r6502::bvc_operation(){
	if(!_register_P.flag_V)
		return perform_branch() + 1;
	return 1;
}
ui8_t r6502::bvs_operation(){
	if(_register_P.flag_V)
		return perform_branch() + 1;
	return 1;
}
ui8_t r6502::clc_operation(){
	_register_P.flag_C = false;
	return 1;
}
ui8_t r6502::cld_operation(){
	_register_P.flag_D = false;
	return 1;
}
ui8_t r6502::cli_operation(){
	_register_P.flag_I = false;
	return 1;
}
ui8_t r6502::clv_operation(){
	_register_P.flag_V = false;
	return 1;
}
ui8_t r6502::cmp_operation(){
	fetch_operand();
	ui8_t result = _register_A - _operand_value;
	_register_P.flag_N = result & 0x80;
	_register_P.flag_Z = result == 0;
	_register_P.flag_C = _register_A >= _operand_value;
	return 1;
}
ui8_t r6502::cpx_operation(){
	fetch_operand();
	ui8_t result = _register_X - _operand_value;
	_register_P.flag_N = result & 0x80;
	_register_P.flag_Z = result == 0;
	_register_P.flag_C = _register_X >= _operand_value;
	return 1;
}
ui8_t r6502::cpy_operation(){
	fetch_operand();
	ui8_t result = _register_Y - _operand_value;
	_register_P.flag_N = result & 0x80;
	_register_P.flag_Z = result == 0;
	_register_P.flag_C = _register_Y >= _operand_value;
	return 1;
}

ui8_t r6502::dec_operation(){
	fetch_operand();
	_operand_value--;
	write(_operand_address, _operand_value);
	_register_P.flag_N = _operand_value & 0x80;
	_register_P.flag_Z = _operand_value == 0;
	return 2;
}
ui8_t r6502::dex_operation(){
	_register_X--;
	_register_P.flag_N = _register_X & 0x80;
	_register_P.flag_Z = _register_X == 0;
	return 1;
}
ui8_t r6502::dey_operation(){
	_register_Y--;
	_register_P.flag_N = _register_Y & 0x80;
	_register_P.flag_Z = _register_Y == 0;
	return 1;
}

ui8_t r6502::eor_operation(){
	fetch_operand();
	_register_A ^= _operand_value;
	_register_P.flag_N = _register_A & 0x80;
	_register_P.flag_Z = _register_A == 0;
	return 1;
}

ui8_t r6502::inc_operation(){
	fetch_operand();
	_operand_value++;
	write(_operand_address, _operand_value);
	_register_P.flag_N = _operand_value & 0x80;
	_register_P.flag_Z = _operand_value == 0;
	return 2;
}
ui8_t r6502::inx_operation(){
	_register_X++;
	_register_P.flag_N = _register_X & 0x80;
	_register_P.flag_Z = _register_X == 0;
	return 1;
}
ui8_t r6502::iny_operation(){
	_register_Y++;
	_register_P.flag_N = _register_Y & 0x80;
	_register_P.flag_Z = _register_Y == 0;
	return 1;
}

ui8_t r6502::jmp_operation(){
	fetch_operand();
	_register_PC = _operand_address;
	return 0;
}
ui8_t r6502::jsr_operation(){
	fetch_operand();
	_register_PC--;
	
	push(_register_PC >> 8);
	push(_register_PC);

	_register_PC = _operand_address;
	
	return 1;
}

ui8_t r6502::lda_operation(){
	fetch_operand();
	_register_A = _operand_value;
	_register_P.flag_N = _register_A & 0x80;
	_register_P.flag_Z = _register_A == 0;
	return 1;
}
ui8_t r6502::ldx_operation(){
	fetch_operand();
	_register_X = _operand_value;
	_register_P.flag_N = _register_X & 0x80;
	_register_P.flag_Z = _register_X == 0;
	return 1;
}
ui8_t r6502::ldy_operation(){
	fetch_operand();
	_register_Y = _operand_value;
	_register_P.flag_N = _register_Y & 0x80;
	_register_P.flag_Z = _register_Y == 0;
	return 1;
}
ui8_t r6502::lsr_operation(){
	fetch_operand();
	_register_P.flag_C = _operand_value & 0x01;
	_operand_value >>= 1;
	_register_P.flag_N = _operand_value & 0x80;
	_register_P.flag_Z = _operand_value == 0;
	
	if(_is_implied){
		_register_A = _operand_value;
	}
	else {
		write(_operand_address, _operand_value);
		_operation_cycles += 2;
	}
	
	return 0;
}

ui8_t r6502::nop_operation(){
	return 2;
}

ui8_t r6502::ora_operation(){
	fetch_operand();
	_register_A |= _operand_value;
	_register_P.flag_N = _register_A & 0x80;
	_register_P.flag_Z = _register_A == 0;
	return 1;
}

ui8_t r6502::pha_operation(){
	push(_register_A);
	return 2;
}
ui8_t r6502::php_operation(){
	push(_register_P.data | 0x30);
	return 2;
}
ui8_t r6502::pla_operation(){
	_register_A = pop();
	_register_P.flag_N = _register_A & 0x80;
	_register_P.flag_Z = _register_A == 0;
	return 2;
}
ui8_t r6502::plp_operation(){
	bool current_flag_U = _register_P.flag_U;
	bool current_flag_B = _register_P.flag_B;
	_register_P.data = pop();
	_register_P.flag_U = current_flag_U;
	_register_P.flag_B = current_flag_B;
	return 2;
}

ui8_t r6502::rol_operation(){
	
	fetch_operand();
	ui16_t result = (_operand_value << 1) | (_register_P.flag_C ? 0x01 : 0x00);
	_register_P.flag_C = result & 0xFF00;
	_operand_value = result;
	

	_register_P.flag_N = _operand_value & 0x80;
	_register_P.flag_Z = _operand_value == 0;
	
	if(_is_implied){
		_register_A = _operand_value;
	}
	else {
		write(_operand_address, _operand_value);
		_operation_cycles += 2;
	}
	return 0;
}
ui8_t r6502::ror_operation(){
	fetch_operand();
	bool aux_c = _operand_value & 0x01;
	_operand_value >>= 1;
	_operand_value |= (_register_P.flag_C ? 0x80 : 0x00);
	_register_P.flag_C = aux_c;
	
	_register_P.flag_N = _operand_value & 0x80;
	_register_P.flag_Z = _operand_value == 0;
	
	if(_is_implied){
		_register_A = _operand_value;
	}
	else {
		write(_operand_address, _operand_value);
		_operation_cycles += 2;
	}
	return 0;
}
ui8_t r6502::rti_operation(){
	plp_operation();
	
	ui8_t lower_byte = pop();
	ui8_t higher_byte = pop();
	
	_register_PC = lower_byte | (higher_byte << 8);
	
	return 0;
}
ui8_t r6502::rts_operation(){
	
	ui8_t lower_byte = pop();
	ui8_t higher_byte = pop();
	
	_register_PC = lower_byte | (higher_byte << 8);
	_register_PC++;
	
	return 2;
}

ui8_t r6502::sbc_operation(){
	asdc_operation(true);
	return 1;
}
ui8_t r6502::sec_operation(){
	_register_P.flag_C = true;
	return 1;
}
ui8_t r6502::sed_operation(){
	_register_P.flag_D = true;
	return 1;
}
ui8_t r6502::sei_operation(){
	_register_P.flag_I = true;
	return 1;
}
ui8_t r6502::sta_operation(){
	write(_operand_address, _register_A);
	return 1;
}
ui8_t r6502::stx_operation(){
	write(_operand_address, _register_X);
	return 1;
}
ui8_t r6502::sty_operation(){
	write(_operand_address, _register_Y);
	return 1;
}

ui8_t r6502::tax_operation(){
	_register_X = _register_A;
	_register_P.flag_N = _register_X & 0x80;
	_register_P.flag_Z = _register_X == 0;
	return 1;
}
ui8_t r6502::tay_operation(){
	_register_Y = _register_A;
	_register_P.flag_N = _register_Y & 0x80;
	_register_P.flag_Z = _register_Y == 0;
	return 1;
}
ui8_t r6502::tsx_operation(){
	_register_X = _register_SP;
	_register_P.flag_N = _register_X & 0x80;
	_register_P.flag_Z = _register_X == 0;
	return 1;
}
ui8_t r6502::txa_operation(){
	_register_A = _register_X;
	_register_P.flag_N = _register_A & 0x80;
	_register_P.flag_Z = _register_A == 0;
	return 1;
}
ui8_t r6502::txs_operation(){
	_register_SP = _register_X;
	return 1;
}
ui8_t r6502::tya_operation(){
	_register_A = _register_Y;
	_register_P.flag_N = _register_A & 0x80;
	_register_P.flag_Z = _register_A == 0;
	return 1;
}


//Illegal Opcodes
ui8_t r6502::anc_operation(){
	return 0;
}
ui8_t r6502::alr_operation(){
	return 0;
}
ui8_t r6502::ahx_operation(){
	return 0;
}
ui8_t r6502::arr_operation(){
	return 0;
}
ui8_t r6502::axs_operation(){
	return 0;
}

ui8_t r6502::dcp_operation(){
	dec_operation();
	cmp_operation();
	return 1;
}
ui8_t r6502::isc_operation(){
	inc_operation();
	sbc_operation();
	return 1;
}
ui8_t r6502::las_operation(){
	return 0;
}
ui8_t r6502::lax_operation(){
	lda_operation();
	ldx_operation();
	return 0;
}

ui8_t r6502::rla_operation(){
	rol_operation();
	and_operation();
	return -1;
}
ui8_t r6502::rra_operation(){
	ror_operation();
	adc_operation();
	return -1;
}

ui8_t r6502::sax_operation(){
	fetch_operand();
	write(_operand_address, _register_A & _register_X);
	return 0;
}
ui8_t r6502::shx_operation(){
	return 0;
}
ui8_t r6502::shy_operation(){
	return 0;
}
ui8_t r6502::sre_operation(){
	lsr_operation();
	eor_operation();
	return -1;
}
ui8_t r6502::slo_operation(){
	asl_operation();
	ora_operation();
	return -1;
}

ui8_t r6502::tas_operation(){
	return 0;
}
ui8_t r6502::xaa_operation(){
	return 0;
}

ui8_t r6502::stp_operation(){
	return 0;
}


//Helper methods
void r6502::fetch_operand(){
	if(_is_implied){
		_operand_value = _register_A;
	}
	else {
		_operand_value = read(_operand_address);
	}
	
}
void r6502::push(ui8_t data){
	write(0x0100 + _register_SP, data);
	_register_SP--;
}
ui8_t r6502::pop() {
	_register_SP++;
	_operation_cycles++;
	return read(0x0100 + _register_SP);
}
ui8_t r6502::cross_pages_cycles(ui16_t base_address, ui16_t indexed_address){
	
	if((base_address & 0xFF00) != (indexed_address & 0xFF00) || 
		cross_pages_instructions.find(_operation_opcode) != cross_pages_instructions.end()) {
		return 1;
	}
	
	return 0;
}
ui8_t r6502::perform_branch(){
	ui8_t cycles = cross_pages_cycles(_register_PC, _operand_address);
	_register_PC = _operand_address;
	return cycles + 1;
}
void r6502::asdc_operation(bool is_subtraction){
	fetch_operand();
	if(is_subtraction){
		_operand_value = ~_operand_value;
	}
	
	ui16_t result = _register_A + _operand_value + (_register_P.flag_C ? 1 : 0);
	_register_P.flag_C = result > 0xFF;
	
	/*
	bool msb_A = _register_A & 0x80;
	bool msb_M = _operand_value & 0x80;
	bool msb_R = result & 0x80;
	_flag_V = (msb_A != msb_R) && (~(msb_A != msb_M));
	*/
	
	_register_P.flag_V = ((~(_register_A ^ _operand_value) & (_register_A ^ result)) & 0x80) != 0x00;
	
	
	_register_A = result;
	_register_P.flag_N = _register_A & 0x80;
	_register_P.flag_Z = _register_A == 0;
}

//Debug
void r6502::setup_state(ui16_t register_PC, ui8_t register_P, ui8_t register_SP, ui32_t cycles){
	_register_PC = register_PC;
	_register_P.data = register_P;
	_register_SP = register_SP;
	_total_cycles = cycles;
}
bool r6502::compare_state(ui16_t register_PC, ui8_t register_A, ui8_t register_X, ui8_t register_Y, ui8_t state, ui8_t register_S, ui32_t cycles){
	
	bool pc = _register_PC == register_PC;
	bool a = _register_A == register_A;
	bool x = _register_X == register_X;
	bool y = _register_Y == register_Y;
	bool p = state == _register_P.data;
	bool s = _register_SP == register_S;
	bool cyc = _total_cycles == cycles;
	
	return pc && a && x && y && p && s && cyc;
}
void r6502::debug(){
	
	std::stringstream ss;
	ss << std::hex << _register_PC;
	std::string debug_string = "[" + ss.str() + "]";
	
	ss.str(std::string());
	ss << std::hex << ((int)_bus->read(_register_PC));
	debug_string += " " + ss.str();
	
	ss.str(std::string());
	ss << std::hex << ((int)_register_A);
	debug_string += " A:" + ss.str();
	
	ss.str(std::string());
	ss << std::hex << ((int)_register_X);
	debug_string += " X:" + ss.str();
	
	ss.str(std::string());
	ss << std::hex << ((int)_register_Y);
	debug_string += " Y:" + ss.str();

	
	ss.str(std::string());
	ss << std::hex << (int)_register_P.data;
	debug_string += " P:" + ss.str();
	
	ss.str(std::string());
	ss << std::hex << ((int)_register_SP);
	debug_string += " SP:" + ss.str();
	

	std::cout << debug_string
	<< dec
	<< " CYC:" << ((int)_total_cycles)
	<< " \t "
	<< " N:" << int(_register_P.flag_N)
	<< " V:" << int(_register_P.flag_V)
	<< " U:" << int(_register_P.flag_U)
	<< " B:" << int(_register_P.flag_B)
	<< " D:" << int(_register_P.flag_D)
	<< " I:" << int(_register_P.flag_I)
	<< " Z:" << int(_register_P.flag_Z)
	<< " C:" << int(_register_P.flag_C)

	<< std::endl;
	
}


