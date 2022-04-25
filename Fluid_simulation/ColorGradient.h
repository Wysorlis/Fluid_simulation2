#ifndef COLORGRADIENT_H
#define COLORGRADIENT_H

#include <SFML/Graphics.hpp>

using namespace sf;

class ColorGradient
{
	Color m_colorMin;
	Color m_colorMax;

	float m_minValue;
	float m_maxValue;
	float m_minPlotValue;

public:
	ColorGradient(const Color& colorMin, const Color& colorMax);
	void setMin(const float& value);
	void setMax(const float& value);
	Color evaluate(const float& value);

};

#endif 
