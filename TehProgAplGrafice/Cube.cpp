#include "Cube.h"


Cube::Cube()
{
	data = ShapeGenerator::makeCube();
	numIndices = data.numIndices;
	bufferSize = data.vertexBufferSize() + data.indexBufferSize();
}


Cube::~Cube()
{
}

GLuint Cube::sendDataToOpenGL(GLuint offset, GLuint theBufferID)
{
	glBufferSubData(GL_ARRAY_BUFFER, offset, data.vertexBufferSize(), data.vertices);
	offset += data.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, offset, data.indexBufferSize(), data.indices);
	indexDataByteOffset = offset;
	offset += data.indexBufferSize();
	numIndices = data.numIndices;

	glGenVertexArrays(1, &vertexArrayObjectID);
	glBindVertexArray(vertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)* 6, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)* 6, (void*)(sizeof(GLfloat)* 3));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);
	data.cleanUp();
	return offset;
}

void Cube::drawCube(glm::mat4 worldToProjectionMatrix, GLuint fullTransformUniformLocation)
{
	glBindVertexArray(vertexArrayObjectID);
	glm::mat4 fullTransforMatrix = worldToProjectionMatrix * modelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransforMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, (void*)indexDataByteOffset);
}