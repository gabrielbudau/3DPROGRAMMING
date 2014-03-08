#ifndef GLENGINE_H
#define GLENGINE_H

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include "Texture.h"
#include "Camera.h"
#include "Light.h"

#define iGLEngine GLEngine::getEngine()

class GLEngine
{
private:
	GLuint			fontBase;

	Texture			*fontTexture;
public:
	GLEngine();
	~GLEngine();

	static GLvoid Uninitialize(GLvoid);
	static GLEngine* getEngine(GLvoid);

	GLvoid initialize(GLint, GLint);

	GLvoid buildTextureFont(GLvoid);
	GLvoid drawText(GLint x, GLint y, const char *text,  ...);

	GLuint getTextWidth(const char*);
	GLuint getTextHeight(const char*);
};

#endif
