#include <GLWindow.h>


GLWindow::GLWindow(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Poligoane");
	initGL();
	glClear(GL_COLOR_BUFFER_BIT);
	glutDisplayFunc(paintGL);
	glutMainLoop();
}


GLWindow::~GLWindow()
{
}

void GLWindow::initGL()
{

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-1, 1, -1, 1);


	glewInit();
	GLfloat verts[] =
	{
		+0.0f, +0.0f,
		+0.0f, +0.0f, +0.0f,
		+1.0f, +1.0f,
		+0.0f, +0.0f, +0.0f,
		-1.0f, +1.0f,
		+0.0f, +0.0f, +0.0f,
		-1.0f, -1.0f,
		+0.0f, +0.0f, +0.0f,
		+1.0f, -1.0f,
		+0.0f, +0.0f, +0.0f
	};
	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)* 5, (char*)(sizeof(GLfloat)*2));


	GLushort indices[] = { 0,1,2, 0,3,4 };
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	
}

void paintGL(void)
{

	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	glFlush();
}