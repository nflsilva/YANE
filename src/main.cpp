#include "yane.hpp"

#include "nes_console.hpp"
#include "opengl_display.hpp"

void run_nestest_cpu(nes_console* console);
void run_nestest_ppu();




void draw_square(float* frame, int x, int y, float* color){
	
	int base_x = x * 4;
	int base_y = y * GL_DISPLAY_WIDTH * 4;
	
	for(int fr = 0; fr < 8; fr++){
		int offset_y = fr * GL_DISPLAY_WIDTH * 4;
		for(int fc = 0; fc < 8; fc++){
			
			int offset_x = fc * 4;
			
			frame[base_x + offset_x + base_y + offset_y + 0] = color[0];
			frame[base_x + offset_x + base_y + offset_y + 1] = color[1];
			frame[base_x + offset_x + base_y + offset_y + 2] = color[2];
			frame[base_x + offset_x + base_y + offset_y + 3] = 1.0;
		}
	}
}
void draw_palette(int palette_number, int y, float* frame, float* colors){
	for(int p = 0; p < 4; p++){
		draw_square(frame, (p + 1) * 10.0 + palette_number * 50.0, y,  	&colors[p * 3]);
	}
}
void draw_square_in_screen_tile(float* frame, int tile_x, int tile_y, float* color){
	draw_square(frame, tile_x * 8, tile_y * 8, color);
}
void tick(nes_console* console){
	while(1){
		console->clock();
		std::this_thread::sleep_for(std::chrono::nanoseconds(600 / 16));
	};
}


int main(int argc, char **argv){
	
	
	char* file_name = (char*)"resources/nestest.nes";
	cartridge* cart = new cartridge(file_name);
	nes_console* console = new nes_console(cart);
	
	
	
	//run_nestest_cpu(console);
	
	openGL_display* display = new openGL_display();
	display->init();
	
	console->get_cpu()->reset();
	while(1){
		console->clock();
		//console->get_cpu()->debug();
		
		if(console->_ppu->is_visible()){
			
			ui16_t x = console->_ppu->_current_cycle;
			ui16_t y = console->_ppu->_current_scanline;
			ui8_t c = console->_ppu->_color_index;
			/*
			std::cout << (int)x;
			std::cout << " ";
			std::cout << (int)y;
			std::cout << std::endl;
			*/
			display->notify_pixel(x, y, c);
			
			if(console->_ppu->completed_frame()){
				display->draw_buffer();
				
				/*
				for(int p = 0; p < 8; p++){
					cout << hex << (int)console->_ppu_bus->read(0x3F00 + p * 4 + 0);
					cout << " ";
					cout << hex << (int)console->_ppu_bus->read(0x3F00 + p * 4 + 1);
					cout << " ";
					cout << hex << (int)console->_ppu_bus->read(0x3F00 + p * 4 + 2);
					cout << " ";
					cout << hex << (int)console->_ppu_bus->read(0x3F00 + p * 4 + 3);
					cout << " | " << endl;
				}
				
				for(int r = 0; r < 32; r++){
					for(int c = 0; c < 32; c++){
						cout << hex << (int)console->_ppu_bus->read(0x2000 + r * 32 + c);
					}
					cout << endl;
				}
				
				
				cin.ignore();*/
				
			}
		}
		//std::this_thread::sleep_for(std::chrono::nanoseconds(1));
	};
	
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

