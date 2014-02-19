#ifndef GL_WINDOW_H
#define GL_WINDOW_H
#include <glew.h>
#include <freeglut.h>

void paintGL(void);

class GLWindow
{
protected:
	static void initGL();
public:
	GLWindow(int argc, char** argv);
	~GLWindow();
	
};

#endif
