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

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, tex_id);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, (const GLvoid *)texData);

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

bool loadPngImage(char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData) {
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read = 0;
	int color_type, interlace_type;
	FILE *fp;

	if ((fp = fopen(name, "rb")) == NULL)
		return false;

	/* Create and initialize the png_struct
	* with the desired error handler
	* functions.  If you want to use the
	* default stderr and longjump method,
	* you can supply NULL for the last
	* three parameters.  We also supply the
	* the compiler header file version, so
	* that we know if the application
	* was compiled with a compatible version
	* of the library.  REQUIRED
	*/
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
		NULL, NULL, NULL);

	if (png_ptr == NULL) {
		fclose(fp);
		return false;
	}

	/* Allocate/initialize the memory
	* for image information.  REQUIRED. */
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return false;
	}

	/* Set error handling if you are
	* using the setjmp/longjmp method
	* (this is the normal method of
	* doing things with libpng).
	* REQUIRED unless you  set up
	* your own error handlers in
	* the png_create_read_struct()
	* earlier.
	*/
	if (setjmp(png_jmpbuf(png_ptr))) {
		/* Free all of the memory associated
		* with the png_ptr and info_ptr */
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		/* If we get here, we had a
		* problem reading the file */
		return false;
	}

	/* Set up the output control if
	* you are using standard C streams */
	png_init_io(png_ptr, fp);

	/* If we have already
	* read some of the signature */
	png_set_sig_bytes(png_ptr, sig_read);

	/*
	* If you have enough memory to read
	* in the entire image at once, and
	* you need to specify only
	* transforms that can be controlled
	* with one of the PNG_TRANSFORM_*
	* bits (this presently excludes
	* dithering, filling, setting
	* background, and doing gamma
	* adjustment), then you can read the
	* entire image (including pixels)
	* into the info structure with this
	* call
	*
	* PNG_TRANSFORM_STRIP_16 |
	* PNG_TRANSFORM_PACKING  forces 8 bit
	* PNG_TRANSFORM_EXPAND forces to
	*  expand a palette into RGB
	*/
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

	png_uint_32 width, height;
	int bit_depth;
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
		&interlace_type, NULL, NULL);
	outWidth = width;
	outHeight = height;

	unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
	*outData = (unsigned char*)malloc(row_bytes * outHeight);

	png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

	for (int i = 0; i < outHeight; i++) {
		// note that png is ordered top to
		// bottom, but OpenGL expect it bottom to top
		// so the order or swapped
		memcpy(*outData + (row_bytes * (outHeight - 1 - i)), row_pointers[i], row_bytes);
	}

	/* Clean up after the read,
	* and free any memory allocated */
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	/* Close the file */
	fclose(fp);

	/* That's it */
	return true;
}