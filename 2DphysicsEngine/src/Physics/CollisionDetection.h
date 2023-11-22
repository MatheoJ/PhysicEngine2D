#pragma once
#include "Body.h"
#include "Contact.h"

struct CollisionDetection
{
	static bool IsColliding(Body* a, Body* b, Contact& contact);
	static bool IsCollidingCircleCircle(Body* a, Body* b, Contact& contact);
	static bool IsCollidingPolygonPolygon(Body* a, Body* b, Contact& contact);
};