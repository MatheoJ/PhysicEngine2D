#include "Body.h"
#include <iostream>
#include <math.h>

Body::Body(const Shape& shape, float x, float y, float mass)
{
	this->shape=shape.Clone();
	this->position = Vec2(x, y);
	this->velocity = Vec2(0, 0);
	this->acceleration = Vec2(0, 0);
	this->sumForces = Vec2(0, 0);
	this->rotation = 0;
	this->angularVelocity = 0;
	this->angularAcceleration = 0;
	this->sumTorque = 0;
	this->restitution = 1.0;

	this->mass = mass;
	if(mass == 0)
		this->invMass = 0;
	else
		this->invMass = 1 / mass;

	I = shape.GetMomentOfInertia()* mass;
	if(I == 0)
		this->invI = 0;
	else
		this->invI = 1 / I;

	std::cout << "Body created at " << this->position.x << ", " << this->position.y << std::endl;
}

Body::~Body()
{
	delete shape;
	std::cout << "Body destroyed" << std::endl;
}

void Body::ApplyImpulse(const Vec2& impulse)
{
	if(this->IsStatic())
		return;

	this->velocity += impulse * this->invMass;
}

void Body::IntegrateLinear(float dt)
{
	if (this->IsStatic())
		return;

	this->acceleration = this->sumForces * this->invMass;
	this->velocity += this->acceleration * dt;
	this->position += this->velocity * dt;

	this->ClearForces();
}

void Body::IntegrateAngular(float dt)
{
	if(this->IsStatic())
		return;

	this->angularAcceleration = this->sumTorque * this->invI;
	this->angularVelocity += this->angularAcceleration * dt;
	this->rotation += this->angularVelocity * dt;

	this->ClearTorque();
}

void Body::AddForce(const Vec2& force)
{
	this->sumForces += force;
}

void Body::AddTorque(float torque)
{
	this->sumTorque += torque;
}

void Body::ClearForces()
{
	this->sumForces = Vec2(0, 0);
}

void Body::ClearTorque()
{
	this->sumTorque = 0;
}

void Body::Update(float dt)
{
	this->IntegrateLinear(dt);
	this->IntegrateAngular(dt);
	bool isPolygon = (this->shape->GetType() == POLYGON || this->shape->GetType() == BOX);
	if (isPolygon) {
		PolygonShape* polygon = (PolygonShape*)this->shape;
		polygon->UpdateVertices(this->rotation, this->position);
	}
}

bool Body::IsStatic() const
{
	float epsilon = 0.005;
	return fabs(invMass - 0.0) < epsilon;
}
