#include <vector>
#include <glew.h>
#include <freeglut.h>
#include <iostream>
#include <fstream>
#include <glm.hpp>
#include <gtx\transform.hpp>
#include <gtc\matrix_transform.hpp>

#define MAX_POINTS 4
using namespace glm;
using namespace std;

bool mouseBtn[3];
struct point
{
	GLfloat x, y, z;
	point(float _x, float _y, float _z) :x(_x), y(_y), z(_z){}
};
GLfloat windowLeft		= -5.0;
GLfloat windowRight		= +5.0;
GLfloat windowTop		= +5.0;
GLfloat windowBottom	= -5.0;
GLint	windowWidth		= +500;
GLint	windowHeight	= +500;
GLint mouseX;
GLint mouseY;
vector<point> P;
vector<point> PColor;
GLfloat ctrlpoints[MAX_POINTS][3] =
{
	{ -4.0, -4.0, +0.0 }, 
	{ -2.0, +4.0, +0.0 },
	{ +2.0, -4.0, +0.0 }, 
	{ +4.0, +4.0, +0.0 },
};
GLboolean activePts[MAX_POINTS];
GLfloat colctrlpoints[MAX_POINTS][4] = {
	{ 1.0, 1.0, 0.0, 1.0 }, { 0.9, 0.5, 0.0, 1.0 },
	{ 0.2, 0.4, 0.3, 1.0 }, { 0.0, 0.0, 1.0, 1.0 } };
GLfloat tolerance = 0.05;

GLvoid init(GLvoid);
GLvoid display(GLvoid);
GLvoid reshape(GLint, GLint);
GLvoid keyboard(GLubyte, GLint, GLint);
GLvoid mouse(GLint, GLint, GLint, GLint);
GLvoid motion(GLint, GLint);
GLvoid drawGrid();
GLvoid mouseAction(GLint, GLint);
GLvoid drawMousePoints();
GLvoid setRandomColor();
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutIdleFunc(display);
	glutMainLoop();
	return 0;
}

GLvoid init(GLvoid)
{
	vector<point> V;
	vector<point> VColor;
	P = V;
	PColor = VColor;
	for (int i = 0; i < MAX_POINTS; i++)
		activePts[i] = false;
	mouseBtn[0] = mouseBtn[1] = mouseBtn[2] = false;
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glShadeModel(GL_FLAT);
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
	glMap1f(GL_MAP1_COLOR_4, 0.0, 1.0, 4, 4, &colctrlpoints[0][0]);
	glEnable(GL_MAP1_VERTEX_3);
	glEnable(GL_MAP1_COLOR_4);
}

GLvoid display(GLvoid)
{
	mouseAction(mouseX, mouseY);
	int i;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	
	drawGrid();

	glLineWidth(3.0);
	glBegin(GL_LINE_STRIP);
	for (i = 0; i <= 30; i++)
		glEvalCoord1f((GLfloat)i / 30.0);
	glEnd();
	/* The following code displays the control points as dots. */
	glPointSize(10.0);
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_POINTS);
	for (i = 0; i < 4; i++)
		glVertex3fv(&ctrlpoints[i][0]);
	// glColor3fv (&colctrlpoints[i][0]);
	glEnd();

	drawMousePoints();

	glFlush();

	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
	glMap1f(GL_MAP1_COLOR_4, 0.0, 1.0, 4, 4, &colctrlpoints[0][0]);
	glEnable(GL_MAP1_VERTEX_3);
	glEnable(GL_MAP1_COLOR_4);
}

GLvoid reshape(GLint w, GLint h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(windowLeft, windowRight, windowBottom*(GLfloat)h/(GLfloat)w, windowTop*(GLfloat)h/(GLfloat)w, -5.0, 5.0);
	else
		glOrtho(windowLeft*(GLfloat)w / (GLfloat)h, windowRight*(GLfloat)w / (GLfloat)h, windowBottom, windowTop, -5.0, 5.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	windowWidth		= w;
	windowHeight	= h;
}

GLvoid keyboard(GLubyte key, GLint x, GLint y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	}
}

GLvoid mouse(GLint button, GLint state, GLint x, GLint y)
{
	static GLint buttons_down = 0;

	if (state == GLUT_DOWN) {
		mouseBtn[button] = true;
		mouseX = x;
		mouseY = y;
		//mouseAction(x, y);
	}
	else if(state = GLUT_UP){
		mouseBtn[button] = false;
		mouseX = x;
		mouseY = y;
		//mouseAction(x, y);
	}
}

GLvoid  motion(GLint x, GLint y)
{
	if (mouseBtn[GLUT_LEFT_BUTTON]){
		mouseX = x;
		mouseY = y;
		//mouseAction(x, y);
	}
}

GLvoid drawGrid()
{
	glLineWidth(1.0);
	glColor3f(.7, .7, .7);
	glBegin(GL_LINES);
	for (int i = -5; i <= 5; i++)
	{
		if (i != 0)
		{
			glVertex2i(i, 5);
			glVertex2i(i, -5);

			glVertex2i(5, i);
			glVertex2i(-5, i);
		}

	}
	glEnd();

	glLineWidth(1.5);
	glBegin(GL_LINES);
	glVertex2i(0, 5);
	glVertex2i(0, -5);
	glVertex2i(5, 0);
	glVertex2i(-5, 0);
	glEnd();

	glLineWidth(.5);
	glPushAttrib(GL_ENABLE_BIT);
	glLineStipple(1, 0xAAAA);
	glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINES);
	for (float i = -4.5; i <= 4.5; i++)
	{
		if (i != 0)
		{
			glVertex2f(i, 5);
			glVertex2f(i, -5);

			glVertex2f(5, i);
			glVertex2f(-5, i);
		}

	}
	glEnd();
	glPopAttrib();
}

GLvoid mouseAction(GLint x, GLint y){
	GLfloat sceneX = 0.0;
	GLfloat sceneY = 0.0;
	GLfloat ratioX = float(x) / float(windowWidth);
	GLfloat ratioY = float(y) / float(windowHeight);
	ratioY = 1.0 - ratioY;
	GLfloat sceneWidth = abs(windowLeft) + abs(windowRight);
	GLfloat sceneHeight = abs(windowTop) + abs(windowBottom);

	sceneX = (ratioX > 0.5) ? (sceneWidth * ratioX) - windowRight /*La dreapta de 0.0 pe axa X*/ : (sceneWidth * ratioX) + windowLeft;/*La stanga de 0.0 pe axa X*/
	sceneY = (ratioY > 0.5) ? (sceneHeight * ratioY) + windowBottom /*Dedesupt de 0.0 pe axa Y*/ : (sceneHeight * ratioY) - windowTop;/*Deasupra de 0.0 pe axa Y*/
	P.push_back(point(sceneX, sceneY, 0.0));
	PColor.push_back(point((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX));
	if (mouseBtn[GLUT_LEFT_BUTTON])
	{
		for (int i = 0; i < MAX_POINTS; i++)
		{
			if ((sceneX < (ctrlpoints[i][0] + tolerance)) &&
				(sceneX >(ctrlpoints[i][0] - tolerance)) &&
				(sceneY < (ctrlpoints[i][1] + tolerance)) &&
				(sceneY >(ctrlpoints[i][1] - tolerance)))
				activePts[i] = true;
				//ctrlpoints[i][0] = sceneX;
				//ctrlpoints[i][1] = sceneY;
		}

	}
	else{
		for (int i = 0; i < MAX_POINTS; i++)
		{
			if (activePts[i])
			{
				ctrlpoints[i][0] = sceneX;
				ctrlpoints[i][1] = sceneY;
				activePts[i] = false;
			}
		}
	}
}

GLvoid drawMousePoints()
{
	glPushAttrib(GL_POINT_BIT);
	glPointSize(20.0);
	
	glBegin(GL_POINTS);
	for (int i = 0; i < P.size(); i++)
	{
		//setRandomColor();
		glColor3f(P.at(i).x, P.at(i).y, P.at(i).z);
		glVertex2f(P.at(i).x, P.at(i).y);
	}
	glEnd();
	glPopAttrib();
}

GLvoid setRandomColor()
{
	glColor3f((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);
}