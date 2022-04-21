#include "mesh.h"

Mesh::Mesh(Vector2f& sceneSize, Vector2f& origin, Vector2u& meshNumber)
    : colorGradient(Color::Black, Color::White)
{
    meshCount = meshNumber;
    dimensions = sceneSize;
    Vector2f prevOrigin = Vector2f(sceneSize.x / 2.0f, sceneSize.y / 2.0f);
    offset = origin - prevOrigin;
    maxDensity = 0.0f;
    currentSourceStyle = SourceStyle::CROSS;

    const Vector2f size(sceneSize.x / meshCount.x, sceneSize.y / meshCount.y);

    for (int j = 0; j < meshCount.y; j++)
    {
        for (int i = 0; i < meshCount.x; i++)
        {
            Cell cell(size, Vector2u(i, j));
            Vector2f prevPos(i * size.x, j * size.y);
            Vector2f pos = prevPos + offset;

            cell.setPosition(pos);

            cell.setValue(0.0f);

            cells.push_back(cell);
        }
    }   
    for (auto& c : cells)
    {
        //Log::print(c.getCenter());
    }
}

void Mesh::draw(RenderTarget& target, RenderStates states) const
{
    for (auto& c : cells)
    {
        target.draw(c, states);
    }    
}

void Mesh::update(RenderWindow& window)
{
    addDensity(window);
    addVelocity(window);

    fill();

    plot2D();
}

void Mesh::addDensity(RenderWindow& window)
{
    if (Mouse::isButtonPressed(Mouse::Left))
    {
        Cell* c = &cells[indexFromMousePosition(window)];

        for (auto& n : getMeshSource(c->m_index))
        {
            cells[n].addDensity(0.1f);
        }

        updateMaxDensity(c->getValue());
    }
}

void Mesh::addVelocity(RenderWindow& window)
{
    if (sf::Mouse::isButtonPressed(Mouse::Right))
    {
        /*Cell* c = &cells[indexFromMousePosition(window)];

        c->addDensity(0.1f);

        updateMaxDensity(c->getValue());
        */

        for (auto& c : cells)
        {
            c.setValue(0.0f);
        }
    }
}

void Mesh::fill()
{
    if (Keyboard::isKeyPressed(Keyboard::A))
    {
        for (auto& c : cells)
        {
            float r = sqrt(c.getCenter().x * c.getCenter().x + c.getCenter().y * c.getCenter().y);
            float val = sin(2.0f * M_PI * r / (0.25f * dimensions.x));

            c.setValue(val);
        }
        colorGradient.setMin(getMin());
        colorGradient.setMax(getMax());
    }

    if (Keyboard::isKeyPressed(Keyboard::Z))
    {
        for (int j = 0; j < meshCount.y; j++)
        {
            for (int i = 0; i < meshCount.x; i++)
            {
                float r = i + j;

                cells[index(i, j)].setValue(r);
            }
        }
        colorGradient.setMin(getMin());
        colorGradient.setMax(getMax());
    }

    if (Keyboard::isKeyPressed(Keyboard::E))
    {
        for (auto& c : cells)
        {
            float r = c.getCenter().x * c.getCenter().x + c.getCenter().y * c.getCenter().y;

            c.setValue(r);
        }
        colorGradient.setMin(getMin());
        colorGradient.setMax(getMax());
    }

    if (Keyboard::isKeyPressed(Keyboard::R))
    {
        for (auto& c : cells)
        {
            float r = sin(2.0f * M_PI * c.getCenter().x / (0.25f * dimensions.x))
                + cos(2.0f * M_PI * c.getCenter().y / (0.25f * dimensions.y));

            c.setValue(r);
        }
        colorGradient.setMin(getMin());
        colorGradient.setMax(getMax());
    }
}

void Mesh::plot2D()
{
    for (auto& c : cells)
    {
        c.setFillColor(colorGradient.evaluate(c.getValue()));
    }
}

std::vector<int> Mesh::getMeshSource(const Vector2u& meshIndex)
{
    std::vector<int> sources;

    switch (currentSourceStyle)
    {
    case DOT:
        sources.push_back(index(meshIndex.x, meshIndex.y));
        break;
    case CROSS:
        sources = neighboursIndex(meshIndex.x, meshIndex.y);
        sources.push_back(index(meshIndex.x, meshIndex.y));
        break;
    case SQUARE:
        sources.push_back(0);
        break;
    default:
        sources.push_back(0);
        break;
    }

    return sources;
}

float Mesh::getMax()
{
    float max = -1.0e90;

    for (auto& c : cells)
    {
        if (max < c.getValue())
        {
            max = c.getValue();
        } 
    }
    return max;
}

float Mesh::getMin()
{
    float min = 1.0e90;

    for (auto& c : cells)
    {
        if (min > c.getValue())
        {
            min = c.getValue();
        }
    }
    return min;
}

unsigned int Mesh::index(unsigned int posX, unsigned int posY)
{
    if (posX > meshCount.x && posY > meshCount.y)
        return 0;
    return posX + posY * meshCount.y;
}

unsigned int Mesh::indexFromMousePosition(RenderWindow& window)
{
    Vector2i position = Mouse::getPosition(window);
    Vector2f worldPos = window.mapPixelToCoords(position);
    //Log::print(worldPos);

    unsigned int posX = (int)((worldPos.x - offset.x)/ dimensions.x * meshCount.x);
    unsigned int posY = (int)((worldPos.y - offset.y)/ dimensions.y * meshCount.y);

    return index(posX, posY);
}

std::vector<int> Mesh::neighboursIndex(unsigned int posX, unsigned int posY)
{
    std::vector<int> neighboursList;

    if (posX > 0)
    {
        neighboursList.push_back(index(posX - 1, posY));
    }

    if (posX < meshCount.x - 1)
    {
        neighboursList.push_back(index(posX + 1, posY));
    }
    
    if (posY < meshCount.y - 1)
    {
        neighboursList.push_back(index(posX, posY + 1));
    }
    if (posY > 0)
    {
        neighboursList.push_back(index(posX, posY - 1));
    }
    
    return neighboursList;
}

void Mesh::updateMaxDensity(const float& cellDensity)
{
    if (cellDensity > maxDensity)
        maxDensity = cellDensity;
        colorGradient.setMax(maxDensity);
}