#ifndef GL_WINDOW_H
#define GL_WINDOW_H
#include <glew.h>
#include <freeglut.h>
#include <iostream>
#include <fstream>
#include <glm.hpp>
#include <Vertex.h>
#include <ShapeGenerator.h>

using namespace std;
using namespace glm;

extern const char* vertexShaderCode;
extern const char* fragmentShaderCode;
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
