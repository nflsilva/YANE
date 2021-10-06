#include "../include/yane.hpp"
#include "nes_console.hpp"
#include <chrono>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define NES_VIDEO_WIDGTH 256
#define NES_VIDEO_HEIGHT 240

bool isRunning;

class PixelDisplay : public olc::PixelGameEngine
{
private:
	GLfloat* _pal_screen_colors;
	ui8_t* _frame;

public:
	PixelDisplay(ui8_t* frameColors)
	{
		sAppName = "Example";
		_frame = frameColors;
		_pal_screen_colors = new GLfloat[192] {
			84, 84, 84, 
			0, 30, 116,
			8, 16, 144,
			48, 0, 136,
			68, 0, 100,
			92, 0, 48,
			84, 4, 0,
			60, 24, 0,
			32, 42, 0,
			8, 58, 0,
			0, 64, 0,
			0, 60, 0,
			0, 50, 60,
			0, 0, 0,
			0, 0, 0,
			0, 0, 0,
			
			152, 150, 152,
			8, 76, 196,
			48, 50, 236,
			92, 30, 228,
			136, 20, 176,
			160, 20, 100,
			152, 34, 32,
			120, 60, 0,
			84, 90, 0,
			40, 114, 0,
			8, 124, 0,
			0, 118, 40,
			0, 102, 120,
			0, 0, 0,
			0, 0, 0,
			0, 0, 0,

			236, 238, 236,
			76, 154, 236,
			120, 124, 236,
			176, 98, 236,
			228, 84, 236,
			236, 88, 180,
			236, 106, 100,
			212, 136, 32,
			160, 170, 0,
			116, 196, 0,
			76, 208, 32,
			56, 204, 108,
			56, 180, 204,
			60, 60, 60,
			0, 0, 0,
			0, 0, 0,

			236, 238, 236,
			168, 204, 236,
			188, 188, 236,
			212, 178, 236,
			236, 174, 236,
			236, 174, 212,
			236, 180, 176,
			228, 196, 144,
			204, 210, 120,
			180, 222, 120,
			168, 226, 144,
			152, 226, 180,
			160, 214, 228,
			160, 162, 160,
			0, 0, 0,
			0, 0, 0,
		};

	}

	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		for (int x = 0; x < NES_VIDEO_WIDGTH; x++) {
			for (int y = 0; y < NES_VIDEO_HEIGHT; y++) {

				uint8_t color_index = _frame[x + y * NES_VIDEO_WIDGTH];

				uint8_t pal_color_r = _pal_screen_colors[color_index * 3 + 0];
				uint8_t pal_color_g = _pal_screen_colors[color_index * 3 + 1];
				uint8_t pal_color_b = _pal_screen_colors[color_index * 3 + 2];
				uint8_t a = 255;
				/*
				if((x % 8 == 0) || (y % 8 == 0)){
					pal_color_r = 255;
					pal_color_g = 255;
					pal_color_b = 255;
					a = 1;
				}
				*/
				Draw(x, y, olc::Pixel(pal_color_r, pal_color_g, pal_color_b, a));

			}	
		}
		return true;
	}
};

bool run_nestest(nes_console* console){

	console->get_cpu()->setup_state(0xC000, 0x24, 0xFD, 7);
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
		
		bool r = console->get_cpu()->compare_state(pc, a, x, y, s, sp, cyc);
		
		if(verbose){
			console->get_cpu()->debug();
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
		
		while(!console->get_cpu()->clock()){};

		
	}
	
	
	return true;

}


void consoleTick(nes_console* console, ui8_t* frame) {

	int ppu_clock_time_ms = 46 * 1000;

	while(isRunning){

		auto begin = std::chrono::high_resolution_clock::now();

		console->clock();

		if(console->_ppu->is_visible()) {
			u2c02_pixel pixel = console->_ppu->get_pixel();
			frame[pixel.x + pixel.y * NES_VIDEO_WIDGTH] = pixel.color_index;
		}

		auto end = std::chrono::high_resolution_clock::now();
		int frame_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
		int frame_delta = ppu_clock_time_ms - frame_time;

		//std::this_thread::sleep_for(std::chrono::microseconds(ppu_clock_time_ms));

	}

}

int main(int argc, char **argv){

	ui8_t* frame = new uint8_t[NES_VIDEO_WIDGTH * NES_VIDEO_HEIGHT];

	char* file_name = (char*)"resources/sl.nes";
	cartridge* cart = new cartridge(file_name);
	nes_console* console = new nes_console(cart);
	console->_cpu->reset();

	PixelDisplay display(frame);
	if (!display.Construct(NES_VIDEO_WIDGTH, NES_VIDEO_HEIGHT, 1, 1))
		return 0;

	isRunning = true;
	display.olc_UpdateWindowSize(NES_VIDEO_WIDGTH * 4, NES_VIDEO_HEIGHT * 4);
	std::thread console_thread (consoleTick, console, frame);

	display.Start();
	isRunning = false;
	console_thread.join();

	printf("Done!\n");
	return 0;
}


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
		
		bool r = console->get_cpu()->compare_state(pc, a, x, y, s, sp, cyc);
		
		console->get_cpu()->debug();
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

		while(!console->get_cpu()->clock()){
			
			
		};
		
	}

	
}

