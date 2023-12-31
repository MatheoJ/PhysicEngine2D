#pragma once

#include "Vec2.h"
#include <vector>

enum ShapeType
{
	CIRCLE,
	POLYGON,
	BOX
};

struct Shape
{
	virtual ~Shape() = default;
	virtual ShapeType GetType() const = 0;
	virtual Shape* Clone() const = 0;
	virtual float GetMomentOfInertia() const =0;
};

struct CircleShape: public Shape 
{
	float radius;
	CircleShape (const float radius);
	virtual ~CircleShape();
	ShapeType GetType() const override;
	Shape* Clone() const override;
	float GetMomentOfInertia() const override;
};

struct PolygonShape : public Shape
{
	std::vector<Vec2> localVertices;
	std::vector<Vec2> worldVertices;

	PolygonShape() = default;
	PolygonShape(const std::vector<Vec2>& vertices);
	virtual ~PolygonShape();
	ShapeType GetType() const override;	
	Shape* Clone() const override;
	float GetMomentOfInertia() const override;
	void UpdateVertices(float rotation, const Vec2& position);
	Vec2 EdgeAt(int index) const;
	float FindMinSeparation(const PolygonShape* other, Vec2& axis, Vec2& point) const;
};

struct BoxShape : public PolygonShape
{
	float width;
	float height;

	BoxShape(const float width, const float height);
	virtual ~BoxShape();
	ShapeType GetType() const override;
	Shape* Clone() const override;
	float GetMomentOfInertia() const override;
};
