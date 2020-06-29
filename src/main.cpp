#include "yane.hpp"

#include "nes_console.hpp"


void run_nestest_cpu(nes_console* console);
void run_nestest_ppu(nes_console* console);



int main(int argc, char **argv){
	
	
	char* file_name = (char*)"resources/nestest.nes";
	cartridge* cart = new cartridge(file_name);
	
	nes_console* console = new nes_console(cart);
	
	
	
	run_nestest_ppu(console);
	
	printf("Done!\n");
	return 0;
}

void run_nestest_ppu(nes_console* console){
	
	while(1) {
		
		console->clock();
		
	}
	
	
}

/*
void run_nestest_cpu(nes_console* console){
	
	
	
	char* file_name = (char*)"resources/nestest.log";
	
	std::ifstream infile(file_name);
	std::string line;
	
	while(std::getline(infile, line)){
		
		ui8_t op = (ui8_t)strtol(line.substr(6, 2).c_str(), NULL, 16); 
		ui16_t pc = (ui16_t)strtol(line.substr(0, 4).c_str(), NULL, 16); 
		ui8_t a = (ui8_t)strtol(line.substr(50, 2).c_str(), NULL, 16);  
		ui8_t x = (ui8_t)strtol(line.substr(55, 2).c_str(), NULL, 16);  
		ui8_t y = (ui8_t)strtol(line.substr(60, 2).c_str(), NULL, 16); 
		ui16_t s = (ui16_t)strtol(line.substr(65, 2).c_str(), NULL, 16); 
		ui8_t sp = (ui8_t)strtol(line.substr(71, 2).c_str(), NULL, 16); 
		ui32_t cyc = (ui32_t)strtol(line.substr(90, line.length()-90).c_str(), NULL, 10); 
		
		bool r = cpu->compare_state(pc, a, x, y, s, sp, cyc);
		
		cpu->debug();
		if(!r){
			
			bool _flag_N = s & 0x80; 
			bool _flag_V = s & 0x40;
			bool _flag_U = s & 0x20;
			bool _flag_B = s & 0x10;
			bool _flag_D = s & 0x08;
			bool _flag_I = s & 0x04;
			bool _flag_Z = s & 0x02;
			bool _flag_C = s & 0x01;
			
			
			std::cout << std::endl;
			std::cout << std::hex << "[" << pc << "]" 
			<< " " << (int)op 
			<< " A:" << (int)a 
			<< " X:" << (int)x 
			<< " Y:" << (int)y 
			<< " P:" << (int)s 
			<< " SP:" << (int)sp
			<< std::dec
			<< " CYC:" << (int)cyc
			<< " \t "
			<< " N:" << int(_flag_N)
			<< " V:" << int(_flag_V)
			<< " U:" << int(_flag_U)
			<< " B:" << int(_flag_B)
			<< " D:" << int(_flag_D)
			<< " I:" << int(_flag_I)
			<< " Z:" << int(_flag_Z)
			<< " C:" << int(_flag_C)
			
			
			<< std::endl; 
			break;
		}

		while(!cpu->clock()){
			
			
		};
		
	}

	
}*/

