#include "Model.h"


Model::Model(string _fileName)
{
	objectLoaded = false;

	if (_fileName != "")
	{
		loadObject(_fileName);
	}
	displayList = 0;
}
Model::~Model()
{
	deleteObjects();
}
void		Model::drawModel(void)
{
	if (!objectLoaded)
		return;

	if (displayList != 0)
	{
		glCallList(displayList);
		return;
	}

	displayList = glGenLists(1);

	glNewList(displayList, GL_COMPILE_AND_EXECUTE);

	drawObject(false);
	drawObject(true);

	glEndList();

}
void		Model::drawObject(bool transparency)
{
	Material *lastMaterial = NULL;
	for (int i = 0; i < (int)objects.size(); i++)
	{
		GroupObject *object = objects[i];
		for (int j = 0; j < (int)object->faces.size(); j++)
		{
			Face *face = object->faces[j];
			Material *material = face->material;

			if (material != lastMaterial)
			{
				if (transparency == false && material->alpha < 1.0f)
					continue;
				material->Kd[3] = material->alpha;
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat*)material->Ka);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat*)material->Kd);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat*)material->Ks);
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, (GLfloat*)material->Ke);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material->shininess);

				if (material->diffuseMap != NULL)
				{
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, material->diffuseMap->texID);
				}
				else{
					glDisable(GL_TEXTURE_2D);
				}
				lastMaterial = material;
			}
			drawFace(*face);
		}

	}
}
void		Model::drawFace(Face _face)
{
	if ((int)_face.numVertices <= 3)
		glBegin(GL_TRIANGLES);
	else
		glBegin(GL_POLYGON);

	for (int i = 0; i < _face.numVertices; i++)
	{
		if ((int)_face.numUVW > i && _face.UVW != NULL)
			glTexCoord2f(_face.UVW[i]->x, _face.UVW[i]->y);
		if ((int)_face.numNormals > i && _face.numNormals != NULL)
			glNormal3f(_face.normals[i]->x, _face.normals[i]->y, _face.normals[i]->z);
		if ((int)_face.numVertices > i && _face.numVertices != NULL)
			glVertex3f(_face.vertices[i]->x, _face.vertices[i]->y, _face.vertices[i]->z);
	}
	glEnd();
}
void		Model::deleteObjects(void)
{
	for (int i = 0; i < (int)materials.size(); i++)
	{
		if (materials[i]->ambientMap != NULL)
			delete materials[i]->ambientMap;
		if (materials[i]->diffuseMap != NULL)
			delete materials[i]->diffuseMap;
		if (materials[i]->specularMap != NULL)
			delete materials[i]->specularMap;
		if (materials[i]->emissionMap != NULL)
			delete materials[i]->emissionMap;
		if (materials[i]->shininessMap != NULL)
			delete materials[i]->shininessMap;
		if (materials[i]->transparencyMap != NULL)
			delete materials[i]->transparencyMap;
		if (materials[i]->bumpMap != NULL)
			delete materials[i]->bumpMap;


		delete materials[i];
	}
	for (int i = 0; i < (int)normals.size(); i++)
		delete normals[i];
	for (int i = 0; i < (int)UVWs.size(); i++)
		delete UVWs[i];
	for (int i = 0; i < (int)vertices.size(); i++)
		delete vertices[i];
	for (int i = 0; i < (int)objects.size(); i++)
	{
		GroupObject *object = objects[i];
		for (int j = 0; j < (int)object->faces.size(); j++)
		{
			delete object->faces[j]->vertices;
			delete object->faces[j]->normals;
			delete object->faces[j]->UVW;
			delete object->faces[j];
		}
	}
	UVWs.clear();
	normals.clear();
	vertices.clear();
	objects.clear();
	materials.clear();
}
bool		Model::loadObject(string _fileName)
{
	fileName = _fileName;
	ifstream istr(fileName.data());
	if (!istr)
		return false;
	deleteObjects();
	displayList = 0;
	objectLoaded = false;

	GroupObject *defaulObject = new GroupObject();
	GroupObject *currentObject = defaulObject;

	objects.push_back(defaulObject);

	char path[256];
	strcpy_s(path, _fileName.data());
	for (int i = (int)_fileName.length(); path[i] != '\\' && path[i] != '/'; i--)
		path[i] = 0;

	Material *currentMaterial = NULL;
	char line[256];

	while (istr.getline(line, 256))
	{
		istringstream newLine(line, istringstream::in);
		string firstWord;//command
		newLine >> firstWord;
		if (firstWord == "#")
		{

		}
		else if (firstWord == "mtllib")
		{
			string materialFileName;
			while (newLine >> materialFileName)
				loadMaterials(string(path) + materialFileName);
		}
		else if (firstWord == "usemtl")
		{
			string materialName;
			newLine >> materialName;
			for (int i = 0; i < (int)materials.size(); i++)
			{
				if (materials[i]->name == materialName){
					currentMaterial = materials[i];
				}
			}
		}
		else if (firstWord == "v")
		{
			Vector3 *vertex = new Vector3;
			newLine >> vertex->x >> vertex->y >> vertex->z;
			vertices.push_back(vertex);
		}
		else if (firstWord == "vt")
		{
			Vector3 *uvw = new Vector3;
			newLine >> uvw->x >> uvw->y >> uvw->z;
			UVWs.push_back(uvw);
		}
		else if (firstWord == "vn")
		{
			Vector3 *normal = new Vector3;
			newLine >> normal->x >> normal->y >> normal->z;
			normals.push_back(normal);
		}
		else if (firstWord == "g")
		{
			string objectName;
			newLine >> objectName;
			if (objectName == "default")
				currentObject = defaulObject;
			else{
				GroupObject *object = new GroupObject();
				object->objectname = objectName;
				newLine >> object->groupName;
				currentObject = object;
				objects.push_back(object);
			}
		}
		else if (firstWord == "f")
		{
			Face *newFace = new Face();
			newFace->material = currentMaterial;

			currentObject->faces.push_back(newFace);

			vector<Vector3 *> tempVertices;
			vector<Vector3 *> tempNormals;
			vector<Vector3 *> tempUVW;

			while (!newLine.eof())
			{
				char temp;
				int normal = 0; int vertex = 0; int uvw = 0;

				bool noUV = false;

				if ((int)vertices.size() > 0)
				{
					newLine >> vertex;
					
					while (newLine.get(temp))
					{
						if (temp == '/' || temp == ' ')
							break;
					}
					newLine.get(temp);
					if (temp == '/')
						noUV = true;
					else
						newLine.unget();
					tempVertices.push_back(vertices[--vertex]);
				}

				if ((int)UVWs.size() > 0 && noUV == false)
				{
					newLine >> uvw;

					while (newLine.get(temp))
					{
						if (temp == '/' || temp == ' ')
							break;
					}
					tempUVW.push_back(UVWs[--uvw]);
				}

				if ((int)normals.size() > 0)
				{
					newLine >> normal;

					while (newLine.get(temp))
					{
						if (temp == '/' || temp == ' ')
							break;
					}
					tempNormals.push_back(normals[--normal]);
				}
			}
			newFace->numVertices = (int)tempVertices.size();
			newFace->numNormals = (int)tempNormals.size();
			newFace->numUVW = (int)tempUVW.size();

			newFace->vertices = new Vector3 *[newFace->numVertices];
			newFace->normals = new Vector3 *[newFace->numNormals];
			newFace->UVW = new Vector3 *[newFace->numUVW];

			for (int v = 0; v < newFace->numVertices; v++)
				newFace->vertices[v] = tempVertices[v];

			for (int n = 0; n < newFace->numNormals; n++)
				newFace->normals[n] = tempNormals[n];

			for (int u = 0; u < newFace->numUVW; u++)
				newFace->UVW[u] = tempUVW[u];

			for (int v = 0; v < newFace->numVertices; v++)
				newFace->faceCenter += (*tempVertices[v]);
			newFace->faceCenter /= (float)newFace->numVertices;

			if (newFace->numVertices >= 3)
			{
				Vector3 vector1 = ((*newFace->vertices[0]) - (*newFace->vertices[1])).Normalize();
				Vector3 vector2 = ((*newFace->vertices[0]) - (*newFace->vertices[2])).Normalize();
				newFace->faceNormal = vector1 * vector2;
			}


		}

	}
	float xmin, xmax, ymin, ymax, zmin, zmax;
	center = Vector3(0.0, 0.0, 0.0);
	for (int n = 0; n < (int)vertices.size(); n++)
	{
		if (n == 0)
		{
			xmin = xmax = vertices[n]->x;
			ymin = ymax = vertices[n]->y;
			zmin = zmax = vertices[n]->z;
			continue;
		}
		if (vertices[n]->x < xmin) xmin = vertices[n]->x;
		if (vertices[n]->x > xmax) xmax = vertices[n]->x;
		if (vertices[n]->y < ymin) ymin = vertices[n]->y;
		if (vertices[n]->y > ymax) ymax = vertices[n]->y;
		if (vertices[n]->z < zmin) zmin = vertices[n]->z;
		if (vertices[n]->z > zmax) zmax = vertices[n]->z;

		center += (*vertices[n]);
	}
	center /= vertices.size();
	boundingPoints[0] = Vector3(xmax, ymin, zmin);
	boundingPoints[1] = Vector3(xmin, ymax, zmin);
	boundingPoints[2] = Vector3(xmin, ymin, zmax);

	boundingPoints[3] = Vector3(xmin, ymax, zmax);
	boundingPoints[4] = Vector3(xmax, ymax, zmin);
	boundingPoints[5] = Vector3(xmax, ymin, zmax);

	boundingPoints[6] = Vector3(xmin, ymin, zmin);
	boundingPoints[0] = Vector3(xmax, ymax, zmax);

	radius = (Vector3(xmax, ymax, zmax) - Vector3(xmin, ymin, zmin)).Length() / 2.0f;
	objectLoaded = true;
	return true;
}
void		Model::loadMaterials(string _fileName)
{
	ifstream istr(_fileName.data());
	if (!istr)
		return;

	char path[256];
	strcpy_s(path, _fileName.data());
	for (int i = (int)_fileName.length(); path[i] != '\\' && path[i] != '/'; i--)
		path[i] = 0;

	Material *material = NULL;
	char line[256];
	while (istr.getline(line, 256))
	{
		istringstream newLine(line, istringstream::in);

		string firstWord;
		newLine >> firstWord;
		if (firstWord == "newmtl")
		{
			material = new Material;
			materials.push_back(material);
			newLine >> material->name;
		}
		else if (firstWord == "illum")
		{
			newLine >> material->illum;
		}
		else if (firstWord == "Ka")
		{
			newLine >> material->Ka[0] >> material->Ka[1] >> material->Ka[2];
		}
		else if (firstWord == "Kd")
		{
			newLine >> material->Kd[0] >> material->Kd[1] >> material->Kd[2];
		}
		else if (firstWord == "Ks")
		{
			newLine >> material->Ks[0] >> material->Ks[1] >> material->Ks[2];
		}
		else if (firstWord == "Ke")
		{
			newLine >> material->Ke[0] >> material->Ke[1] >> material->Ke[2];
		}
		else if (firstWord == "Ns")
		{
			newLine >> material->shininess;
		}
		else if (firstWord == "d" || firstWord == "Tr")
		{
			newLine >> material->alpha;
		}
		else if (firstWord == "Tf")
		{
			Vector3 color;
			newLine >> color.x >> color.y >> color.z;
			material->alpha = (color.x + color.y + color.z) / 3.0f;
		}
		else if (firstWord == "map_Ka")
		{
			string fileN;
			newLine >> fileN;
			Texture *map = new Texture(string(path) + fileN);
			material->ambientMap = map;
		}
		else if (firstWord == "map_Kd")
		{
			string fileN;
			newLine >> fileN;
			Texture *map = new Texture(string(path) + fileN);
			material->diffuseMap = map;
		}
		else if (firstWord == "map_Ks")
		{
			string fileN;
			newLine >> fileN;
			Texture *map = new Texture(string(path) + fileN);
			material->specularMap = map;
		}
		else if (firstWord == "map_Ke")
		{
			string fileN;
			newLine >> fileN;
			Texture *map = new Texture(string(path) + fileN);
			material->emissionMap = map;
		}
		else if (firstWord == "map_Ns")
		{
			string fileN;
			newLine >> fileN;
			Texture *map = new Texture(string(path) + fileN);
			material->shininessMap = map;
		}
		else if (firstWord == "map_d")
		{
			string fileN;
			newLine >> fileN;
			Texture *map = new Texture(string(path) + fileN);
			material->transparencyMap = map;
		}
		else if (firstWord == "map_Bump")
		{
			string fileN;
			newLine >> fileN;
			Texture *map = new Texture(string(path) + fileN);
			material->bumpMap = map;
		}
	}
}
float		Model::getRadius(void)
{
	return radius;
}
Vector3		Model::getCenter(void)
{
	return center;
}
string		Model::getPath(void)
{
	return fileName;
}