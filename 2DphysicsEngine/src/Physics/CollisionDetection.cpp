#include "CollisionDetection.h"

#include <iostream>

bool CollisionDetection::IsColliding(Body* a, Body* b, Contact& contact)
{
	bool aIsCircle = a->shape->GetType() == CIRCLE;
	bool bIsCircle = b->shape->GetType() == CIRCLE;
	if (aIsCircle && bIsCircle)
	{
		return IsCollidingCircleCircle(a, b, contact);
	}
	return false;
}

bool CollisionDetection::IsCollidingCircleCircle(Body* a, Body* b, Contact& contact)
{
	CircleShape* circleA = (CircleShape*)a->shape;
	CircleShape* circleB = (CircleShape*)b->shape;

	Vec2 ab = b->position - a->position;
	float sumOfRadii = circleA->radius + circleB->radius;

	bool isColliding = ab.MagnitudeSquared() <= (sumOfRadii * sumOfRadii);

	if (!isColliding)
	{
		return false;
	}

	contact.a = a;
	contact.b = b;

	contact.normal = ab.Normalize();

	contact.start = b->position - contact.normal * circleB->radius;
	contact.end = a->position + contact.normal * circleA->radius;

	contact.depth = (contact.start - contact.end).Magnitude();

	return true;
}