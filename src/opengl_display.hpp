#pragma once
#include "yane.hpp"
#include "i_display.hpp"

#define GL_DISPLAY_WIDTH 256
#define GL_DISPLAY_HEIGHT 240


class openGL_display : public i_display
{
public:
	
	int _argc;
	char** _argv;
	
	GLfloat* _pal_screen_colors;
	
	GLfloat* _screen_points;
	GLfloat* _screen_frame;
	
	GLfloat* _white_noise_frame;
	
public:
	

	openGL_display(int argc, char** argv);
	~openGL_display();
	
	void init();
	void notify_pixels(GLfloat* frame);


private:

	void draw_buffer();

};

