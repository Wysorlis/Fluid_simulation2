#ifndef CELL_H
#define CELL_H

#include <SFML/Graphics.hpp>

#include "fluid.h"

using namespace sf;

class Cell : public RectangleShape
{
	Fluid fluid;
																														
	Vector2f center;

public:
	Vector2u m_index;
	float density, previousDensity;
	Vector2f velocity;
	Vector2f previousVelocity;

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

