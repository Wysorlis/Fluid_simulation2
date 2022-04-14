#include "mesh.h"

Mesh::Mesh(Vector2u& windowSize, int& meshCount)
{
    _meshCount = meshCount;
    
    const Vector2f size(windowSize.x / _meshCount, windowSize.y / _meshCount);

    for (int j = 0; j < _meshCount; j++)
    {
        for (int i = 0; i < _meshCount; i++)
        {
            int index = _meshCount + j * _meshCount;
            RectangleShape quad(size);
            //Color color((index * 255) / (_meshCount * _meshCount), 0, 0);
            quad.setFillColor(Color::Red);
            quad.setOutlineColor(Color::Black);
            quad.setOutlineThickness(1.0f);
            quad.setPosition(Vector2f(i * size.x, j * size.y));
            float origin_x = 0.5f * (float)quad.getSize().x;
            float origin_y = 0.5f * (float)quad.getSize().y;
            //quad.setOrigin(Vector2f(origin_x, origin_y));
            quads.push_back(quad);
        }
        
    }
    
    
}

void Mesh::check(RenderWindow& window)
{
    if (sf::Mouse::isButtonPressed(Mouse::Left))
    {
        Vector2i position = Mouse::getPosition(window);
        Vector2f worldPos = window.mapPixelToCoords(position);

        for (auto& q : quads)
        {
            //std::cout << worldPos.x << "; " << worldPos.y << std::endl;
            if (Mesh::checkMesh(q, position))
            {
                q.setFillColor(Color::Black);
            }
            
        }
    }
}

bool Mesh::checkMesh(RectangleShape& quad, Vector2i& mousePosition)
{
    if (quad.getPosition().x < mousePosition.x
        && mousePosition.x < quad.getPosition().x + quad.getSize().x)
    {
        if (quad.getPosition().y < mousePosition.y
            && mousePosition.y < quad.getPosition().y + quad.getSize().y)
        {
            return true;
        }
    }
    return false;
}

void Mesh::draw(RenderTarget& target, RenderStates states) const
{
    for (auto& q : quads)
    {
        target.draw(q, states);
    }
    

    
}