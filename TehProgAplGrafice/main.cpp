#include <vector>
#include <glew.h>
#include <freeglut.h>
#include <iostream>
#include <fstream>
#include <glm.hpp>
#include <gtx\transform.hpp>
#include <gtc\matrix_transform.hpp>

using namespace glm;
using namespace std;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 400
#define NOT_POLYGON			0
#define CONVEX				1
#define CONCAVE				2
#define NECOPLANARE			0
#define COPLANARE			1
#define COUNTERCLOCKWISE	3
#define CLOCKWISE			4

bool* keyStates = new bool[256];
bool* keySpecialStates = new bool[256];

GLdouble *red, *blue, *yellow, *black, *green, *v;
vector<vec3> Points;  //vertices
vector<vector<vec3> > Edges; //edges
vector<vector<vec3> > Tris; //triangles
vector<double> pointsType;
int type;
int direction;

vector<double> ShiftVector(const vector<double>& vec);
void initColors();
int getPointType(int i);
void preprocesareInformatii();
void triangulate();
void setRandomColor();
void initGL();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
int testCC(std::vector<glm::vec3>& P);
int testC(std::vector<glm::vec3>& P);
int ClockWise(std::vector<glm::vec3>& P, std::vector<double>& PType);
bool checkTriangle(unsigned int i, unsigned int j, unsigned int k, std::vector<glm::vec3> Points);
float area(glm::vec3 a, glm::vec3 b, glm::vec3 c);
bool isInside(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 point);
vector<vec3> readFromFile();


/* Handler for window-repaint event. Call back when the window first appears and
whenever the window needs to be re-painted. */

void display() {


	glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color
	// Define shapes enclosed within a pair of glBegin and glEnd
	glBegin(GL_QUADS);
	glColor3f(0.7f, 0.7f, 0.7f);
	glVertex2f(0.0f, 1.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex2f(0.0f, 0.0f);
	glColor3f(0.7f, 0.7f, 0.7f);
	glVertex2f(1.0f, 0.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex2f(1.0f, 1.0f);
	glEnd();

	for (float i = 0.0; i < 1.0; i += 0.05){
		glBegin(GL_LINES);
		glColor3f(0.3f, 0.3f, 0.3f);
		glVertex2f(0.0f, i);
		glVertex2f(1.0f, i);
		glEnd();
	}

	for (float i = 0.0; i < 1.0; i += 0.05){
		glBegin(GL_LINES);
		glColor3f(0.3f, 0.3f, 0.3f);
		glVertex2f(i, 0.0f);
		glVertex2f(i, 1.0f);
		glEnd();
	}
	/*****************************************************************************/
	/* AFISARE TRIUNGHIURI
	*/

	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < Tris.size(); i++){
		setRandomColor();
		glVertex2f(Tris.at(i).at(0).x, Tris.at(i).at(0).y);
		glVertex2f(Tris.at(i).at(1).x, Tris.at(i).at(1).y);
		glVertex2f(Tris.at(i).at(2).x, Tris.at(i).at(2).y);
	}
	glEnd();
	/*****************************************************************************/
	/* AFISARE LINII
	concav concav = blue
	convex convex = red
	concav convex = yellow */
	glEnable(GL_LINE_SMOOTH);
	glColor3dv(black);
	//glLineWidth(1.0);
	glBegin(GL_LINE_STRIP);
	for (unsigned int i = 0; i < Points.size(); i++){
		if (i == (Points.size() - 1)){
			if ((getPointType(i) == CONVEX) && (getPointType(0) == CONVEX)){
				glColor3dv(red);
				glVertex2f(Points.at(i).x, Points.at(i).y);
				glVertex2f(Points.at(0).x, Points.at(0).y);
			}
			else if ((getPointType(i) == CONCAVE) && (getPointType(0) == CONCAVE)){
				glColor3dv(blue);
				glVertex2f(Points.at(i).x, Points.at(i).y);
				glVertex2f(Points.at(0).x, Points.at(0).y);
			}
			else{
				glColor3dv(yellow);
				glVertex2f(Points.at(i).x, Points.at(i).y);
				glVertex2f(Points.at(0).x, Points.at(0).y);
			}
		}
		else{
			if ((getPointType(i) == CONVEX) && (getPointType(i + 1) == CONVEX)){
				glColor3dv(red);
				glVertex2f(Points.at(i).x, Points.at(i).y);
				glVertex2f(Points.at(i + 1).x, Points.at(i + 1).y);
			}
			else if ((getPointType(i) == CONCAVE) && (getPointType(i + 1) == CONCAVE)){
				glColor3dv(blue);
				glVertex2f(Points.at(i).x, Points.at(i).y);
				glVertex2f(Points.at(i + 1).x, Points.at(i + 1).y);
			}
			else{
				glColor3dv(yellow);
				glVertex2f(Points.at(i).x, Points.at(i).y);
				glVertex2f(Points.at(i + 1).x, Points.at(i + 1).y);
			}
		}
	}
	glEnd();

	/*****************************************************************************/
	/* AFISARE PUNCTE
	convex = green
	concav = black */
	glColor3dv(v);
	glPointSize(4.0);
	glBegin(GL_POINTS);
	for (unsigned int i = 0; i < Points.size(); i++){
		if (getPointType(i) == CONVEX){
			glColor3dv(green);
			glVertex2d(Points.at(i).x, Points.at(i).y);
		}
		else{
			glColor3dv(black);
			glVertex2d(Points.at(i).x, Points.at(i).y);
		}
	}
	glEnd();

	//glLineWidth(1.0);
	glFlush();  // Render now
}

/* Handler for window re-size event. Called back when the window first appears and
whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
	// Compute aspect ratio of the new window
	if (height == 0) height = 1;                // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);

	// Set the aspect ratio of the clipping area to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset the projection matrix

	if (width >= height) {
		// aspect >= 1, set the height from -1 to 1, with larger width
		gluOrtho2D(0.0 * aspect, 0.5 * aspect, 0.0, 0.5);
	}
	else {
		// aspect < 1, set the width to -1 to 1, with larger height
		gluOrtho2D(0.0, 0.5, 0.0 / aspect, 0.5 / aspect);
	}
}


int main(int argc, char** argv) {
	initColors();
	preprocesareInformatii();
	triangulate();
	glutInit(&argc, argv);          // Initialize GLUT
	glutInitWindowSize(480, 480);   // Set the window's initial width & height
	glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
	glutCreateWindow("OPENGL_2D");  // Create window with the given title
	glutDisplayFunc(display);       // Register callback handler for window re-paint event
	glutReshapeFunc(reshape);       // Register callback handler for window re-size event

	initGL();                        // Our own OpenGL initialization
	glutMainLoop();                 // Enter the event-processing loop
	return 0;
}

void initGL() {
	// Set "clearing" or background color
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Black and opaque
}

vector<vec3> readFromFile(){
	vector<vec3> V;
	ifstream f("puncte.txt");
	float x, y, z;
	while (f >> x >> y >> z)
		V.push_back(vec3(x, y, z));
	f.close();
	return V;
}

vector<double> ShiftVector(const vector<double>& vec){
	vector<double> temp;
	temp.push_back(vec.at(vec.size() - 1));
	for (unsigned int i = 0; i < vec.size() - 1; i++)
		temp.push_back(vec.at(i));
	return temp;
}

void preprocesareInformatii(){
	Points = readFromFile();
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

void setRandomColor(){
	glColor3f((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);
}

void triangulate(){
	bool pit;
	pit = true;//points in triangle, assume there are points in the given tri
	unsigned int i = 0;
	vector<vec3> vec;
	vector<vec3> temp(Points);
	vector<double> temp1(pointsType);
	if (Points.size()>0){
		while (Points.size() > 2 && i < Points.size()){
			pit = true;
			if (Points.size() == 3){
				vec.push_back(Points.at(0)); vec.push_back(Points.at(1)); vec.push_back(Points.at(2));
				Tris.push_back(vec);
				vec.clear();
				break;
			}
			if (getPointType(i) == CONVEX){
				cout << "CONVEX\n";
				if (i == 0)
					pit = checkTriangle(i, i + 1, Points.size() - 1, Points);
				else
					pit = checkTriangle(i, i + 1, i - 1, Points);

				if (pit){
					if (i == 0){
						vec.push_back(Points.at(i)); vec.push_back(Points.at(i + 1)); vec.push_back(Points.at(Points.size() - 1));
						Tris.push_back(vec);
						vec.clear();
					}
					else{
						vec.push_back(Points.at(i)); vec.push_back(Points.at(i + 1)); vec.push_back(Points.at(i - 1));
						Tris.push_back(vec);
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

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	}
}

int testCC(std::vector<glm::vec3>& P){
	std::vector<glm::vec3> V;
	int pos = 0, neg = 0, max;
	/*how many negative and positive elements are in V <0, 0, pos/neg> */

	if (P.size() > 2){
		for (unsigned int i = 0; i < P.size(); i++){
			if (i == P.size() - 2)
				V.push_back((P.at(i + 1) - P.at(i)) * ((P.at(0) - P.at(i + 1))));
			else if (i == P.size() - 1)
				V.push_back((P.at(0) - P.at(i)) * ((P.at(1) - P.at(0))));
			else
				V.push_back((P.at(i + 1) - P.at(i)) * ((P.at(i + 2) - P.at(i + 1))));
		}
	}
	else{
		return NOT_POLYGON;
	}



	for (unsigned int i = 0; i < V.size(); i++){
		if (V.at(i).z < 0)
			neg++;
		else pos++;
	}
	max = pos;
	max = (neg > max) ? neg : max;
	for (unsigned int i = 0; i < V.size(); i++){
		if (max == pos)
		if (V.at(i).z < 0)
			return CONCAVE;
		if (max == neg)
		if (V.at(i).z >= 0)
			return CONCAVE;
	}
	return CONVEX;
	return 0;
}

int testC(std::vector<glm::vec3>& P){
	std::vector<glm::vec3> V;
	bool flag = true; //presupunem ca punctele sunt coplanare
	for (unsigned int i = 1; i < P.size(); i++){
		V.push_back(P.at(i) - P.at(0));
	}
	std::cout << "\n";
	for (unsigned int i = 0; i < V.size(); i++){
		std::cout << "< " << (V.at(i)).x << ", " << (V.at(i)).y << ", " << (V.at(i)).z << " >";
		std::cout << "\n";
	}

	for (unsigned int i = 0; i < V.size(); i++){
		if (!(V.at(i).z == 0.0)){
			flag = false;
			break;
		}
	}
	return flag ? COPLANARE : NECOPLANARE;
}

int ClockWise(std::vector<glm::vec3>& P, std::vector<double>& PType)
{
	int i, j, k;
	int n = P.size();
	int count = 0;
	double z;

	if (n < 3)
		return -1;

	for (i = 0; i < n; i++) {
		j = (i + 1) % n;
		k = (i + 2) % n;
		z = (P.at(j).x - P.at(i).x) * (P.at(k).y - P.at(j).y);
		z -= (P.at(j).y - P.at(i).y) * (P.at(k).x - P.at(j).x);
		PType.push_back(z);
		if (z < 0)
			count--;
		else if (z > 0)
			count++;
	}
	if (count > 0)
		return(COUNTERCLOCKWISE);
	else if (count < 0)
		return(CLOCKWISE);
	else
		return -1;
}

bool checkTriangle(unsigned int i, unsigned int j, unsigned int k, std::vector<glm::vec3> Points){
	bool isinside = false;
	unsigned int q;
	for (q = 0; q < Points.size(); q++){
		if ((q != i) && (q != j) && (q != k)){
			isinside = isInside(Points.at(i), Points.at(j), Points.at(k), Points.at(q));
			if (isinside)
				return isinside;
		}
	}
	return isinside;
}

/* A utility function to calculate area of triangle formed by (x1, y1),
(x2, y2) and (x3, y3) */
float area(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	return abs((a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y)) / 2.0);
}

/* A function to check whether point P(x, y) lies inside the triangle formed
by A(x1, y1), B(x2, y2) and C(x3, y3) */
bool isInside(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 point)
{
	/* Calculate area of triangle ABC */
	float A = area(a, b, c);

	/* Calculate area of triangle PBC */
	float A1 = area(point, b, c);

	/* Calculate area of triangle PAC */
	float A2 = area(a, point, c);

	/* Calculate area of triangle PAB */
	float A3 = area(a, b, point);

	/* Check if sum of A1, A2 and A3 is same as A */
	return (A == A1 + A2 + A3);
}

void mousePasiveMotion(int x, int y){
	/*
	camera.mouseUpdate(vec2(x, y));
	glutPostRedisplay();
	*/
}

void mouseActiveMotion(int x, int y){
	if (keyStates[' '] == true){

	}
	glutPostRedisplay();
}

void keyPressed(unsigned char key, int x, int y)
{
	keyStates[key] = true;
	glutPostRedisplay();
}

void keyUp(unsigned char key, int x, int y)
{
	keyStates[key] = false;
	glutPostRedisplay();
}

void keyOperations(void)
{
	if (keyStates['w']){
		
	}
	else if (keyStates['s']){
		
	}
	else if (keyStates['a']){
		
	}
	else if (keyStates['d']){
		
	}
	else if (keyStates['r']){
		
	}
	else if (keyStates['f']){
		
	}

}

void keySpecial(int key, int x, int y)
{
	keySpecialStates[key] = true;
}

void keySpecialUp(int key, int x, int y)
{
	keySpecialStates[key] = false;
}

void keySpecialOperations(void) {
	if (keySpecialStates[GLUT_KEY_UP]){
		
	}
	else if (keySpecialStates[GLUT_KEY_DOWN]){
		
	}
	else if (keySpecialStates[GLUT_KEY_LEFT]){
		
	}
	else if (keySpecialStates[GLUT_KEY_RIGHT]){
		
	}

}