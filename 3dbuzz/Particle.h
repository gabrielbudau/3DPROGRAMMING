#ifndef PARTICLE_H
#define PARTICLE_H

#include "GLEngine.h"
#include "Vector3.h"
#include <list>
class Particle
{
public:
	Particle(int id);
	void Update(long time);
	void Rotation(float angle, float &x, float &y);
public:
	long lastTime;
	int id;
	Vector3
		color,
		velocity,
		acceleration,
		rotation,
		position;

	float
		life,
		totalLife,
		alpha,
		size,
		bounciness;

	bool active;
};

#endif