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
	
	bool is_implicit;
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
	void write(ui16_t address, byte_t data);


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

	//Legal Opcodes
	ui8_t adc_operation(); // A + M + C -> A
	ui8_t and_operation(); // A & M -> A
	ui8_t asl_operation(); // O << 1 -> O
	
	ui8_t bcc_operation(); // branch on C=0
	ui8_t bcs_operation(); // branch on C=1
	ui8_t beq_operation(); // branch on Z=1
	ui8_t bit_operation(); // A and M
	ui8_t bmi_operation(); // branch on N=1
	ui8_t bne_operation(); // branch on Z=0
	ui8_t bpl_operation(); // branch on N=0
	ui8_t brk_operation();
	ui8_t bvc_operation(); // branch on V=0
	ui8_t bvs_operation(); // branch on V=1
	
	ui8_t clc_operation(); // 0 -> C
	ui8_t cld_operation(); // 0 -> D
	ui8_t cli_operation(); // 0 -> I
	ui8_t clv_operation(); // 0 -> V
	ui8_t cmp_operation(); // A - M
	ui8_t cpx_operation(); // X - M
	ui8_t cpy_operation(); // Y - M
	
	ui8_t dec_operation(); // M - 1 -> M
	ui8_t dex_operation(); // X - 1 -> X
	ui8_t dey_operation(); // Y - 1 -> Y
	
	ui8_t eor_operation(); // A or M -> A
	
	ui8_t inc_operation(); // M + 1 -> M
	ui8_t inx_operation(); // X + 1 -> X
	ui8_t iny_operation(); // Y + 1 -> Y
	
	ui8_t jmp_operation(); // jumps to new location
	ui8_t jsr_operation(); // jumps to new location with return address
	
	ui8_t lda_operation(); // M -> A
	ui8_t ldx_operation(); // M -> X
	ui8_t ldy_operation(); // M -> Y
	ui8_t lsr_operation(); // O >> 1 -> O
	
	ui8_t nop_operation(); // No Operation
	
	ui8_t ora_operation(); // A or M -> A
	
	ui8_t pha_operation(); // A -> M[S]; S - 1 -> S
	ui8_t php_operation(); // P -> M[S]; S - 1 -> S
	ui8_t pla_operation(); // S + 1 -> S; M[S] -> A
	ui8_t plp_operation(); // S + 1 -> S; M[S] -> P
	
	ui8_t rol_operation(); // O << 1 < C -> O
	ui8_t ror_operation(); // C > O >> 1 -> O
	ui8_t rti_operation(); // return from interrupt
	ui8_t rts_operation(); // return from subroutine
	
	ui8_t sbc_operation(); // A - M - C -> A
	ui8_t sec_operation(); // 1 -> C
	ui8_t sed_operation(); // 1 -> D
	ui8_t sei_operation(); // 1 -> I
	ui8_t sta_operation(); // A -> M
	ui8_t stx_operation(); // X -> M
	ui8_t sty_operation(); // Y -> M
	
	ui8_t tax_operation(); // A -> X
	ui8_t tay_operation(); // A -> Y
	ui8_t tsx_operation(); // S -> X
	ui8_t txa_operation(); // X -> A
	ui8_t txs_operation(); // X -> S
	ui8_t tya_operation(); // Y -> A
	
	//Illegal Opcodes
	ui8_t kill_operation();
	
	ui8_t anc_operation();
	ui8_t alr_operation();
	ui8_t ahx_operation();
	ui8_t arr_operation();
	ui8_t axs_operation();
	
	ui8_t dcp_operation();
	
	ui8_t isc_operation();
	
	ui8_t las_operation();
	ui8_t lax_operation();
	
	ui8_t rla_operation();
	ui8_t rra_operation();
	
	ui8_t sax_operation();
	ui8_t shx_operation();
	ui8_t shy_operation();
	ui8_t sre_operation();
	ui8_t slo_operation();
	
	ui8_t tas_operation();
	ui8_t xaa_operation();
	
	
	
	
	// Helpers
	void update_flag_N(byte_t data);
	void update_flag_Z(byte_t data);
	
	void push(byte_t data);
	byte_t pop();
	
	ui8_t cross_pages_cicles(ui16_t base_address, ui16_t indexed_address);
	
	ui8_t perform_branch();

};

