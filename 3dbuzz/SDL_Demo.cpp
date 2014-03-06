#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

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
GLvoid establishProjectionMatrix(GLsizei, GLsizei);
GLvoid initGL(GLsizei, GLsizei);
GLvoid drawScene(GLvoid);
GLboolean checkKeys(GLvoid);
GLvoid displayFPS(GLvoid);
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
		SDL_Delay(1);
	}

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
	establishProjectionMatrix(_Width, _Height);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_PERSPECTIVE_CORRECTION_HINT);
}
GLvoid drawScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0f);
	glRotatef(cubeRotateX, 1, 0, 0);
	glRotatef(cubeRotateY, 0, 1, 0);
	//Draw Cube;
	glBegin(GL_QUADS);
	//top
	glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(1.0, 1.0, -1.0f);
	glVertex3f(-1.0, 1.0, -1.0f);
	glVertex3f(-1.0, 1.0, 1.0f);
	glVertex3f(1.0, 1.0, 1.0f);
	//bottom
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0, -1.0, -1.0f);
	glVertex3f(-1.0, -1.0, -1.0f);
	glVertex3f(-1.0, -1.0, 1.0f);
	glVertex3f(1.0, -1.0, 1.0f);
	//front
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0, 1.0, 1.0f);
	glVertex3f(-1.0, 1.0, 1.0f);
	glVertex3f(-1.0, -1.0, 1.0f);
	glVertex3f(1.0, -1.0, 1.0f);
	//back
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0, 1.0, -1.0f);
	glVertex3f(-1.0, 1.0, -1.0f);
	glVertex3f(-1.0, -1.0, -1.0f);
	glVertex3f(1.0, -1.0, -1.0f);
	//left
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-1.0, 1.0, 1.0f);
	glVertex3f(-1.0, 1.0, -1.0f);
	glVertex3f(-1.0, -1.0, -1.0f);
	glVertex3f(-1.0, -1.0, 1.0f);
	//right
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.0, 1.0, 1.0f);
	glVertex3f(1.0, 1.0, -1.0f);
	glVertex3f(1.0, -1.0, -1.0f);
	glVertex3f(1.0, -1.0, 1.0f);
	glEnd();


	glFlush();
	//this line is modified in other window interfaces
	SDL_GL_SwapWindow(screen);
	displayFPS();
}
GLboolean checkKeys(GLvoid)
{
	static long lastTime = SDL_GetTicks();
	const GLfloat speed = 1.0f;
	const long updateTime = 100;
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