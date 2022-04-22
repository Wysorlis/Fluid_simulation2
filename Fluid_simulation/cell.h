#ifndef CELL_H
#define CELL_H

#include <SFML/Graphics.hpp>

#include "fluid.h"

using namespace sf;

class Cell : public RectangleShape
{
	Fluid fluid;

	float density;
	Vector2f velocity;
	Vector2f previousVelocity;

	Vector2f center;

public:
	Vector2u m_index;

	Cell(const Vector2f& size, const Vector2u& index);
	Vector2f getCenter();
	void setValue(const float& value);
	float getValue();
	void addDensity(const float& densityAmount);
	void addVelocity(const Vector2f& velocityAmount);
	void setPosition(const Vector2f& position);

private:
	void setCellOptions();
	

};

#endif 

