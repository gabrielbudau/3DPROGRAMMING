#include <vector>
#include <glew.h>
#include <freeglut.h>
#include <iostream>
#include <fstream>
#include <glm.hpp>
#include <gtx\transform.hpp>
#include <gtc\matrix_transform.hpp>
#include <utils.h>

#define MAX_POINTS 10
#define NUM_POINTS 5
using namespace glm;
using namespace std;


/*	Create checkerboard texture	*/
#define	checkImageWidth 64
#define	checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static GLuint texName;


GLdouble *red, *blue, *yellow, *black, *green, *v;
vector<V3> Points;  //vertices
vector<vector<V3> > Edges; //edges
vector<vector<V3> > Tris; //triangles
vector<V3> TrisColor; //triangles color
vector<double> pointsType;
int type;
int direction;
void initColors();
int getPointType(int i);
vector<double> ShiftVector(const vector<double>& vec);
void preprocesareInformatii();
void triangulate();
void setRandomColor();

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
GLint	windowWidth		= +1000;
GLint	windowHeight	= +1000;
GLint mouseX;
GLint mouseY;
vector<point> P;
vector<point> PColor;
GLfloat ctrlpoints[MAX_POINTS][3] =
{
	{ -4.0, +3.0, +0.0 }, 
	{ -2.0, +4.0, +0.0 },
	{ +0.0, +4.3, +0.0 },
	{ +2.0, +4.0, +0.0 },
	{ +4.0, +3.0, +0.0 },

	{ +4.0, -3.0, +0.0 },
	{ +2.0, -4.0, +0.0 },
	{ +0.0, -4.3, +0.0 },
	{ -2.0, -4.0, +0.0 },
	{ -4.0, -3.0, +0.0 },
	

};


GLboolean activePts[MAX_POINTS];
GLfloat colctrlpoints[MAX_POINTS][4] = {
	{ 1.0, 1.0, 0.0, 1.0 }, { 0.9, 0.5, 0.0, 1.0 },
	{ 0.2, 0.4, 0.3, 1.0 }, { 0.0, 0.0, 1.0, 1.0 },
	{ 1.0, 1.0, 0.0, 1.0 }, { 0.9, 0.5, 0.0, 1.0 },
	{ 0.2, 0.4, 0.3, 1.0 }, { 0.0, 0.0, 1.0, 1.0 }

};
GLfloat tolerance = 0.1;

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
float RandomFloat(float a, float b);
V3 getRandomColor();
void makeCheckImage(void);

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(10, 10);
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
	initColors();
	Tris.clear();
	TrisColor.clear();
	preprocesareInformatii();
	triangulate();
	vector<point> V;
	vector<point> VColor;
	P = V;
	PColor = VColor;
	for (int i = 0; i < MAX_POINTS; i++)
		activePts[i] = false;
	mouseBtn[0] = mouseBtn[1] = mouseBtn[2] = false;
	glClearColor(1.0, 1.0, 1.0, 0.0);
	
	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, checkImageWidth, checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

	glShadeModel(GL_FLAT);

}

GLvoid display(GLvoid)
{
	
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	mouseAction(mouseX, mouseY);
	drawGrid();
	drawMousePoints();
	int i;

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, texName);
	
	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < Tris.size(); i++){
		//glColor3f(TrisColor.at(i).getX(), TrisColor.at(i).getY(), TrisColor.at(i).getZ());
		glTexCoord2f(0.5, 0.0);
		glVertex2f(Tris.at(i).at(0).getX(), Tris.at(i).at(0).getY());
		glTexCoord2f(0.0, 1.0);
		glVertex2f(Tris.at(i).at(1).getX(), Tris.at(i).at(1).getY());
		glTexCoord2f(1.0, 1.0);
		glVertex2f(Tris.at(i).at(2).getX(), Tris.at(i).at(2).getY());
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	/*****************************************************************************/
	/* AFISARE LINII
	concav concav = blue
	convex convex = red
	concav convex = yellow */
	glEnable(GL_LINE_SMOOTH);
	//glColor3dv(black);
	//glLineWidth(1.0);
	glBegin(GL_LINE_STRIP);
	for (unsigned int i = 0; i < Points.size(); i++){
		if (i == (Points.size() - 1)){
			if ((getPointType(i) == CONVEX) && (getPointType(0) == CONVEX)){
				glColor3dv(red);
				glVertex2f(Points.at(i).getX(), Points.at(i).getY());
				glVertex2f(Points.at(0).getX(), Points.at(0).getY());
			}
			else if ((getPointType(i) == CONCAVE) && (getPointType(0) == CONCAVE)){
				glColor3dv(blue);
				glVertex2f(Points.at(i).getX(), Points.at(i).getY());
				glVertex2f(Points.at(0).getX(), Points.at(0).getY());
			}
			else{
				glColor3dv(yellow);
				glVertex2f(Points.at(i).getX(), Points.at(i).getY());
				glVertex2f(Points.at(0).getX(), Points.at(0).getY());
			}
		}
		else{
			if ((getPointType(i) == CONVEX) && (getPointType(i + 1) == CONVEX)){
				glColor3dv(red);
				glVertex2f(Points.at(i).getX(), Points.at(i).getY());
				glVertex2f(Points.at(i + 1).getX(), Points.at(i + 1).getY());
			}
			else if ((getPointType(i) == CONCAVE) && (getPointType(i + 1) == CONCAVE)){
				glColor3dv(blue);
				glVertex2f(Points.at(i).getX(), Points.at(i).getY());
				glVertex2f(Points.at(i + 1).getX(), Points.at(i + 1).getY());
			}
			else{
				glColor3dv(yellow);
				glVertex2f(Points.at(i).getX(), Points.at(i).getY());
				glVertex2f(Points.at(i + 1).getX(), Points.at(i + 1).getY());
			}
		}
	}
	glEnd();

	/*****************************************************************************/
	/* AFISARE PUNCTE
	convex = green
	concav = black */
	glColor3dv(v);
	glPointSize(10.0);
	glBegin(GL_POINTS);
	for (unsigned int i = 0; i < Points.size(); i++){
		if (getPointType(i) == CONVEX){
			glColor3dv(green);
			glVertex2d(Points.at(i).getX(), Points.at(i).getY());
		}
		else{
			glColor3dv(black);
			glVertex2d(Points.at(i).getX(), Points.at(i).getY());
		}
	}
	glEnd();

	//First Curve

	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, NUM_POINTS, &ctrlpoints[0][0]);
	glMap1f(GL_MAP1_COLOR_4, 0.0, 1.0, 4, NUM_POINTS, &colctrlpoints[0][0]);
	glEnable(GL_MAP1_VERTEX_3);
	glEnable(GL_MAP1_COLOR_4);
	glLineWidth(3.0);
	glBegin(GL_LINE_STRIP);
	for (i = 0; i <= 30; i++)
		glEvalCoord1f((GLfloat)i / 30.0);
	glEnd();
	/* The following code displays the control points as dots. 
	glPointSize(10.0);
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_POINTS);
	for (i = 0; i < MAX_POINTS; i++)
		glVertex3fv(&ctrlpoints[i][0]);
	glEnd();
	*/
	//Second Curve
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, NUM_POINTS, &ctrlpoints[5][0]);
	glMap1f(GL_MAP1_COLOR_4, 0.0, 1.0, 4, NUM_POINTS, &colctrlpoints[5][0]);
	glEnable(GL_MAP1_VERTEX_3);
	glEnable(GL_MAP1_COLOR_4);
	glLineWidth(3.0);
	glBegin(GL_LINE_STRIP);
	for (i = 0; i <= 30; i++)
		glEvalCoord1f((GLfloat)i / 30.0);
	glEnd();

	

	glFlush();

	
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
		Tris.clear();
		TrisColor.clear();
		preprocesareInformatii();
		triangulate();
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
	glLineWidth(.7);
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

	glLineWidth(1.0);
	glBegin(GL_LINES);
	glVertex2i(0, 5);
	glVertex2i(0, -5);
	glVertex2i(5, 0);
	glVertex2i(-5, 0);
	glEnd();

	glLineWidth(.3);
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
	//P.push_back(point(sceneX, sceneY, 0.0));
	//PColor.push_back(point((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX));
	
	
	if (mouseBtn[GLUT_LEFT_BUTTON])
	{
		for (int i = 0; i < MAX_POINTS; i++)
		{
			if ((sceneX < (ctrlpoints[i][0] + tolerance)) &&
				(sceneX >(ctrlpoints[i][0] - tolerance)) &&
				(sceneY < (ctrlpoints[i][1] + tolerance)) &&
				(sceneY >(ctrlpoints[i][1] - tolerance)))
				activePts[i] = true;
		}

	}
	
	for (int i = 0; i < MAX_POINTS; i++)
	{
		if (activePts[i])
		{
			if (sceneX > (windowLeft - tolerance) && 
				sceneX < (windowRight - tolerance) && 
				sceneY < (windowTop - tolerance) && 
				sceneY >(windowBottom - tolerance))
			{
				ctrlpoints[i][0] = sceneX;
				ctrlpoints[i][1] = sceneY;
			}
		}
	}

	if (!mouseBtn[GLUT_LEFT_BUTTON]){
		for (int i = 0; i < MAX_POINTS; i++)
		{
			if (activePts[i])
			{
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
		//glColor3f(P.at(i).x, P.at(i).y, P.at(i).z);
		glVertex2f(P.at(i).x, P.at(i).y);
	}
	glEnd();
	glPopAttrib();
}

GLvoid setRandomColor()
{
	glColor3f((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);
}

vector<double> ShiftVector(const vector<double>& vec){
	vector<double> temp;
	temp.push_back(vec.at(vec.size() - 1));
	for (unsigned int i = 0; i < vec.size() - 1; i++)
		temp.push_back(vec.at(i));
	return temp;
}

void preprocesareInformatii(){
	Points = readFromFile(ctrlpoints, MAX_POINTS);
	type = testCC(Points);
	direction = ClockWise(Points, pointsType);
	pointsType = ShiftVector(pointsType);

}

void initColors(){
	red = (GLdouble*)malloc(3 * sizeof(GLdouble));
	blue = (GLdouble*)malloc(3 * sizeof(GLdouble));
	yellow = (GLdouble*)malloc(3 * sizeof(GLdouble));
	black = (GLdouble*)malloc(3 * sizeof(GLdouble));
	green = (GLdouble*)malloc(3 * sizeof(GLdouble));
	v = (GLdouble*)malloc(3 * sizeof(GLdouble));
	red[0] = 1.00; red[1] = 0.00; red[2] = 0.00;
	black[0] = 0.00; black[1] = 0.00; black[2] = 0.00;
	blue[0] = 0.00; blue[1] = 0.00; blue[2] = 1.00;
	yellow[0] = 1.00; yellow[1] = 1.00; yellow[2] = 0.00;
	green[0] = 0.00; green[1] = 1.00; green[2] = 0.00;
	v[0] = 0.50; v[1] = 0.50; v[2] = 0.50;
}

int getPointType(int i){
	if (direction == CLOCKWISE){
		if (pointsType.at(i) < 0){
			return CONVEX;
		}
		else {
			return CONCAVE;
		}
	}
	else{
		if (pointsType.at(i) > 0){
			return CONVEX;
		}
		else {
			return CONCAVE;
		}
	}
}

float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

void triangulate(){
	bool pit;
	pit = true;//points in triangle, assume there are points in the given tri
	unsigned int i = 1;
	vector<V3> vec;
	vector<V3> temp(Points);
	vector<double> temp1(pointsType);
	if (Points.size()>0){
		while (Points.size() > 2 && i < Points.size()-1){
			pit = true;
			if (Points.size() == 3){
				vec.push_back(Points.at(0)); vec.push_back(Points.at(1)); vec.push_back(Points.at(2));
				Tris.push_back(vec);
				TrisColor.push_back(getRandomColor());
				vec.clear();
				break;
			}
			if (getPointType(i) == CONVEX){
				
				if (i == 0)
					pit = checkTriangle(i, i + 1, Points.size() - 1, Points);
				else
					pit = checkTriangle(i, i + 1, i - 1, Points);

				if (!pit){
					if (i == 0){
						vec.push_back(Points.at(i)); vec.push_back(Points.at(i + 1)); vec.push_back(Points.at(Points.size() - 1));
						Tris.push_back(vec);
						TrisColor.push_back(getRandomColor());
						vec.clear();
					}
					else{
						vec.push_back(Points.at(i)); vec.push_back(Points.at(i + 1)); vec.push_back(Points.at(i - 1));
						Tris.push_back(vec);
						TrisColor.push_back(getRandomColor());
						vec.clear();
					}
					Points.erase(Points.begin() + i);
					pointsType.erase(pointsType.begin() + i);
				}
				else{
					i++;
				}
			}
			else{
				i++;
			}
		}
	}
	pointsType = temp1;
	Points = temp;
}

V3 getRandomColor(){
	return V3(RandomFloat(0.0, 1.0), RandomFloat(0.0, 1.0), RandomFloat(0.0, 1.0));
}

void makeCheckImage(void)
{
	int i, j, c;

	for (i = 0; i < checkImageHeight; i++) {
		for (j = 0; j < checkImageWidth; j++) {
			c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 200;

			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}
}