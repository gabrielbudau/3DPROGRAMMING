#include <GLWindow.h>
GLuint programID;
GLuint numIndices;
Camera camera;
GLuint width = WINDOW_WIDTH, height = WINDOW_HEIGHT;
bool* keyStates = new bool[256];
bool* keySpecialStates = new bool[256];

void GLWindow::sendDataToOpenGL()
{
	ShapeData shape = ShapeGenerator::makeArrow();

	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)* 9, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)* 9, (char*)(sizeof(GLfloat)* 3));

	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_STATIC_DRAW);
	numIndices = shape.numIndices;
	shape.cleanUp();
}

bool GLWindow::checkStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetter, PFNGLGETSHADERINFOLOGPROC getInfoLogFunc, GLenum statusType)
{
	GLint status;
	objectPropertyGetter(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetter(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];
		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << endl << buffer << endl;
		delete[] buffer;
		return false;
	}
	return true;
}

bool GLWindow::checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool GLWindow::checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

string GLWindow::readVertexShaderCode(const char* fileName)
{
	ifstream input(fileName);
	if (!input)
	{
		cout << "File failed to load... " << fileName << endl;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(input),
		std::istreambuf_iterator<char>());
}

void GLWindow::installShaders()
{
	//create shaders
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	//insert shader into compiler
	const GLchar* adapter[1];
	string temp = readVertexShaderCode("VertexShaderCode.glsl");
	adapter[0] =  temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readVertexShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);
	//compile shaders
	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);
	//get compile errors
	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
	{
		return;
	}
	
	//atach shaders
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	//link program
	glLinkProgram(programID);
	//check link errors
	if (!checkProgramStatus(programID))
	{
		return;
	}

	//delete shaders
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(programID);
}


GLWindow::GLWindow(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(width, height);
	glutCreateWindow("OPENGL");
	initGL();
	glClear(GL_COLOR_BUFFER_BIT);

	glutDisplayFunc(paintGL);
	glutMotionFunc(mouseActiveMotion);
	glutPassiveMotionFunc(mousePasiveMotion);

	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);

	glutSpecialFunc(keySpecial);
	glutSpecialUpFunc(keySpecialUp);
	glutIdleFunc(paintGL);
	glutReshapeFunc(reshape);
	glutMainLoop();
}

GLWindow::~GLWindow()
{
	glUseProgram(0);
	glDeleteProgram(programID);
}

void GLWindow::initGL()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-3, 3, -1, 1);
	glewInit();
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glEnable(GL_DEPTH_TEST);
	cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);
	initBoolArray(keyStates);
	initBoolArray(keySpecialStates);
	sendDataToOpenGL();
	installShaders();
}

void paintGL(void)
{
	keyOperations();
	keySpecialOperations();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	mat4 fullTransformMatrix;
	mat4 viewToProjectionMatrix = perspective(60.0f, ((GLfloat)width / height), 0.1f, 10.0f);
	mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;
	GLuint fullTransformUniformLocation = 0;
	glGetUniformLocation(fullTransformUniformLocation, "fulltransformMatrix");

	mat4 cube1ModelToWorldMatrix = translate(vec3(-1.0f, +0.0f, -3.75f))*rotate(126.0f, vec3(1.0f, 0.0f, 0.0f));
	fullTransformMatrix = worldToProjectionMatrix * cube1ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);

	mat4 cube2ModelToWorldMatrix = translate(vec3(+1.0f, +0.0f, -3.75f))*rotate(36.0f, vec3(0.0f, 1.0f, 0.0f));
	fullTransformMatrix = worldToProjectionMatrix * cube2ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);

	glFlush();
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glutPostRedisplay();
}

void mousePasiveMotion(int x, int y){
	/*
	camera.mouseUpdate(vec2(x, y));
	glutPostRedisplay();
	*/
}

void mouseActiveMotion(int x, int y){
	camera.mouseUpdate(vec2(x, y));
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
		camera.moveForward();
	}
	else if (keyStates['s']){
		camera.moveBackward();
	}
	else if (keyStates['a']){
		camera.strafeLeft();
	}
	else if (keyStates['d']){
		camera.strafeRight();
	}
	else if (keyStates['r']){
		camera.moveUp();
	}
	else if (keyStates['f']){
		camera.moveDown();
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
		camera.moveForward();
	}
	else if (keySpecialStates[GLUT_KEY_DOWN]){
		camera.moveBackward();
	}
	else if (keySpecialStates[GLUT_KEY_LEFT]){
		camera.strafeLeft();
	}
	else if (keySpecialStates[GLUT_KEY_RIGHT]){
		camera.strafeRight();
	}
	
}

void initBoolArray(bool *ba)
{
	for (int i = 0; i < 256; i++)
		ba[i] = false;
}
