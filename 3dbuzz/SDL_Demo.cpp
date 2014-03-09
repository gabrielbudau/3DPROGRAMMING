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
GLfloat zoom = -100.0f;

const float rotateXSpeed = 0.5f;
const float rotateYSpeed = 0.5f;
const float zoomSpeed = 0.01f;

const GLsizei windowWidth = 800;
const GLsizei windowHeight = 650;
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
GLvoid drawGrid(GLvoid);
GLvoid updateControls(GLvoid);
GLvoid initializeControls();
GLvoid handleEvent(Control *control);
Light	*light = NULL;
bool navigating = false;
Control *controlled = NULL;
MouseState state;

vector<Texture *>textureList;
vector<Emitter *>emitterList;

ListBox *lstTextures		= NULL;
ListBox *lstEmitters		= NULL;

Slider *slidLife			= NULL;
Slider *slidLifeRange		= NULL;
Slider *slidSize			= NULL;
Slider *slidSizeRange		= NULL;
Slider *slidSpread			= NULL;
Slider *slidSaturation		= NULL;
Slider *slidEmissionRadius	= NULL;
Slider *slidEmissionRate	= NULL;
Slider *slidGravity			= NULL;
Slider *slidOffsetX			= NULL;
Slider *slidOffsetY			= NULL;
Slider *slidOffsetZ			= NULL;
Slider *slidVortex			= NULL;
Slider *slidAlpha			= NULL;

int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf_s(stderr, "Unable to initialize SDL:  %s", SDL_GetError());
		exit(1);
	}
	if ((screen = SDL_CreateWindow("My Game Window", 10, 50, windowWidth, windowHeight, SDL_SWSURFACE | SDL_WINDOW_OPENGL)) == NULL)
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
		//SDL_Delay(1);
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
	light->setDiffuse(1.0, 1.0, 1.0, 1.0);
	light->setPosition(0, 50, 0);

	initializeControls();

}
GLvoid drawScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//PASS 1- persp
	
	establishProjectionMatrix(windowWidth, windowHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(0.0, 0.0, zoom);
	glRotatef(rotateX, 1, 0, 0);
	glRotatef(rotateY, 0, 1, 0);
	for (int i = 0; i < (int)Light::lights.size(); i++)
	{
		Light::lights[i]->updateLight();
	}

	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	drawGrid();
	glEnable(GL_BLEND);
	
	for (int i = 0; i < (int)emitterList.size(); i++)
	{
		emitterList[i]->Update(SDL_GetTicks());
	}

	//PASS 2 - ortho
	glColor3f(1.0f, 1.0f, 1.0f);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
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

		if (rotateX < 0.0f)
			rotateX = 1.0f;

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

	iGLEngine->drawText(205, 5, "fps:%.2f\n ", FPS);

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

			handleEvent(control);
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
GLvoid drawGrid(GLvoid)
{
	const float w = 40.0f;
	const float h = 40.0f;
	const int divisions = 50;

	float incX = w / (float)divisions;
	float incY = h / (float)divisions;
	glColor4f(0, 0, 0, 1);
	glNormal3f(0, 1, 0);

	for (float x = -w / 2; x < w / 2; x += incX)
	{
		for (float y = -h / 2; y < h / 2; y += incY)
		{
			glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(x + incX, 0, y + incY);
			glVertex3f(x, 0, y + incY);
			glVertex3f(x + incX, 0, y);
			glVertex3f(x, 0, y);
			glEnd();
		}
	}
}
GLvoid updateControls(GLvoid)
{
	int currentEmitter = lstEmitters->getIndex();
	slidLife->setValue(&emitterList[currentEmitter]->life);
	slidLifeRange->setValue(&emitterList[currentEmitter]->lifeRange);
	slidSize->setValue(&emitterList[currentEmitter]->size);
	slidSizeRange->setValue(&emitterList[currentEmitter]->sizeRange);
	slidSpread->setValue(&emitterList[currentEmitter]->spread);
	slidSaturation->setValue(&emitterList[currentEmitter]->saturation);
	slidEmissionRadius->setValue(&emitterList[currentEmitter]->emissionRadius);
	slidEmissionRate->setValue(&emitterList[currentEmitter]->emissionRate);
	slidGravity->setValue(&emitterList[currentEmitter]->gravity);
	slidOffsetX->setValue(&emitterList[currentEmitter]->position.x);
	slidOffsetY->setValue(&emitterList[currentEmitter]->position.y);
	slidOffsetZ->setValue(&emitterList[currentEmitter]->position.z);
	slidVortex->setValue(&emitterList[currentEmitter]->rotation.y);
	slidAlpha->setValue(&emitterList[currentEmitter]->alpha);
}
GLvoid initializeControls()
{
	textureList.push_back(new Texture("..\\Resources\\Textures\\fire_2.tga", "Star"));
	textureList.push_back(new Texture("..\\Resources\\Textures\\flare.tga", "Flare"));
	textureList.push_back(new Texture("..\\Resources\\Textures\\fire_2.tga", "Fire"));
	textureList.push_back(new Texture("..\\Resources\\Textures\\smoke.tga", "Smoke"));

	Emitter *emitter = new Emitter();
	emitterList.push_back(emitter);
	emitter->setTexture(textureList[0]);
	slidLife			= (Slider*)addControl(new Slider("Life", 0.0f, 10.0f, 0, 0, 200, 20));
	slidLifeRange		= (Slider*)addControl(new Slider("Life Range", 0.0f, 3.0f, 0, 0, 200, 20));
	slidSize			= (Slider*)addControl(new Slider("Size", 0.0f, 10.0f, 0, 0, 200, 20));
	slidSizeRange		= (Slider*)addControl(new Slider("Size Range", 0.0f, 5.0f, 0, 0, 200, 20));
	slidSpread			= (Slider*)addControl(new Slider("Spread", 0.0f, 10.0f, 0, 0, 200, 20));
	slidSaturation		= (Slider*)addControl(new Slider("Saturation", 0.0f, 1.0f, 0, 0, 200, 20));
	slidEmissionRadius	= (Slider*)addControl(new Slider("Emission Radius", 0.0f, 10.0f, 0, 0, 200, 20));
	slidEmissionRate	= (Slider*)addControl(new Slider("Emission rate", 0.0f, 1000.0f, 0, 0, 200, 20));
	slidGravity			= (Slider*)addControl(new Slider("Gravity", 0.0f, 20.0f, 0, 0, 200, 20));
	slidOffsetX			= (Slider*)addControl(new Slider("Offset X", -15.0f, 15.0f, 0, 0, 200, 20));
	slidOffsetY			= (Slider*)addControl(new Slider("Offset Y",   0.0f, 15.0f, 0, 0, 200, 20));
	slidOffsetZ			= (Slider*)addControl(new Slider("Offset Z", -15.0f, 15.0f, 0, 0, 200, 20));
	slidVortex			= (Slider*)addControl(new Slider("Vortex", -50.0f, 50.0f, 0, 0, 200, 20));
	slidAlpha			= (Slider*)addControl(new Slider("Alpha", 0.0f, 1.0f, 0, 0, 200, 20));

	lstTextures = (ListBox *)addControl(new ListBox(0, 0, 200, 100));
	for (int i = 0; i < (int)textureList.size(); i++)
	{
		lstTextures->addItem(textureList[i]->name);
	}

	lstEmitters = (ListBox *)addControl(new ListBox(0, 0, 200, 100));
	for (int i = 0; i < (int)emitterList.size(); i++)
	{
		char text[80];
		sprintf_s(text, "Emitter %d", i + 1);
		lstEmitters->addItem(text);
	}

	addControl(new Button("Add", 0, 0, 100, 20));
	addControl(new Button("Delete", 0, 0, 100, 20));

	updateControls();
}
GLvoid handleEvent(Control *control)
{
	if (control->getType() == "button")
	{
		Button *button = (Button*)control;

		if (button->getLabel() == "Add" && (int)emitterList.size() < 6)
		{
			Emitter *emitter = new Emitter();
			emitterList.push_back(emitter);

			emitter->setTexture(textureList[lstTextures->getIndex()]);

			char text[80];
			sprintf_s(text, "Emitter %d", (int)emitterList.size());
			lstEmitters->addItem(text);

		}

		else if (button->getLabel() == "Delete" && (int)emitterList.size() > 1){
			int i = 0;
			vector<Emitter *>::iterator it;
			for (it = emitterList.begin(); it != emitterList.end(); it++)
			{
				if (i == lstEmitters->getIndex())
				{
					break;
				}
				i++;
			}
			emitterList.erase(emitterList.begin() + i);

			lstEmitters->removeItem(lstEmitters->getIndex());
		}

	}
	else if (control == lstTextures)
	{
		Emitter *emitter = emitterList[lstEmitters->getIndex()];
		emitter->setTexture(textureList[lstTextures->getIndex()]);
	}
	else if (control == lstEmitters)
	{
		int currentEmitter = lstEmitters->getIndex();
		for (int i = 0; i < (int)textureList.size(); i++)
		{
			if (emitterList[currentEmitter]->texture == textureList[i])
			{
				lstTextures->setCurrent(i);
			}
		}
		updateControls();
		
	}
}