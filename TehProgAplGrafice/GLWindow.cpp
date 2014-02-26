#include <GLWindow.h>
GLuint programID;
GLuint numIndices, cubeNumIndices, arrowNumIndices, planeNumIndices;
Camera camera;
GLuint width = WINDOW_WIDTH, height = WINDOW_HEIGHT;
bool* keyStates = new bool[256];
bool* keySpecialStates = new bool[256];
GLuint fullTransformUniformLocation;

GLuint theBufferID;

GLuint cubeVertexArrayObjectID;
GLuint arrowVertexArrayObjectID;
GLuint planeVertexArrayObjectID;

GLuint arrowIndexDataByteOffset;
GLuint cubeIndexDataByteOffset;
GLuint planeIndexDataByteOffset;

Cube cub, cub1;

#define	checkImageWidth 64
#define	checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

#ifdef GL_VERSION_1_1
static GLuint texName;
#endif

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



void GLWindow::sendDataToOpenGL()
{
	/*
	ShapeData cube = ShapeGenerator::makeTeapot(10);
	ShapeData arrow = ShapeGenerator::makePlane(20);
	ShapeData plane = ShapeGenerator::makePlane(20);
	*/

	glGenBuffers(1, &theBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glBufferData(GL_ARRAY_BUFFER, cub.bufferSize + cub1.bufferSize, 0, GL_STATIC_DRAW);
	GLuint currentOffset = 0;

	currentOffset = cub.sendDataToOpenGL(currentOffset, theBufferID);
	currentOffset = cub1.sendDataToOpenGL(currentOffset, theBufferID);

	/*
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.vertexBufferSize(), cube.vertices);
	currentOffset += cube.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.indexBufferSize(), cube.indices);
	cubeIndexDataByteOffset = currentOffset;
	currentOffset += cube.indexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.vertexBufferSize(), arrow.vertices);
	currentOffset += arrow.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.indexBufferSize(), arrow.indices);
	arrowIndexDataByteOffset = currentOffset;
	currentOffset += arrow.indexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.vertexBufferSize(), plane.vertices);
	planeIndexDataByteOffset = currentOffset;
	currentOffset += plane.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.indexBufferSize(), plane.indices);

	arrowNumIndices = arrow.numIndices;
	cubeNumIndices = cube.numIndices;
	planeNumIndices = plane.numIndices;

	cout << "\ncube byte offset: " << cubeIndexDataByteOffset << "\narrow byte offset: " << arrowIndexDataByteOffset << "\nplane byte offset: " << planeIndexDataByteOffset;
	cout << "\ncube indices: " << cubeNumIndices << "\narrow indices: " << arrowNumIndices << "\nplane indices: " << planeNumIndices;

	glGenVertexArrays(1, &cubeVertexArrayObjectID);
	glGenVertexArrays(1, &arrowVertexArrayObjectID);
	glGenVertexArrays(1, &planeVertexArrayObjectID);

	glBindVertexArray(cubeVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)* 6, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)* 6, (void*)(sizeof(GLfloat)* 3));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);


	glBindVertexArray(arrowVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint arrowByteOffset = cube.vertexBufferSize() + cube.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)* 6, (void*)(arrowByteOffset));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)* 6, (void*)(arrowByteOffset + sizeof(GLfloat)* 3));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	glBindVertexArray(planeVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint planeByteOffset = cube.vertexBufferSize() + cube.indexBufferSize() + arrow.vertexBufferSize() + arrow.indexBufferSize();;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)* 6, (void*)(planeByteOffset));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)* 6, (void*)(planeByteOffset + sizeof(GLfloat) * 3));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	cube.cleanUp();
	arrow.cleanUp();
	plane.cleanUp();
	*/
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
	adapter[0] = temp.c_str();
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
	glDeleteBuffers(1, &theBufferID);
	glUseProgram(0);
	glDeleteProgram(programID);
}

void GLWindow::initGL()
{
	cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-3, 3, -1, 1);
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glewInit();
	initBoolArray(keyStates);
	initBoolArray(keySpecialStates);
	sendDataToOpenGL();
	installShaders();
	fullTransformUniformLocation = glGetUniformLocation(programID, "fulltransformMatrix");

}

void paintGL(void)
{
	keyOperations();
	keySpecialOperations();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 fullTransformMatrix;
	mat4 viewToProjectionMatrix = perspective(60.0f, ((GLfloat)width / height), 0.1f, 20.0f);
	
	mat4 worldToViewMatrix = camera.getWorldToViewMatrix() * rotate(15.0f, vec3(1.0f, 0.0f, 0.0f));
	mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;
	GLint ambientLightUniformLocation = glGetUniformLocation(programID, "ambientLight");
	vec3 ambientLight(0.9f, 0.9f, 0.9f);
	glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);
	//Cube
	glBindVertexArray(cubeVertexArrayObjectID);

	
	cub.modelToWorldMatrix =
		translate(vec3(-3.0f, +0.0f, -3.75f)) *
		rotate(-90.0f, vec3(1.0f, 0.0f, 0.0f)) *
		rotate(-90.0f, vec3(0.0f, 0.0f, 1.0f));
	cub.drawCube(worldToProjectionMatrix, fullTransformUniformLocation);

	cub1.modelToWorldMatrix = 
		translate(vec3(+3.0f, +0.0f, -3.75f))*
		rotate(-90.0f, vec3(1.0f, 0.0f, 0.0f));

	cub1.drawCube(worldToProjectionMatrix, fullTransformUniformLocation);

	/*
	fullTransformMatrix = worldToProjectionMatrix * cube1ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, cubeNumIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexDataByteOffset);
	*/

	/*

	mat4 cube2ModelToWorldMatrix = translate(vec3(+3.0f, +0.0f, -3.75f))*rotate(-90.0f, vec3(1.0f, 0.0f, 0.0f));
	fullTransformMatrix = worldToProjectionMatrix * cube2ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, cubeNumIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexDataByteOffset);

	//Arrow
	glBindVertexArray(arrowVertexArrayObjectID);
	mat4 arrowModelToWorldMatrix = translate(vec3(0.0f, +0.0f, -3.0f));
	fullTransformMatrix = worldToProjectionMatrix * arrowModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, arrowNumIndices, GL_UNSIGNED_SHORT, (void*)(arrowIndexDataByteOffset));


	glBindVertexArray(planeVertexArrayObjectID);
	mat4 planeModelToWorldMatrix = glm::mat4();
	fullTransformMatrix = worldToProjectionMatrix * planeModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, planeNumIndices, GL_UNSIGNED_SHORT, (void*)(planeIndexDataByteOffset));
	*/
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

