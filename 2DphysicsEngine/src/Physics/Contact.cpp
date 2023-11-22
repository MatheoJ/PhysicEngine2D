#include "Contact.h"
#include <iostream>

void Contact::ResolvePenetration()
{
	if(a->IsStatic() && b->IsStatic())
		return;

	float da = depth / (a->invMass + b->invMass) * a->invMass;
	float db = depth / (a->invMass + b->invMass) * b->invMass;

	a->position -= normal * da;
	b->position += normal * db;
}

void Contact::ResolveCollision()
{
	ResolvePenetration();

	float e = std::min(a->restitution, b->restitution);

	const Vec2 vre1 = a->velocity - b->velocity;

	float vre1DotNormal = vre1.Dot(normal);

	const Vec2 impulseDirection = normal;
	const float impulseMagnitude = -(1 + e) * vre1DotNormal / (a->invMass + b->invMass);

	Vec2 j = impulseDirection * impulseMagnitude;
	
	a->ApplyImpulse(j);
	b->ApplyImpulse(-j);

	std::cout << "Impulse: " << j.x << ", " << j.y << std::endl;
}
