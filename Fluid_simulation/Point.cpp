#include "Point.h"

Point::Point(Vector2f& position, Vector2f& dimensions)
    :m_radius(0.015f* dimensions.x), m_point(m_radius)
{
    m_point.setOrigin(m_radius, m_radius);
    m_point.setPosition(position);
    m_point.setFillColor(Color::Black);
}

void Point::draw(RenderTarget& target, RenderStates states) const
{
    target.draw(m_point, states);
}