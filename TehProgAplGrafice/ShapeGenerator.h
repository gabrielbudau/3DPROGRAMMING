#pragma once
#include <ShapeData.h>
#include <Vertex.h>
#include <glm.hpp>

class ShapeGenerator
{
	static ShapeData makePlaneVerts(GLuint dimensions);
	static ShapeData makePlaneIndices(GLuint dimensions);

public:
	static ShapeData makeTriangle();
	static ShapeData makeCube();
	static ShapeData makeArrow();
	static ShapeData makePlane(GLuint dimensions = 10);
};

