#include <freeglut.h>
#include <stdio.h>
#include "Vector3.h"
#include <SDL.h>
#define GLUT_KEYS_ESCAPE		27

const GLsizei windowWidth = 500;
const GLsizei windowHeight = 500;
GLfloat cubeRotateX = 45.0f;
GLfloat cubeRotateY = 45.0f;
bool keys[256];
bool keysSpecial[256];

GLvoid establishProjectionMatrix(GLsizei, GLsizei);
GLvoid initGL(GLsizei, GLsizei);
GLvoid drawScene(GLvoid);
GLvoid timerLoop(GLint);
GLvoid keyboardCB(GLubyte, GLint, GLint);
GLvoid keyboardUpCB(GLubyte, GLint, GLint);
GLvoid keyboardSpecialCB(GLint, GLint, GLint);
GLvoid keyboardSpecialUpCB(GLint, GLint, GLint);
GLboolean checkKeys(GLvoid);
GLvoid displayFPS(GLvoid);
int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE);

	int windowID = glutCreateWindow("OpenGL cube Window");
	glutReshapeWindow(windowWidth, windowHeight);

	initGL(windowWidth, windowHeight);

	glutDisplayFunc(drawScene);
	glutKeyboardFunc(keyboardCB);
	glutKeyboardUpFunc(keyboardUpCB);
	glutSpecialFunc(keyboardSpecialCB);
	glutSpecialUpFunc(keyboardSpecialUpCB);
	glutTimerFunc(1, timerLoop, 0);

	glutMainLoop();

	return 0;
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
	glVertex3f( 1.0, 1.0, -1.0f);
	glVertex3f(-1.0, 1.0, -1.0f);
	glVertex3f(-1.0, 1.0,  1.0f);
	glVertex3f( 1.0, 1.0,  1.0f);
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
	glutSwapBuffers();
	displayFPS();
}
GLvoid timerLoop(GLint)
{
	if (checkKeys())
	{
		exit(0);
	}
	glutPostRedisplay();
	glutTimerFunc(1, timerLoop, 0);
}
GLvoid keyboardCB(GLubyte key, GLint x, GLint y)
{
	keys[key] = true;
}
GLvoid keyboardUpCB(GLubyte key, GLint x, GLint y)
{
	keys[key] = false;
}
GLvoid keyboardSpecialCB(GLint key, GLint x, GLint y)
{
	keysSpecial[key] = true;
}
GLvoid keyboardSpecialUpCB(GLint key, GLint x, GLint y)
{
	keysSpecial[key] = false;
}
GLboolean checkKeys(GLvoid)
{
	const GLfloat speed = 1.0f;
	if (keys[GLUT_KEYS_ESCAPE])
	{
		return true;
	}

	if (keysSpecial[GLUT_KEY_LEFT])
	{
		cubeRotateY -= speed;
	}
	if (keysSpecial[GLUT_KEY_RIGHT])
	{
		cubeRotateY += speed;
	}

	if (keysSpecial[GLUT_KEY_UP])
	{
		cubeRotateX -= speed;
	}
	if (keysSpecial[GLUT_KEY_DOWN])
	{
		cubeRotateX += speed;
	}
	return false;
}
GLvoid displayFPS(GLvoid)
{
	
	static long lastTime = glutGet(GLUT_ELAPSED_TIME);
	static long loops = 0;
	static GLfloat FPS = 0.0f;
	GLint newtime = glutGet(GLUT_ELAPSED_TIME);
	if (newtime - lastTime > 100)
	{
		GLfloat newFPS = (GLfloat)loops / (GLfloat)(newtime - lastTime) * 1000.0f;
		FPS = (FPS + newFPS) / 2.0f;
		char title[80];
		sprintf_s(title, "OpenGL Demo - %.2f", FPS);
		glutSetWindowTitle(title);
		lastTime = newtime;
		loops = 0;
	}
	loops++;
}