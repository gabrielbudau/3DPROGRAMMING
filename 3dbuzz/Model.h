#ifndef MODEL_H
#define MODEL_H

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Vector3.h"
#include "Texture.h"
#include "freeglut.h"

using std::string;
using std::vector;
using std::ifstream;
using std::istringstream;

struct Material
{
	float Ka[4];
	float Kd[4];
	float Ks[4];
	float Ke[4];

	float shininess;
	float alpha;
	float illum;

	Texture *ambientMap;
	Texture *diffuseMap;
	Texture *specularMap;
	Texture *emissionMap;
	Texture *shininessMap;
	Texture *transparencyMap;
	Texture *bumpMap;

	string name;

	Material()
	{
		float defaultAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		float defaultDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float defaultSpecular[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		float defaultEmission[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		memcpy(Ka, defaultAmbient, sizeof(float)* 4);
		memcpy(Kd, defaultDiffuse, sizeof(float)* 4);
		memcpy(Ks, defaultSpecular, sizeof(float)* 4);
		memcpy(Ke, defaultEmission, sizeof(float)* 4);
		ambientMap = NULL;
		diffuseMap = NULL;
		specularMap = NULL;
		emissionMap = NULL;
		shininessMap = NULL;
		transparencyMap = NULL;
		bumpMap = NULL;
		shininess = 2.0f;
		alpha = 1.0f;
		illum = 1.0f;
	}
};
struct Face
{
	Vector3 **vertices;
	int numVertices;

	Vector3 **normals;
	int numNormals;

	Vector3 **UVW;
	int numUVW;

	Material *material;

	Vector3 faceCenter;
	Vector3 faceNormal;

	Face()
	{
		vertices = NULL;
		normals = NULL;
		UVW = NULL;
	}
};

struct GroupObject
{
	vector<Face *> faces;
	string objectname;
	string groupName;
};

class Model
{
private:
	vector<GroupObject*> objects;

	vector<Vector3 *> vertices;
	vector<Vector3 *> normals;
	vector<Vector3 *> UVWs;

	vector<Material * > materials;
	Vector3 boundingPoints[8];
	float radius;
	Vector3 center;
	bool objectLoaded;
	GLuint displayList;

	string fileName;

public:
	Model(string _fileName = "");
	~Model();

	void		drawModel(void);
	void		drawObject(bool transparency = false);
	void		drawFace(Face _face);

	void		deleteObjects(void);

	bool		loadObject(string _fileName);
	void		loadMaterials(string _fileName);

	float		getRadius(void);
	Vector3		getCenter(void);
	string		getPath(void);

};

#endif
