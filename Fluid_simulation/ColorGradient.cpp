#include "ColorGradient.h"

ColorGradient::ColorGradient(const Color& colorMin, const Color& colorMax)
	: m_colorMin(colorMin), m_colorMax(colorMax),
	m_minValue(0.0f), m_maxValue(0.0f),
	m_minPlotValue(0.05f)
{
}

Color ColorGradient::evaluate(const float& value)
{
	float t = (value - m_minValue) / (m_maxValue - m_minValue);

	Color evaluatedColor;

	evaluatedColor.r = m_colorMin.r * (1 - t) + m_colorMax.r * t;
	evaluatedColor.g = m_colorMin.g * (1 - t) + m_colorMax.g * t;
	evaluatedColor.b = m_colorMin.b * (1 - t) + m_colorMax.b * t;

	return evaluatedColor;
}

void ColorGradient::setMax(const float& value)
{
	if(value > m_minPlotValue)
		m_maxValue = value;
}

void ColorGradient::setMin(const float& value)
{
	m_minValue = value;
}