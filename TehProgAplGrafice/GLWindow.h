#ifndef GL_WINDOW_H
#define GL_WINDOW_H
#include <glew.h>
#include <freeglut.h>
#include <iostream>
#include <fstream>
#include <glm.hpp>
#include <Vertex.h>
#include <ShapeGenerator.h>

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

using namespace std;
using namespace glm;


void paintGL(void);

class GLWindow
{
protected:
	void initGL();
	void sendDataToOpenGL();
	void installShaders();
	bool checkShaderStatus(GLuint shaderID);
	bool checkProgramStatus(GLuint programID);
	bool checkStatus(GLuint objectID,
		PFNGLGETSHADERIVPROC objectPropertyGetter,
		PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
		GLenum statusType
		);
	string readVertexShaderCode(const char* fileName);
public:
	
	GLWindow(int argc, char** argv);
	~GLWindow();
	
};

#endif
