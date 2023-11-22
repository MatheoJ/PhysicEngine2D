#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vec2.h"
#include "Shape.h"

struct Body
{
	bool isColiding = false;

	Vec2 position;
	Vec2 velocity;
	Vec2 acceleration;
	Vec2 sumForces;

	float mass;
	float invMass;

	float rotation;
	float angularVelocity;
	float angularAcceleration;
	float sumTorque;

	//Moment of inertia
	float I;
	float invI;

	float restitution;

	Shape* shape = nullptr;

	Body(const Shape& shape,float x, float y, float mass);
	~Body();


	bool IsStatic() const;

	void AddForce(const Vec2& force);
	void AddTorque(float torque);
	void ClearForces();
	void ClearTorque();

	void ApplyImpulse(const Vec2& impulse);

	void IntegrateLinear(float dt);
	void IntegrateAngular(float dt);

	void Update(float dt);
};

#endif