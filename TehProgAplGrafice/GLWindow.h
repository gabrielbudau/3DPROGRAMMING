#ifndef GL_WINDOW_H
#define GL_WINDOW_H
#include <glew.h>
#include <freeglut.h>
#include <iostream>
using namespace std;

extern const char* vertexShaderCode;
extern const char* fragmentShaderCode;
void paintGL(void);

class GLWindow
{
protected:
	void initGL();
	void sendDataToOpenGL();
	void installShaders();
public:
	GLWindow(int argc, char** argv);
	~GLWindow();
	
};

#endif
