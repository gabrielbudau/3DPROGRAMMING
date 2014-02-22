#ifndef GL_WINDOW_H
#define GL_WINDOW_H
#include <glew.h>
#include <freeglut.h>
#include <iostream>
#include <fstream>
#include <glm.hpp>
#include <gtx\transform.hpp>
#include <gtc\matrix_transform.hpp>
#include <Vertex.h>
#include <ShapeGenerator.h>
#include <Camera.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 400

using namespace std;
using namespace glm;

void paintGL(void);
void reshape(int w, int h);
void mousePasiveMotion(int x, int y);
void mouseActiveMotion(int x, int y);
void keyPressed(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void keyOperations(void);
void keySpecial(int key, int x, int y);
void keySpecialUp(int key, int x, int y);
void keySpecialOperations(void);
void initBoolArray(bool *ba);
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
