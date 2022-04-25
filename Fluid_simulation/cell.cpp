#include "cell.h"

Cell::Cell(const Vector2f& size, const Vector2u& index)
	:RectangleShape(size), fluid(), density(0.0f), previousDensity(0.0f),
	velocity(0.0f, 0.0f), previousVelocity(0.0f, 0.0f), m_index(index)
{
	setCellOptions();
	
	center.x = getPosition().x + 0.5f * getSize().x;
	center.y = -(getPosition().y + 0.5f * getSize().y);

}

Vector2f Cell::getCenter()
{
	return center;
}

void Cell::setPosition(const Vector2f& position)
{
	RectangleShape::setPosition(position);
	center.x = getPosition().x + 0.5f * getSize().x;
	center.y = -(getPosition().y + 0.5f * getSize().y);
}

void Cell::setValue(const float& value)
{
	density = value;
}

float Cell::getValue()
{
	return density;
}



void Cell::addDensity(const float& densityAmount)
{
	if (density >= fluid.density)
		density = fluid.density;
	else
		density += densityAmount;
}


void Cell::addVelocity(const Vector2f& velocityAmount)
{
	velocity += velocityAmount;
}

void Cell::setCellOptions()
{
	setFillColor(Color::Black);
	setOutlineColor(Color::Red);
	setOutlineThickness(0.0f);
}

