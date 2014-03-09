#ifndef EMITTER_H
#define EMITTER_H

#include"Particle.h"

using std::list;
using std::vector;

class Emitter
{
public:
	Emitter();
	~Emitter();
	void Update(long time); 
	void setTexture(Texture *texture);
protected:
	void addParticle(void);
	float fRand(float start = 0.0f, float end = 1.0f);
public:
	vector<Particle *>		particles;

	Texture					*texture;

	long					lastTime;
	//Attributes
	//==================================
	float
		emissionRate,
		emissionRadius,

		life,
		lifeRange,

		size,
		sizeRange,

		saturation,
		alpha,

		spread,
		gravity;

	Vector3
		position,
		wind,
		rotation;
};

#endif
