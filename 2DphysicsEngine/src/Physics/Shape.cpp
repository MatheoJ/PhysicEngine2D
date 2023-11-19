#include "Shape.h"
#include <iostream>

CircleShape::CircleShape(const float radius)
{
	this->radius = radius;
	std::cout << "CircleShape created with radius " << this->radius << std::endl;
}

CircleShape::~CircleShape()
{
	std::cout << "CircleShape destroyed" << std::endl;
}

Shape* CircleShape::Clone() const
{
	return new CircleShape(radius);
}

ShapeType CircleShape::GetType() const
{
	return CIRCLE;
}

float CircleShape::GetMomentOfInertia() const
{
	return 0.5f * radius * radius;
}

PolygonShape::PolygonShape(const std::vector<Vec2>& vertices)
{
	this->localVertices = vertices;
	std::cout << "PolygonShape created with " << this->localVertices.size() << " vertices" << std::endl;
}

PolygonShape::~PolygonShape()
{
	std::cout << "PolygonShape destroyed" << std::endl;
}

Shape* PolygonShape::Clone() const
{
	return new PolygonShape(localVertices);
}

ShapeType PolygonShape::GetType() const
{
	return POLYGON;
}

float PolygonShape::GetMomentOfInertia() const
{
	return 0.0f;
}

void PolygonShape::UpdateVertices(float rotation, const Vec2& position)
{
	for (int i = 0; i < localVertices.size(); i++)
	{
		worldVertices[i] = localVertices[i].Rotate(rotation);
		worldVertices[i] += position;
	}
}

BoxShape::BoxShape(const float width, const float height)
{
	this->width = width;
	this->height = height;

	localVertices.push_back(Vec2(-width / 2, -height / 2));
	localVertices.push_back(Vec2(width / 2, -height / 2));
	localVertices.push_back(Vec2(width / 2, height / 2));
	localVertices.push_back(Vec2(-width / 2, height / 2));

	worldVertices.push_back(Vec2(-width / 2, -height / 2));
	worldVertices.push_back(Vec2(width / 2, -height / 2));
	worldVertices.push_back(Vec2(width / 2, height / 2));
	worldVertices.push_back(Vec2(-width / 2, height / 2));
}

BoxShape::~BoxShape()
{
}

float BoxShape::GetMomentOfInertia() const
{
	return 0.083333 * (width * width + height * height);
}

Shape* BoxShape::Clone() const
{
	return new BoxShape(width, height);
}

ShapeType BoxShape::GetType() const
{
	return BOX;
}