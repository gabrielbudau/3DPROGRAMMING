#pragma once
#include <ShapeData.h>
#include <Vertex.h>
#include <glm.hpp>

class ShapeGenerator
{
public:
	static ShapeData makeTriangle();
	static ShapeData makeCube();
};

