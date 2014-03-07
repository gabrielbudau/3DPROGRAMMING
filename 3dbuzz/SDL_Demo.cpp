#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Texture.h"
#include "Camera.h"
#include "Light.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <glew.h>
#include <freeglut.h>


GLfloat cubeRotateX = 45.0f;
GLfloat cubeRotateY = 45.0f;
const GLsizei windowWidth = 500;
const GLsizei windowHeight = 500;
const Uint8 *keys = NULL;
SDL_Window *screen;
SDL_GLContext glContext;
const unsigned char *version;

CCamera		Camera;
Texture		*texture1	= NULL;
Texture		*texture2	= NULL;
Texture		*texture3	= NULL;
int			*seed		= NULL;

GLvoid establishProjectionMatrix(GLsizei, GLsizei);
GLvoid initGL(GLsizei, GLsizei);
GLvoid drawScene(GLvoid);
GLboolean checkKeys(GLvoid);
GLvoid displayFPS(GLvoid);
GLvoid drawGrid(GLvoid);

int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf_s(stderr, "Unable to initialize SDL:  %s", SDL_GetError());
		exit(1);
	}
	if ((screen = SDL_CreateWindow("My Game Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_SWSURFACE | SDL_WINDOW_OPENGL)) == NULL)
	{
		fprintf_s(stderr, "Unable to create window:  %s", SDL_GetError());
		exit(2);
	}
	if ((glContext = SDL_GL_CreateContext(screen)) == NULL)
	{
		fprintf_s(stderr, "Unable to create context:  %s", SDL_GetError());
		exit(3);
	}
	if ((version = glGetString(GL_VERSION)) == NULL)
	{
		exit(4);
	}

	SDL_GL_MakeCurrent(screen, glContext);

	initGL(windowWidth, windowHeight);

	seed = new int[Light::numLights];
	for (int i = 0; i < Light::numLights; i++)
	{
		Light *light = new Light(LIGHT_SPOT);

		float r = (float)rand() / (float)RAND_MAX;
		float g = (float)rand() / (float)RAND_MAX;
		float b = (float)rand() / (float)RAND_MAX;
		light->setDiffuse(r, g, b, 1.0);
		light->setAmbient(0, 0, 0, 1);
		light->setSpotDirection(0, -1, 0);

		float cutoff = 20.0f + ((float)rand() / (float)RAND_MAX*60.0f);

		light->setCutOff(cutoff);
		light->setExponent(20.0 * cutoff);
		seed[i] = rand();
	}
	int done = 0;

	while (!done)
	{

		drawScene();
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				done = 1;
			}

			keys = SDL_GetKeyboardState(NULL);
		}

		if (checkKeys())
		{
			done = 1;

		}
		//SDL_Delay(1);
	}

	for (int i = 0; i < (int)Light::lights.size(); i ++)
	{
		delete Light::lights[i];
	}
	delete[] seed;
	SDL_GL_DeleteContext(glContext);
	SDL_Quit();
	return 1;
}


GLvoid establishProjectionMatrix(GLsizei Width, GLsizei Height)
{
	glViewport(0, 0, Width, Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 200.0f);
}
GLvoid initGL(GLsizei _Width, GLsizei _Height)
{
	Light::Initialise();
	establishProjectionMatrix(_Width, _Height);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_PERSPECTIVE_CORRECTION_HINT);
	//glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	
	texture1 = new Texture("..\\Resources\\Textures\\stoneWall_1.tga", "Surface Texture");
	texture2 = new Texture("..\\Resources\\Textures\\crate_1.tga", "Surface Texture");
	texture3 = new Texture("..\\Resources\\Textures\\crate_2.tga", "Surface Texture");
	glBindTexture(GL_TEXTURE_2D, texture3->texID);

	//Camera.Move(F3dVector(0.0, 0.0, 3.0));
	//Camera.MoveForward(1.0);



}
GLvoid drawScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Camera.Render();

	glTranslatef(0.0, 0.0, -80.0f);
	glRotatef(cubeRotateX, 1, 0, 0);
	glRotatef(cubeRotateY, 0, 1, 0);
	
	for (int i = 0; i < (int)Light::lights.size(); i++)
	{
		double randomNumber = (double)SDL_GetTicks() + seed[i];
		float randX = (float)sin(randomNumber / 1600.0f) * (float)cos(randomNumber / 1200.0f) * 50.0f;
		float randY = (float)sin(randomNumber / 900.0f) * (float)cos(randomNumber / 1400.0f) * 50.0f;
		Light::lights[i]->setPosition(randX, 30.0f, randY);
		Light::lights[i]->updateLight();
	}
	
	drawGrid();

	glFlush();
	//this line is modified in other window interfaces
	SDL_GL_SwapWindow(screen);
	displayFPS();
}
GLboolean checkKeys(GLvoid)
{
	static long lastTime = SDL_GetTicks();
	const GLfloat speed = 1.0f;
	const long updateTime = 10;
	if (keys[SDL_GetScancodeFromKey(SDLK_ESCAPE)])
	{
		return true;
	}

	long newTime = SDL_GetTicks();

	if (newTime - lastTime > updateTime){
		if (keys[SDL_GetScancodeFromKey(SDLK_LEFT)])
		{
			cubeRotateY -= speed;
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_RIGHT)])
		{
			cubeRotateY += speed;
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_UP)])
		{
			cubeRotateX -= speed;
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_DOWN)])
		{
			cubeRotateX += speed;
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_1)])
		{
			glBindTexture(GL_TEXTURE_2D, texture1->texID);
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_2)])
		{
			glBindTexture(GL_TEXTURE_2D, texture2->texID);
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_3)])
		{
			glBindTexture(GL_TEXTURE_2D, texture3->texID);
		}
		/*
		CAMERA movement :
		w		: forwards
		s		: backwards
		a		: turn left
		d		: turn right
		x		: turn up
		y		: turn down
		v		: strafe right
		c		: strafe left
		r		: move up
		f		: move down
		m / n	: roll
		*/
		if (keys[SDL_GetScancodeFromKey(SDLK_a)])
		{
			Camera.RotateY(5.0);
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_d)])
		{
			Camera.RotateY(-5.0);
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_w)])
		{
			Camera.MoveForward(-0.1);
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_s)])
		{
			Camera.MoveForward(0.1);
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_x)])
		{
			Camera.RotateX(5.0);
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_y)])
		{
			Camera.RotateX(-5.0);
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_c)])
		{
			Camera.StrafeRight(-0.1);
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_v)])
		{
			Camera.StrafeRight(0.1);
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_f)])
		{
			Camera.MoveUpward(-0.3);
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_r)])
		{
			Camera.MoveUpward(0.3);
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_m)])
		{
			Camera.RotateZ(-5.0);
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_n)])
		{
			Camera.RotateZ(5.0);
		}

	}

	return false;
}
GLvoid displayFPS(GLvoid)
{

	static long lastTime = SDL_GetTicks();
	static long loops = 0;
	static GLfloat FPS = 0.0f;
	GLint newtime = SDL_GetTicks();
	if (newtime - lastTime > 100)
	{
		GLfloat newFPS = (GLfloat)loops / (GLfloat)(newtime - lastTime) * 1000.0f;
		FPS = (FPS + newFPS) / 2.0f;
		char title[80];
		sprintf_s(title, "OpenGL Demo - %.2f", FPS);
		SDL_SetWindowTitle(screen, title);
		lastTime = newtime;
		loops = 0;
	}
	loops++;
}
GLvoid drawGrid(GLvoid)
{
	const float w = 80.0f;
	const float h = 80.0f;
	const int divisions = 100;

	float incX = w / (float)divisions;
	float incY = h / (float)divisions;
	glColor3f(0, 0, 0);
	glNormal3f(0, 1, 0);
	
	for (float x = -w/2; x < w/2; x+= incX)
	{
		for (float y = -h / 2; y < h / 2; y += incY)
		{
			glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(x + incX,	0, y + incY);
			glVertex3f(x,			0, y + incY);
			glVertex3f(x + incX,	0, y);
			glVertex3f(x,			0, y);
			glEnd();
		}
	}
}