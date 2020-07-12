#include "nes_test.hpp"

nes_test::nes_test(){	
	char* file_name = (char*)"resources/nestest.nes";
	cartridge* c = new cartridge(file_name);

	_console = new nes_console(c);
	_console->get_cpu()->setup_state(0xC000, 0x24, 0xFD, 7);
	_name = "nestest ROM";
	
}

nes_test::~nes_test(){	
}


bool nes_test::run_test(){
	
	
	bool verbose = true;
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
		
		bool r = _console->get_cpu()->compare_state(pc, a, x, y, s, sp, cyc);
		
		if(verbose){
			_console->get_cpu()->debug();
		}

		
		if(!r) {
			
			if(verbose){
				
				std::stringstream ss;
				ss << std::hex << pc;
				std::string debug_string = "[" + ss.str() + "]";
				
				ss.str(std::string());
				ss << std::hex << ((int)op);
				debug_string += " " + ss.str();
				
				ss.str(std::string());
				ss << std::hex << ((int)a);
				debug_string += " A:" + ss.str();
				
				ss.str(std::string());
				ss << std::hex << ((int)x);
				debug_string += " X:" + ss.str();
				
				ss.str(std::string());
				ss << std::hex << ((int)y);
				debug_string += " Y:" + ss.str();

				
				ss.str(std::string());
				ss << std::hex << (int)s;
				debug_string += " P:" + ss.str();
				
				ss.str(std::string());
				ss << std::hex << ((int)sp);
				debug_string += " SP:" + ss.str();
				

				std::cout << debug_string
				<< " CYC:" << ((int)cyc)
				<< " \t "
				<< " N:" << int((s & 0x80) != 0)
				<< " V:" << int((s & 0x40) != 0)
				<< " U:" << int((s & 0x20) != 0)
				<< " B:" << int((s & 0x10) != 0)
				<< " D:" << int((s & 0x08) != 0)
				<< " I:" << int((s & 0x04) != 0)
				<< " Z:" << int((s & 0x02) != 0)
				<< " C:" << int((s & 0x01) != 0)
				<< std::endl;
				
			}
			

			
			
			
			return false;
		}
		
		while(!_console->get_cpu()->clock()){};

		
	}
	
	
	return true;
}
