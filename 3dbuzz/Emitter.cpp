#include "Emitter.h"

Emitter::Emitter()
{
	lastTime = -1;
	texture = NULL;
	emissionRate = 30.0f;
	emissionRadius = 0.0f;

	life = 2.0f;
	lifeRange = 0.5f;

	size = 5.0f;
	sizeRange = 2.0f;

	saturation = 1.0f;
	alpha = 0.5f;
	
	spread = 1.0f;
	gravity = 0.0f;
}
Emitter::~Emitter()
{
	for (list<Particle *>::iterator it = particles.begin(); it != particles.end(); it++)
	{
		delete (*it);
	}
}
void Emitter::Update(long time)
{
	if (!texture)
		return;
	if (lastTime == -1)
		lastTime = time;

	int numEmission = (int)((float)(time - lastTime) / 1000.0f * emissionRate);

	for (int i= 0; i < numEmission; i++)
	{
		addParticle();
	}

	if (numEmission > 0)
		lastTime = time;

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->texID);

	Particle *particle = NULL;
	for (list<Particle *>::iterator it = particles.begin(); it != particles.end(); it++)
	{
		particle = (*it);

		particle->acceleration.y = -gravity;
		particle->acceleration += wind;
		particle->alpha = alpha;
		particle->rotation = rotation;

		particle->Update(time);

		if (particle->active == false)
		{
			delete particle;

			list <Particle *> ::iterator pTemp = it--;
			particles.erase(pTemp);
		}
	}
	glMatrixMode(GL_MODELVIEW);

	glPopMatrix();
}
void Emitter::setTexture(Texture *_texture)
{
	texture = _texture;
}
void Emitter::addParticle(void)
{
	Particle *particle = new Particle((int)particles.size());
	float r = fRand()*saturation + (1 - saturation);
	float g = fRand()*saturation + (1 - saturation);
	float b = fRand()*saturation + (1 - saturation);

	particle->color = Vector3(r, g, b);

	particle->life = fRand(life - lifeRange, life + lifeRange);
	particle->totalLife = particle->life;

	particle->velocity = Vector3(fRand(-spread, spread), fRand(3, 20), fRand(-spread, spread));

	particle->acceleration = Vector3(0.0f, -gravity, 0.0f);
	particle->size = fRand(size - sizeRange, size + sizeRange);

	particle->position.x = fRand(-emissionRadius, emissionRadius);
	particle->position.z = fRand(-emissionRadius, emissionRadius);

	particles.push_back(particle);
}
float Emitter::fRand(float start, float end)
{
	return start + (end - start)*((float)rand() / (float)RAND_MAX);
}