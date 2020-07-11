#include "opengl_display.hpp"


static GLuint create_shader_program();

openGL_display::openGL_display(){

}

openGL_display::~openGL_display(){
}





void openGL_display::init(){
	
	int argc = 1;
	char * argv[1] = { "a" };
	glutInit(&argc, argv);
	glutCreateWindow("YANE");
	
	GLenum err = glewInit();
	if (GLEW_OK != err){
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	_screen_points = new GLfloat[16] {
		-1.0f, -1.0f, 	0.0f, 1.0f,
		 1.0f, -1.0f,	1.0f, 1.0f,	
		-1.0f,  1.0f,	0.0f, 0.0f,
		 1.0f,  1.0f,	1.0f, 0.0f
	};
	_screen_frame = new GLfloat[GL_DISPLAY_WIDTH * GL_DISPLAY_HEIGHT * 4];
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
	
	GLuint shader_program = create_shader_program();
	glUseProgram(shader_program);
	
}

void openGL_display::draw_buffer(){
	
	GLuint texture;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GL_DISPLAY_WIDTH, GL_DISPLAY_HEIGHT, 0, GL_RGBA, GL_FLOAT, _screen_frame);
	
	
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 16, _screen_points, GL_STREAM_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, 0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (GLvoid*)(sizeof(GLfloat) * 2));
	


	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glutSwapBuffers();
	
	
	glDeleteBuffers(1, &buffer);
	glDeleteTextures(1, &texture);
	
	
}

void openGL_display::notify_pixel(int x, int y, ui8_t color_index){
	
	GLfloat pal_color_r = _pal_screen_colors[color_index * 3 + 0];
	GLfloat pal_color_g = _pal_screen_colors[color_index * 3 + 1];
	GLfloat pal_color_b = _pal_screen_colors[color_index * 3 + 2];
	
	_screen_frame[x * 4 + y * GL_DISPLAY_WIDTH * 4 + 0] = pal_color_r / 255;
	_screen_frame[x * 4 + y * GL_DISPLAY_WIDTH * 4 + 1] = pal_color_g / 255;
	_screen_frame[x * 4 + y * GL_DISPLAY_WIDTH * 4 + 2] = pal_color_b / 255;
	_screen_frame[x * 4 + y * GL_DISPLAY_WIDTH * 4 + 3] = 1.0;
	
}



static std::string read_file(const char *filePath) {
	
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}
static GLuint compile_shader(GLuint shader_type, const char *file_path) {
	
    GLuint shader_id = glCreateShader(shader_type);

    // Read shaders
    std::string shader_code = read_file(file_path);
	
    const char *shader_src = shader_code.c_str();

	glShaderSource(shader_id, 1, &shader_src, nullptr);
	glCompileShader(shader_id);

	GLint compile_result;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_result);
	if(compile_result == GL_FALSE){
		
		GLint log_length;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
		char* log = (char*) alloca(log_length * sizeof(char));
		glGetShaderInfoLog(shader_id, log_length, &log_length, log);
		std::cout << "Error compiling"
		<< (shader_type == GL_VERTEX_SHADER ? " vertex shader " : " fragment shader ")
		<< "shader." << std::endl;
		
		std::cout << "Log: " << log << std::endl;
	}
	
    return shader_id;
}
static GLuint create_shader_program(){
	
	GLuint program = glCreateProgram();
	GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, "shader/vertex_shader.glsl");
	GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, "shader/fragment_shader.glsl");
	
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	glValidateProgram(program);
	
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	
	return program;
}