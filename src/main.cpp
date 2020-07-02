#include "yane.hpp"

#include "nes_console.hpp"
#include "opengl_display.hpp"

void run_nestest_cpu(nes_console* console);
void run_nestest_ppu();


void draw_square(float* frame, int x, int y, float* color){
	
	int base_x = x * 4;
	int base_y = y * GL_DISPLAY_WIDTH * 4;
	
	for(int fr = 0; fr < 10; fr++){
		for(int fc = 0; fc < 10; fc++){
			
			int offset_x = fr * 4;
			int offset_y = fc * GL_DISPLAY_WIDTH * 4;
			
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


int main(int argc, char **argv){
	
	openGL_display* display = new openGL_display(argc, argv);
	display->init();
	
	
	float* actual_frame = new float[GL_DISPLAY_WIDTH * GL_DISPLAY_HEIGHT * 4];
	//background
	for (int i = 0; i < GL_DISPLAY_HEIGHT; i++) {
		for (int j = 0; j < GL_DISPLAY_WIDTH * 4; j+=4) {
			actual_frame[i * GL_DISPLAY_WIDTH * 4 + j + 0] = 100.0 / 255.0;
			actual_frame[i * GL_DISPLAY_WIDTH * 4 + j + 1] = 100.0 / 255.0;
			actual_frame[i * GL_DISPLAY_WIDTH * 4 + j + 2] = 100.0 / 255.0;
			actual_frame[i * GL_DISPLAY_WIDTH * 4 + j + 3] = 1.0;
		}
	}
	
	float* colors = new float[12] {
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0, 
		1.0, 1.0, 1.0
	};
	
	char* file_name = (char*)"resources/nestest.nes";
	cartridge* cart = new cartridge(file_name);
	nes_console* console = new nes_console(cart);
	
	
	while(1){
		
		display->notify_pixels(actual_frame);
		/*
		colors[0] = display->_pal_screen_colors[console->_ppu_bus->read(0x3F00) * 3 + 0] / 255.0;
		colors[1] = display->_pal_screen_colors[console->_ppu_bus->read(0x3F00) * 3 + 1] / 255.0;
		colors[2] = display->_pal_screen_colors[console->_ppu_bus->read(0x3F00) * 3 + 2] / 255.0;
		draw_square(actual_frame, 10.0, 80.0, colors);
		*/
		int y = 40;
		for(int p = 0; p < 8; p++){
			
			if(p == 4) y-= 20;
			
			for(int pp = 0; pp < 4; pp++){
				int pp_offset = pp * 3; 
				colors[0 + pp_offset] = display->_pal_screen_colors[console->_ppu_bus->read(0x3F00 + pp + p * 4) * 3 + 0] / 255.0;
				colors[1 + pp_offset] = display->_pal_screen_colors[console->_ppu_bus->read(0x3F00 + pp + p * 4) * 3 + 1] / 255.0;
				colors[2 + pp_offset] = display->_pal_screen_colors[console->_ppu_bus->read(0x3F00 + pp + p * 4) * 3 + 2] / 255.0;
			}
			
			draw_palette(p % 4, y, actual_frame, colors);
			
			
		};
		
		//console->get_cpu()->debug();
		while(!console->clock()){
			
			
		};
		
	};
	
	printf("Done!\n");
	return 0;
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

