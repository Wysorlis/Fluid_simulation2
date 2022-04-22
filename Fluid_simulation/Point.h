#ifndef POINT_H
#define POINT_H
#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>

using namespace sf;

class Point : public Drawable
{
	float m_radius;
	CircleShape m_point;
public:
	Point(Vector2f& position, Vector2f& dimensions);
private:
	virtual void draw(RenderTarget& target, RenderStates states) const;
};

#endif 

