#ifndef LIGHT_H
#define LIGHT_H

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <glew.h>
#include <freeglut.h>
#include <vector>

using std::vector;
enum LIGHT_TYPE
{
	LIGHT_SPOT,
	LIGHT_POINT,
	LIGHT_DIRECTIONAL
};

class Light
{
private:
	GLfloat			position[3];
	GLfloat			diffuse[4];
	GLfloat			ambient[4];
	GLfloat			specularity[4];
	GLfloat			spotDirection[3];
	float			cutoff;
	float			exponent;
	bool			visible;
	int				lightType;
	int				lightNum;
public:
	static void Initialise(void);
	static vector<int> availableLights;
	static vector<Light *> lights;
	static int numLights;
	void Visible(bool value = true);

	void setLightType(LIGHT_TYPE lightType);
	void setAmbient(float r, float g, float b, float a);
	void setDiffuse(float r, float g, float b, float a);
	void setSpecular(float r, float g, float b, float a);
	void updateLight();
	void setPosition(float x, float y, float z);
	void setSpotDirection(float x, float y, float z);
	void setCutOff(float value);
	void setExponent(float value);
	void setAttenuation(float constant, float linear, float quadratic);
	int getLightNum(void);

	Light(LIGHT_TYPE lightType);
	~Light();
};

#endif

