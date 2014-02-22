#include <GLWindow.h>
GLuint programID;
GLuint numIndices;


void GLWindow::sendDataToOpenGL()
{
	//ShapeData tri = ShapeGenerator::makeTriangle();
	ShapeData cube = ShapeGenerator::makeCube();
	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, cube.vertexBufferSize(), cube.vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)* 6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)* 6, (char*)(sizeof(GLfloat)* 3));

	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube.indexBufferSize(), cube.indices, GL_STATIC_DRAW);
	numIndices = cube.numIndices;
	cube.cleanUp();
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
	glutInitWindowPosition(800, 50);
	glutInitWindowSize(400, 400);
	glutCreateWindow("OPENGL");
	initGL();
	glClear(GL_COLOR_BUFFER_BIT);
	glutDisplayFunc(paintGL);
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
	gluOrtho2D(-1, 1, -1, 1);
	glewInit();

	glEnable(GL_DEPTH_TEST);
	cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);
	
	sendDataToOpenGL();
	installShaders();
}

void paintGL(void)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	
	GLint fullTransformMatrixUniformLocation = glGetUniformLocation(programID, "fullTransformMatrix");
	mat4 fullTransformMatrix;
	mat4 projectionMatrix = perspective(60.0f, ((GLfloat) WINDOW_WIDTH/WINDOW_HEIGHT), 0.1f, 10.0f);

	//Cube 1:
	mat4 translationMatrix = translate( vec3(1.0f, 0.0f, -3.75f));
	mat4 rotationMatrix = rotate(126.0f, vec3(1.0f, 0.0f, 0.0f));
	fullTransformMatrix = projectionMatrix * translationMatrix * rotationMatrix;
	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
	//Cube 2:
	translationMatrix = translate(vec3(-1.0f, 0.0f, -3.0f));
	rotationMatrix = rotate(36.0f, vec3(0.0f, 1.0f, 0.0f));
	fullTransformMatrix = projectionMatrix * translationMatrix * rotationMatrix;
	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);



	glFlush();
	glutPostRedisplay();
}

