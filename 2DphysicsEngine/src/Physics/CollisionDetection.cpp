#include "CollisionDetection.h"

#include <iostream>
#include <limits>

bool CollisionDetection::IsColliding(Body* a, Body* b, Contact& contact)
{
	bool aIsCircle = a->shape->GetType() == CIRCLE;
	bool bIsCircle = b->shape->GetType() == CIRCLE;
	bool aIsPolygon = a->shape->GetType() == POLYGON || a->shape->GetType() == BOX;
	bool bIsPolygon = b->shape->GetType() == POLYGON || b->shape->GetType() == BOX;


	if (aIsCircle && bIsCircle)
	{
		return IsCollidingCircleCircle(a, b, contact);
	}
	else if (aIsPolygon && bIsPolygon)
	{
		return IsCollidingPolygonPolygon(a, b, contact);
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


bool CollisionDetection::IsCollidingPolygonPolygon(Body* a, Body* b, Contact& contact)
{
	const PolygonShape* aPolygonShape = (PolygonShape*)a->shape;
	const PolygonShape* bPolygonShape = (PolygonShape*)b->shape;
	Vec2 aAxis, bAxis;
	Vec2 aPoint, bPoint;

	float abSeparation = aPolygonShape->FindMinSeparation(bPolygonShape, aAxis, aPoint);
	if (abSeparation > 0.0f)
	{
		return false;
	}

	float baSeparation = bPolygonShape->FindMinSeparation(aPolygonShape, bAxis, bPoint);
	if (baSeparation > 0.0f)
	{
		return false;
	}

	contact.a = a;
	contact.b = b;

	if (abSeparation > baSeparation)
	{
		contact.normal = aAxis.Normal();
		contact.depth = - abSeparation;
		contact.start = aPoint;
		contact.end = aPoint + contact.normal * contact.depth;
	}
	else
	{
		contact.normal = - bAxis.Normal();
		contact.depth = - baSeparation;
		contact.start = bPoint - contact.normal * contact.depth;
		contact.end = bPoint;
	}


	return true;
}