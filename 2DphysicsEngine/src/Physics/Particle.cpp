#include "Particle.h"
#include <iostream>

Particle::Particle(float x, float y, float mass)
{
	this->position = Vec2(x, y);
	this->mass = mass;
	if(mass == 0)
		this->invMass = 0;
	else
		this->invMass = 1 / mass;
	std::cout << "Particle created at " << this->position.x << ", " << this->position.y << std::endl;
}

Particle::~Particle()
{
	std::cout << "Particle destroyed" << std::endl;
}

void Particle::Integrate(float dt)
{
	this->acceleration = this->sumForces * this->invMass;
	this->velocity += this->acceleration * dt;
	this->position += this->velocity * dt;

	this->ClearForces();
}

void Particle::AddForce(const Vec2& force)
{
	this->sumForces += force;
}

void Particle::ClearForces()
{
	this->sumForces = Vec2(0, 0);
}

