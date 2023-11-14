#pragma once

#include "Vec2.h"
#include "Body.h"

struct Force
{
	static Vec2 GenerateDragForce(const Body& body, float k);
	static Vec2 GenerateFrictionForce(const Body& body, float k);
	static Vec2 GenerateGravitaionalForce(const Body& a, const Body& b, float G, float minDistance, float maxDistance);
	static Vec2 GenerateSpringForce(const Body& body, Vec2 anchor, float restLength, float k);
	static Vec2 GenerateSpringForce(const Body& a, const Body& b, float restLength, float k);
};