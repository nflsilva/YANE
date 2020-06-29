#include "nes_test.hpp"

nes_test::nes_test()
{	
	char* file_name = (char*)"resources/nestest.nes";
	cartridge* c = new cartridge(file_name);
	ram* r = new ram(2 * 1024);
	cpu_bus* b = new cpu_bus(c, r);
	_cpu = new r6502(b);
	
	_name = "nestest ROM";
	
}

nes_test::~nes_test()
{	
}


bool nes_test::run_test(){
	
	
	char* file_name = (char*)"resources/nestest.log";
	
	std::ifstream infile(file_name);
	std::string line;
	
	while(std::getline(infile, line)){
		
		//ui8_t op = (ui8_t)strtol(line.substr(6, 2).c_str(), NULL, 16); 
		ui16_t pc = (ui16_t)strtol(line.substr(0, 4).c_str(), NULL, 16); 
		ui8_t a = (ui8_t)strtol(line.substr(50, 2).c_str(), NULL, 16);  
		ui8_t x = (ui8_t)strtol(line.substr(55, 2).c_str(), NULL, 16);  
		ui8_t y = (ui8_t)strtol(line.substr(60, 2).c_str(), NULL, 16); 
		ui16_t s = (ui16_t)strtol(line.substr(65, 2).c_str(), NULL, 16); 
		ui8_t sp = (ui8_t)strtol(line.substr(71, 2).c_str(), NULL, 16); 
		ui32_t cyc = (ui32_t)strtol(line.substr(90, line.length()-90).c_str(), NULL, 10);
		
		bool r = _cpu->compare_state(pc, a, x, y, s, sp, cyc);
		if(!r) {
			return false;
		}
		_cpu->clock();
		
	}
	
	
	return true;
}
