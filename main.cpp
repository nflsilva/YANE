#include "include/yane.h"

#include <thread>
#include <chrono>

#include "include/cartridge.h"
#include "include/ram.h"
#include "include/cpu_bus.h"
#include "include/r6502.h"


void run_nestest(r6502* cpu);

int main(int argc, char **argv){
	
	char* file_name = (char*)"/mnt/storage/Extra/CodeLiteWorkspaces/CodeLiteRootWorkspace/YANE/resources/nestest.nes";
	
	cartridge* c = new cartridge(file_name);
	ram* r = new ram(2 * 1024);
	cpu_bus* b = new cpu_bus(c, r);
	r6502* cpu = new r6502(b);
	
	run_nestest(cpu);
	
	printf("Done!\n");
	return 0;
}


void run_nestest(r6502* cpu){
	
	char* file_name = (char*)"/mnt/storage/Extra/CodeLiteWorkspaces/CodeLiteRootWorkspace/YANE/resources/nestest.log";
	
	std::ifstream infile(file_name);
	std::string line;
	
	int nc = 100000;
	while(nc > 0){
		
		
		std::getline(infile, line);
		ui8_t op = (ui8_t)strtol(line.substr(6, 2).c_str(), NULL, 16); 
		ui16_t pc = (ui16_t)strtol(line.substr(0, 4).c_str(), NULL, 16); 
		ui8_t a = (ui8_t)strtol(line.substr(48 + 2, 2).c_str(), NULL, 16);  
		ui8_t x = (ui8_t)strtol(line.substr(48 + 7, 2).c_str(), NULL, 16);  
		ui8_t y = (ui8_t)strtol(line.substr(48 + 12, 2).c_str(), NULL, 16); 
		ui16_t s = (ui16_t)strtol(line.substr(48 + 17, 2).c_str(), NULL, 16); 
		ui8_t sp = (ui8_t)strtol(line.substr(48 + 23, 2).c_str(), NULL, 16); 
		ui32_t cyc = (ui32_t)strtol(line.substr(48 + 35 + 6, line.length()-35 - 48 - 6).c_str(), NULL, 16); 
		
		bool r = cpu->compare_state(pc, a, x, y, s, sp, 0);
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
		
		cpu->clock();
		
		
		//std::this_thread::sleep_for (std::chrono::milliseconds(100));
		nc--;
	}
	
}
