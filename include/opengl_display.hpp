#pragma once
#include "yane.hpp"
#include "i_display.hpp"

#define GL_DISPLAY_WIDTH 256
#define GL_DISPLAY_HEIGHT 240


class openGL_display : public i_display
{
public:
	
	GLfloat* _pal_screen_colors;
	GLfloat* _screen_points;
	GLfloat* _screen_frame;
	
public:
	

	openGL_display();
	~openGL_display();
	
	void init();
	void notify_pixel(int x, int y, ui8_t color_index);


public:

	void draw_buffer();

};

