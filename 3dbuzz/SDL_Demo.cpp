#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "GLEngine.h"
#include "Button.h"
#include "ListBox.h"
#include "Slider.h"
#include "Emitter.h"
#include <glew.h>
#include <freeglut.h>


GLfloat rotateX = 45.0f;
GLfloat rotateY = 45.0f;
GLfloat zoom = -5.0f;

const float rotateXSpeed = 0.5f;
const float rotateYSpeed = 0.5f;
const float zoomSpeed = 0.01f;

const GLsizei windowWidth = 1000;
const GLsizei windowHeight = 1000;
const Uint8 *keys = NULL;
SDL_Window *screen;
SDL_GLContext glContext;
const unsigned char *version;

GLvoid establishProjectionMatrix(GLsizei, GLsizei);
GLvoid initGL(GLsizei, GLsizei);
GLvoid drawScene(GLvoid);
GLboolean updateNavigation(GLvoid);
GLvoid displayFPS(GLvoid);
GLvoid setOrtho(GLsizei, GLsizei);
GLvoid drawControls(GLvoid);

CCamera Camera;
Texture *texture1 = NULL;
Light	*light = NULL;


bool navigating = false;
Control *controlled = NULL;
MouseState state;
float testValue = 3.0f;

int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf_s(stderr, "Unable to initialize SDL:  %s", SDL_GetError());
		exit(1);
	}
	if ((screen = SDL_CreateWindow("My Game Window", 10, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_SWSURFACE | SDL_WINDOW_OPENGL)) == NULL)
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
		SDL_GetMouseState(&state.x, &state.y);

		state.LeftButtonDown = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1);
		state.MiddleButtonDown = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(2);
		state.RightButtonDown = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(3);

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

		if (updateNavigation())
		{
			done = 1;

		}
		SDL_Delay(1);
	}

	for (list<Control *>::iterator it = Control::controls.begin(); it != Control::controls.end(); it++)
	{
		delete (*it);
		
	}
	GLEngine::Uninitialize();
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
	iGLEngine->initialize(_Width, _Height);

	establishProjectionMatrix(_Width, _Height);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_PERSPECTIVE_CORRECTION_HINT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	light = new Light(LIGHT_SPOT);
	light->setDiffuse(2.0, 2.0, 2.0, 1.0);
	light->setPosition(0, 15, 0);



	texture1 = new Texture("..\\Resources\\Textures\\stoneWall_1.tga", "Surface Texture");
	
	addControl(new Button("Run away!", 0, 0, 200, 50));
	ListBox *lstBox = (ListBox*)addControl(new ListBox(0, 0, 200, 200));

	lstBox->addItem("Buzz");
	lstBox->addItem("Joel");
	lstBox->addItem("Logan");
	lstBox->addItem("Angela");

	Slider *testSlider = (Slider*)addControl(new Slider("Test Value", -5.0f, 5.0f, 0, 0, 300, 50));

	testSlider->setValue(&testValue);

}
GLvoid drawScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//PASS 1- persp
	glBindTexture(GL_TEXTURE_2D, texture1->texID);
	establishProjectionMatrix(windowWidth, windowHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_LIGHTING);
	//Camera.Render();

	glDisable(GL_BLEND);

	glTranslatef(0.0, 0.0, zoom);
	glRotatef(rotateX, 1, 0, 0);
	glRotatef(rotateY, 0, 1, 0);
	for (int i = 0; i < (int)Light::lights.size(); i++)
	{
		Light::lights[i]->updateLight();
	}

	glColor3f(1.0f, 1.0f, 1.0f);

	//Draw Cube;
	glBegin(GL_QUADS);
	//top
	glNormal3f(0, 1, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0, 1.0, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0, 1.0, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0, 1.0, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0, 1.0, 1.0f);
	//bottom
	glNormal3f(0, -1, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0, -1.0, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0, -1.0, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0, -1.0, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0, -1.0, 1.0f);
	//front
	glNormal3f(0, 0, 1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0, 1.0, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0, 1.0, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0, -1.0, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0, -1.0, 1.0f);
	//back
	glNormal3f(0, 0, -1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0, 1.0, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0, 1.0, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0, -1.0, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0, -1.0, -1.0f);
	//left
	glNormal3f(1, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0, 1.0, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0, 1.0, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0, -1.0, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0, -1.0, 1.0f);
	//right
	glNormal3f(-1, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0, 1.0, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0, 1.0, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0, -1.0, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0, -1.0, 1.0f);
	glEnd();

	//PASS 2 - ortho
	glDisable(GL_LIGHTING);
	setOrtho(windowWidth, windowHeight);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	displayFPS();
	drawControls();
	glFlush();
	//this line is modified in other window interfaces
	SDL_GL_SwapWindow(screen);
}
GLboolean updateNavigation(GLvoid)
{
	static int lastX = -1;
	static int lastY = -1;
	if (lastX == -1 && lastY == -1)
	{
		lastX = state.x;
		lastY = state.y;
	}

	int changeX = lastX - state.x;
	int changeY = lastY - state.y;

	lastX = state.x;
	lastY = state.y;

	if (state.LeftButtonDown && controlled == NULL)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		rotateX -= (float)changeY * rotateXSpeed;
		rotateY -= (float)changeX * rotateYSpeed;

		navigating = true;
	}
	else if (state.RightButtonDown && controlled == NULL)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		zoom -= (float)changeX * zoomSpeed;

		navigating = true;
	}
	else{
		SDL_SetRelativeMouseMode(SDL_FALSE);
		navigating = false;
	}

	if (keys[SDL_GetScancodeFromKey(SDLK_ESCAPE)])
	{
		return true;
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
		lastTime = newtime;
		loops = 0;
	}

	iGLEngine->drawText(5, 5, "FPS: - %.2f\n ", FPS);

	loops++;
}
GLvoid setOrtho(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);
}
GLvoid drawControls(GLvoid)
{
	for (list<Control *>::iterator it = Control::controls.begin(); it != Control::controls.end(); it++)
	{
		Control *control = (*it);
		control->drawControl();

		if ((controlled != NULL && controlled != control) || navigating)
			continue;
		if (control->updateControl(state))
		{
			controlled = control;

			// Handle events
		}
		else if (control == controlled)
		{
			controlled = NULL;
		}
		else
		{
			//Control has been updated ... but not messages 
		}
	}
}