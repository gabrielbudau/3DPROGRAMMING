#pragma once
#include <glew.h>
#include <freeglut.h>
#include <Vertex.h>
#include <ShapeGenerator.h>
#include <glm.hpp>
#include <gtx\transform.hpp>
#include <gtc\matrix_transform.hpp>
class Cube
{


public:
	glm::mat4 modelToWorldMatrix;
	GLuint bufferSize;
	GLuint numIndices, vertexArrayObjectID, indexDataByteOffset;
	ShapeData data;
	Cube();
	~Cube();
	GLuint sendDataToOpenGL(GLuint offset, GLuint theBufferID);
	void drawCube(glm::mat4 worldToProjectionMatrix, GLuint fullTransformUniformLocation);

};

