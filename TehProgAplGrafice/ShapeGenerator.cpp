#include <ShapeGenerator.h>
#include <TeapotData.h>
#define NUM_ARRAY_ELEMENTS(a) sizeof(a) / sizeof(*a)

glm::vec3 randomColor()
{
	glm::vec3 ret;
	ret.x = rand() / (float)RAND_MAX;
	ret.y = rand() / (float)RAND_MAX;
	ret.z = rand() / (float)RAND_MAX;
	return ret;
}

ShapeData ShapeGenerator::makeTriangle()
{
	ShapeData ret;
	Vertex verts[] =
	{
		glm::vec3(+0.0f, +1.0f, +0.0f),
		glm::vec3(+1.0f, +0.0f, +0.0f),

		glm::vec3(-1.0f, -1.0f, +0.0f),
		glm::vec3(+0.0f, +1.0f, +0.0f),

		glm::vec3(+1.0f, -1.0f, +0.0f),
		glm::vec3(+0.0f, +0.0f, +1.0f)
	};

	ret.numVertices = NUM_ARRAY_ELEMENTS(verts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, verts, sizeof(verts));

	GLushort indices[] = { 0, 1, 2 };
	ret.numIndices = NUM_ARRAY_ELEMENTS(indices);
	ret.indices = new GLushort[ret.numIndices];
	memcpy(ret.indices, indices, sizeof(indices));
	return ret;
}

ShapeData ShapeGenerator::makeCube()
{
	ShapeData ret;
	Vertex stackVerts[] =
	{
		glm::vec3(-1.0f, +1.0f, +1.0f),// 0
		glm::vec3(+1.0f, +0.0f, +0.0f),// Color
		glm::vec3(+1.0f, +1.0f, +1.0f),// 1
		glm::vec3(+0.0f, +1.0f, +0.0f),// Color
		glm::vec3(+1.0f, +1.0f, -1.0f),// 2
		glm::vec3(+0.0f, +0.0f, +1.0f),// Color
		glm::vec3(-1.0f, +1.0f, -1.0f),// 3
		glm::vec3(+1.0f, +1.0f, +1.0f),// Color

		glm::vec3(-1.0f, +1.0f, -1.0f),// 4
		glm::vec3(+1.0f, +0.0f, +1.0f),// Color
		glm::vec3(+1.0f, +1.0f, -1.0f),// 5
		glm::vec3(+0.0f, +0.5f, +0.2f),// Color
		glm::vec3(+1.0f, -1.0f, -1.0f),// 6
		glm::vec3(+0.8f, +0.6f, +0.4f),// Color
		glm::vec3(-1.0f, -1.0f, -1.0f),// 7
		glm::vec3(+0.3f, +1.0f, +0.5f),// Color

		glm::vec3(+1.0f, +1.0f, -1.0f),// 8
		glm::vec3(+0.2f, +0.5f, +0.2f),// Color
		glm::vec3(+1.0f, +1.0f, +1.0f),// 9
		glm::vec3(+0.9f, +0.3f, +0.7f),// Color
		glm::vec3(+1.0f, -1.0f, +1.0f),// 10
		glm::vec3(+0.3f, +0.7f, +0.5f),// Color
		glm::vec3(+1.0f, -1.0f, -1.0f),// 11
		glm::vec3(+0.5f, +0.7f, +0.5f),// Color

		glm::vec3(-1.0f, +1.0f, +1.0f),// 12
		glm::vec3(+0.7f, +0.8f, +0.2f),// Color
		glm::vec3(-1.0f, +1.0f, -1.0f),// 13
		glm::vec3(+0.5f, +0.7f, +0.3f),// Color
		glm::vec3(-1.0f, -1.0f, -1.0f),// 14
		glm::vec3(+0.4f, +0.7f, +0.7f),// Color
		glm::vec3(-1.0f, -1.0f, +1.0f),// 15
		glm::vec3(+0.5f, +0.2f, +1.0f),// Color

		glm::vec3(+1.0f, +1.0f, +1.0f),// 16
		glm::vec3(+0.6f, +1.0f, +0.7f),// Color
		glm::vec3(-1.0f, +1.0f, +1.0f),// 17
		glm::vec3(+0.6f, +0.4f, +0.8f),// Color
		glm::vec3(-1.0f, -1.0f, +1.0f),// 18
		glm::vec3(+0.2f, +0.8f, +0.7f),// Color
		glm::vec3(+1.0f, -1.0f, +1.0f),// 19
		glm::vec3(+0.2f, +0.7f, +1.0f),// Color

		glm::vec3(+1.0f, -1.0f, -1.0f),// 20
		glm::vec3(+0.8f, +0.3f, +0.7f),// Color
		glm::vec3(-1.0f, -1.0f, -1.0f),// 21
		glm::vec3(+0.8f, +0.9f, +0.5f),// Color
		glm::vec3(-1.0f, -1.0f, +1.0f),// 22
		glm::vec3(+0.5f, +0.8f, +0.5f),// Color
		glm::vec3(+1.0f, -1.0f, +1.0f),// 23
		glm::vec3(+0.9f, +1.0f, +0.2f),// Color

	};
	ret.numVertices = NUM_ARRAY_ELEMENTS(stackVerts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, stackVerts, sizeof(stackVerts));

	GLushort stackIndices[] = {
		0, 1, 2, 0, 2, 3,// Top
		4, 5, 6, 4, 6, 7,// Front
		8, 9, 10, 8, 10, 11,// Right
		12, 13, 14, 12, 14, 15,// Left
		16, 17, 18, 16, 18, 19,// Back
		20, 22, 21, 20, 23, 22,// Bottom
	};
	ret.numIndices = NUM_ARRAY_ELEMENTS(stackIndices);
	ret.indices = new GLushort[ret.numIndices];
	memcpy(ret.indices, stackIndices, sizeof(stackIndices));
	return ret;
}

ShapeData ShapeGenerator::makeArrow()
{
	ShapeData ret;
	Vertex stackVerts[] =
	{
		// Top side of arrow head
		glm::vec3(+0.00f, +0.25f, -0.25f),         // 0
		glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
		glm::vec3(+0.50f, +0.25f, -0.25f),         // 1
		glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
		glm::vec3(+0.00f, +0.25f, -1.00f),         // 2
		glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
		glm::vec3(-0.50f, +0.25f, -0.25f),         // 3
		glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
		// Bottom side of arrow head
		glm::vec3(+0.00f, -0.25f, -0.25f),         // 4
		glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
		glm::vec3(+0.50f, -0.25f, -0.25f),         // 5
		glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
		glm::vec3(+0.00f, -0.25f, -1.00f),         // 6
		glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
		glm::vec3(-0.50f, -0.25f, -0.25f),         // 7
		glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
		// Right side of arrow tip
		glm::vec3(+0.50f, +0.25f, -0.25f),         // 8
		glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
		glm::vec3(+0.00f, +0.25f, -1.00f),         // 9
		glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
		glm::vec3(+0.00f, -0.25f, -1.00f),         // 10
		glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
		glm::vec3(+0.50f, -0.25f, -0.25f),         // 11
		glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
		// Left side of arrow tip
		glm::vec3(+0.00f, +0.25f, -1.00f),         // 12
		glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
		glm::vec3(-0.50f, +0.25f, -0.25f),         // 13
		glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
		glm::vec3(+0.00f, -0.25f, -1.00f),         // 14
		glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
		glm::vec3(-0.50f, -0.25f, -0.25f),         // 15
		glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
		// Back side of arrow tip
		glm::vec3(-0.50f, +0.25f, -0.25f),         // 16
		glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
		glm::vec3(+0.50f, +0.25f, -0.25f),         // 17
		glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
		glm::vec3(-0.50f, -0.25f, -0.25f),         // 18
		glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
		glm::vec3(+0.50f, -0.25f, -0.25f),         // 19
		glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
		// Top side of back of arrow
		glm::vec3(+0.25f, +0.25f, -0.25f),         // 20
		glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
		glm::vec3(+0.25f, +0.25f, +1.00f),         // 21
		glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
		glm::vec3(-0.25f, +0.25f, +1.00f),         // 22
		glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
		glm::vec3(-0.25f, +0.25f, -0.25f),         // 23
		glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
		// Bottom side of back of arrow
		glm::vec3(+0.25f, -0.25f, -0.25f),         // 24
		glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
		glm::vec3(+0.25f, -0.25f, +1.00f),         // 25
		glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
		glm::vec3(-0.25f, -0.25f, +1.00f),         // 26
		glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
		glm::vec3(-0.25f, -0.25f, -0.25f),         // 27
		glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
		// Right side of back of arrow
		glm::vec3(+0.25f, +0.25f, -0.25f),         // 28
		glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
		glm::vec3(+0.25f, -0.25f, -0.25f),         // 29
		glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
		glm::vec3(+0.25f, -0.25f, +1.00f),         // 30
		glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
		glm::vec3(+0.25f, +0.25f, +1.00f),         // 31
		glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
		// Left side of back of arrow
		glm::vec3(-0.25f, +0.25f, -0.25f),         // 32
		glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
		glm::vec3(-0.25f, -0.25f, -0.25f),         // 33
		glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
		glm::vec3(-0.25f, -0.25f, +1.00f),         // 34
		glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
		glm::vec3(-0.25f, +0.25f, +1.00f),         // 35
		glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
		// Back side of back of arrow
		glm::vec3(-0.25f, +0.25f, +1.00f),         // 36
		glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
		glm::vec3(+0.25f, +0.25f, +1.00f),         // 37
		glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
		glm::vec3(-0.25f, -0.25f, +1.00f),         // 38
		glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
		glm::vec3(+0.25f, -0.25f, +1.00f),         // 39
		glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
	};

	GLushort stackIndices[] = {
		0, 1, 2, // Top
		0, 2, 3,
		4, 6, 5, // Bottom
		4, 7, 6,
		8, 10, 9, // Right side of arrow tip
		8, 11, 10,
		12, 15, 13, // Left side of arrow tip
		12, 14, 15,
		16, 19, 17, // Back side of arrow tip
		16, 18, 19,
		20, 22, 21, // Top side of back of arrow
		20, 23, 22,
		24, 25, 26, // Bottom side of back of arrow
		24, 26, 27,
		28, 30, 29, // Right side of back of arrow
		28, 31, 30,
		32, 33, 34, // Left side of back of arrow
		32, 34, 35,
		36, 39, 37, // Back side of back of arrow
		36, 38, 39,
	};

	ret.numVertices = sizeof(stackVerts) / sizeof(*stackVerts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, stackVerts, sizeof(stackVerts));

	ret.numIndices = sizeof(stackIndices) / sizeof(*stackIndices);
	ret.indices = new GLushort[ret.numIndices];
	memcpy(ret.indices, stackIndices, sizeof(stackIndices));
	return ret;
}

ShapeData ShapeGenerator::makePlaneVerts(GLuint dimensions)
{
	ShapeData ret;
	ret.numVertices = dimensions * dimensions;
	int half = dimensions / 2;
	ret.vertices = new Vertex[ret.numVertices];
	for (int i = 0; i < dimensions; i++)
	{
		for (int j = 0; j < dimensions; j++)
		{
			Vertex& thisVert = ret.vertices[i * dimensions + j];
			thisVert.position.x = j - half;
			thisVert.position.z = i - half;
			thisVert.position.y = 0;
			thisVert.color = randomColor();
		}
	}
	return ret;
}

ShapeData ShapeGenerator::makePlaneIndices(GLuint dimensions)
{
	ShapeData ret;
	ret.numIndices = (dimensions - 1) * (dimensions - 1) * 2 * 3; // 2 triangles per square, 3 indices per triangle
	ret.indices = new unsigned short[ret.numIndices];
	int runner = 0;
	for (int row = 0; row < dimensions - 1; row++)
	{
		for (int col = 0; col < dimensions - 1; col++)
		{
			ret.indices[runner++] = dimensions * row + col;
			ret.indices[runner++] = dimensions * row + col + dimensions;
			ret.indices[runner++] = dimensions * row + col + dimensions + 1;

			ret.indices[runner++] = dimensions * row + col;
			ret.indices[runner++] = dimensions * row + col + dimensions + 1;
			ret.indices[runner++] = dimensions * row + col + 1;
		}
	}
	assert(runner = ret.numIndices);
	return ret;
}

ShapeData ShapeGenerator::makePlane(GLuint dimensions)
{
	ShapeData ret = makePlaneVerts(dimensions);
	ShapeData ret2 = makePlaneIndices(dimensions);
	ret.numIndices = ret2.numIndices;
	ret.indices = ret2.indices;
	return ret;
}

ShapeData ShapeGenerator::makeTeapot(GLuint tesselation, const glm::mat4& lidTransform)
{
	ShapeData ret;

	ret.numVertices = 32 * (tesselation + 1) * (tesselation + 1);
	GLuint faces = tesselation * tesselation * 32;
	float* vertices = new float[ret.numVertices * 3];
	float* normals = new float[ret.numVertices * 3];
	float* textureCoordinates = new float[ret.numVertices * 2];
	ret.numIndices = faces * 6;
	ret.indices = new unsigned short[ret.numIndices];

	generatePatches(vertices, normals, textureCoordinates, ret.indices, tesselation);
	moveLid(tesselation, vertices, lidTransform);

	// Adapt/convert their data format to mine
	ret.vertices = new Vertex[ret.numVertices];
	for (GLuint i = 0; i < ret.numVertices; i++)
	{
		Vertex& v = ret.vertices[i];
		v.position.x = vertices[i * 3 + 0];
		v.position.y = vertices[i * 3 + 1];
		v.position.z = vertices[i * 3 + 2];
		v.color = randomColor();
	}
	return ret;
}

void ShapeGenerator::generatePatches(float * v, float * n, float * tc, unsigned short* el, int grid) {
	float * B = new float[4 * (grid + 1)];  // Pre-computed Bernstein basis functions
	float * dB = new float[4 * (grid + 1)]; // Pre-computed derivitives of basis functions

	int idx = 0, elIndex = 0, tcIndex = 0;

	// Pre-compute the basis functions  (Bernstein polynomials)
	// and their derivatives
	computeBasisFunctions(B, dB, grid);

	// Build each patch
	// The rim
	buildPatchReflect(0, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
	// The body
	buildPatchReflect(1, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
	buildPatchReflect(2, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
	// The lid
	buildPatchReflect(3, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
	buildPatchReflect(4, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
	// The bottom
	buildPatchReflect(5, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
	// The handle
	buildPatchReflect(6, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
	buildPatchReflect(7, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
	// The spout
	buildPatchReflect(8, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
	buildPatchReflect(9, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);

	delete[] B;
	delete[] dB;
}

void ShapeGenerator::moveLid(int grid, float *v, glm::mat4 lidTransform) {

	int start = 3 * 12 * (grid + 1) * (grid + 1);
	int end = 3 * 20 * (grid + 1) * (grid + 1);

	for (int i = start; i < end; i += 3)
	{
		glm::vec4 vert = glm::vec4(v[i], v[i + 1], v[i + 2], 1.0f);
		vert = lidTransform * vert;
		v[i] = vert.x;
		v[i + 1] = vert.y;
		v[i + 2] = vert.z;
	}
}

void ShapeGenerator::buildPatchReflect(int patchNum,
	float *B, float *dB,
	float *v, float *n,
	float *tc, unsigned short *el,
	int &index, int &elIndex, int &tcIndex, int grid,
	bool reflectX, bool reflectY)
{
	glm::vec3 patch[4][4];
	glm::vec3 patchRevV[4][4];
	getPatch(patchNum, patch, false);
	getPatch(patchNum, patchRevV, true);

	// Patch without modification
	buildPatch(patch, B, dB, v, n, tc, el,
		index, elIndex, tcIndex, grid, glm::mat3(1.0f), true);

	// Patch reflected in x
	if (reflectX) {
		buildPatch(patchRevV, B, dB, v, n, tc, el,
			index, elIndex, tcIndex, grid, glm::mat3(glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)), false);
	}

	// Patch reflected in y
	if (reflectY) {
		buildPatch(patchRevV, B, dB, v, n, tc, el,
			index, elIndex, tcIndex, grid, glm::mat3(glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)), false);
	}

	// Patch reflected in x and y
	if (reflectX && reflectY) {
		buildPatch(patch, B, dB, v, n, tc, el,
			index, elIndex, tcIndex, grid, glm::mat3(glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)), true);
	}
}

void ShapeGenerator::buildPatch(glm::vec3 patch[][4],
	float *B, float *dB,
	float *v, float *n, float *tc,
	unsigned short *el,
	int &index, int &elIndex, int &tcIndex, int grid, glm::mat3 reflect,
	bool invertNormal)
{
	int startIndex = index / 3;
	float tcFactor = 1.0f / grid;

	for (int i = 0; i <= grid; i++)
	{
		for (int j = 0; j <= grid; j++)
		{
			glm::vec3 pt = reflect * evaluate(i, j, B, patch);
			glm::vec3 norm = reflect * evaluateNormal(i, j, B, dB, patch);
			if (invertNormal)
				norm = -norm;

			v[index] = pt.x;
			v[index + 1] = pt.y;
			v[index + 2] = pt.z;

			n[index] = norm.x;
			n[index + 1] = norm.y;
			n[index + 2] = norm.z;

			tc[tcIndex] = i * tcFactor;
			tc[tcIndex + 1] = j * tcFactor;

			index += 3;
			tcIndex += 2;
		}
	}

	for (int i = 0; i < grid; i++)
	{
		int iStart = i * (grid + 1) + startIndex;
		int nextiStart = (i + 1) * (grid + 1) + startIndex;
		for (int j = 0; j < grid; j++)
		{
			el[elIndex] = iStart + j;
			el[elIndex + 1] = nextiStart + j + 1;
			el[elIndex + 2] = nextiStart + j;

			el[elIndex + 3] = iStart + j;
			el[elIndex + 4] = iStart + j + 1;
			el[elIndex + 5] = nextiStart + j + 1;

			elIndex += 6;
		}
	}
}

void ShapeGenerator::getPatch(int patchNum, glm::vec3 patch[][4], bool reverseV)
{
	for (int u = 0; u < 4; u++) {          // Loop in u direction
		for (int v = 0; v < 4; v++) {     // Loop in v direction
			if (reverseV) {
				patch[u][v] = glm::vec3(
					Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + (3 - v)]][0],
					Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + (3 - v)]][1],
					Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + (3 - v)]][2]
					);
			}
			else {
				patch[u][v] = glm::vec3(
					Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + v]][0],
					Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + v]][1],
					Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + v]][2]
					);
			}
		}
	}
}

void ShapeGenerator::computeBasisFunctions(float * B, float * dB, int grid) {
	float inc = 1.0f / grid;
	for (int i = 0; i <= grid; i++)
	{
		float t = i * inc;
		float tSqr = t * t;
		float oneMinusT = (1.0f - t);
		float oneMinusT2 = oneMinusT * oneMinusT;

		B[i * 4 + 0] = oneMinusT * oneMinusT2;
		B[i * 4 + 1] = 3.0f * oneMinusT2 * t;
		B[i * 4 + 2] = 3.0f * oneMinusT * tSqr;
		B[i * 4 + 3] = t * tSqr;

		dB[i * 4 + 0] = -3.0f * oneMinusT2;
		dB[i * 4 + 1] = -6.0f * t * oneMinusT + 3.0f * oneMinusT2;
		dB[i * 4 + 2] = -3.0f * tSqr + 6.0f * t * oneMinusT;
		dB[i * 4 + 3] = 3.0f * tSqr;
	}
}

glm::vec3 ShapeGenerator::evaluate(int gridU, int gridV, float *B, glm::vec3 patch[][4])
{
	glm::vec3 p(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			p += patch[i][j] * B[gridU * 4 + i] * B[gridV * 4 + j];
		}
	}
	return p;
}

glm::vec3 ShapeGenerator::evaluateNormal(int gridU, int gridV, float *B, float *dB, glm::vec3 patch[][4])
{
	glm::vec3 du(0.0f, 0.0f, 0.0f);
	glm::vec3 dv(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			du += patch[i][j] * dB[gridU * 4 + i] * B[gridV * 4 + j];
			dv += patch[i][j] * B[gridU * 4 + i] * dB[gridV * 4 + j];
		}
	}
	return glm::normalize(glm::cross(du, dv));
}

