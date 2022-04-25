#include "mesh.h"

Mesh::Mesh(Vector2f& sceneSize, Vector2f& origin, Vector2u& meshNumber)
    : colorGradient(Color::Black, Color::White)
{
    N = meshNumber;
    dimensions = sceneSize;
    Vector2f prevOrigin = Vector2f(sceneSize.x / 2.0f, sceneSize.y / 2.0f);
    offset = origin - prevOrigin;
    maxDensity = 0.0f;
    currentSourceStyle = SourceStyle::CROSS;
    iter = 3;
    dt = 0.01f;
    diff = 0.001f;
    const Vector2f size(sceneSize.x / N.x, sceneSize.y / N.y);

    for (int j = 0; j < N.y; j++)
    {
        for (int i = 0; i < N.x; i++)
        {
            Cell cell(size, Vector2u(i, j));
            Vector2f prevPos(i * size.x, j * size.y);
            Vector2f pos = prevPos + offset;

            cell.setPosition(pos);

            cell.setValue(0.0f);

            cells.push_back(cell);
        }
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

    step();

    fill();

    plot2D();
}

void Mesh::step()
{
    diffuseVelocity();
    /*diffuse();

    project(Vx0, Vy0, Vx, Vy);*/

    advect();

    /*project(Vx, Vy, Vx0, Vy0);*/

    diffuse();
    advectDensity();
}

void Mesh::addDensity(RenderWindow& window)
{
    if (Mouse::isButtonPressed(Mouse::Left))
    {
        Cell* c = &cells[indexFromMousePosition(window)];

        for (auto& n : getMeshSource(c->m_index))
        {
            cells[n].addDensity(1.f);
        }

        updateMaxDensity(c->getValue());
    }
}

void Mesh::addVelocity(RenderWindow& window)
{
    if (sf::Mouse::isButtonPressed(Mouse::Right))
    {
        Vector2f currentMousePosition  = cells[indexFromMousePosition(window)].getCenter();
        float factor = -500.0f;
        Vector2f velocity = factor*(currentMousePosition - previousMousePosition);
        velocity.x = -velocity.x;
        Cell* c = &cells[indexFromMousePosition(window)];
        c->addVelocity(velocity);

        previousMousePosition = currentMousePosition;
    }
}

void Mesh::diffuse()
{
    float a = dt * diff * (N.x - 2) * (N.y - 2);
    //float* dens = cells[].density;
    linear_solve(1, a, 1 + 4 * a);
}

void Mesh::diffuseVelocity()
{
    float a = dt * 1.0f * (N.x - 2) * (N.y - 2);
    //float* dens = cells[].density;
    linear_solve_Velocity(1, a, 1 + 4 * a);
}

void Mesh::linear_solve_Velocity(int b, float a, float c)
{
    float cRecip = 1.0 / c;

    for (int k = 0; k < iter; k++)
    {
        for (int j = 1; j < N.y - 1; j++)
        {
            for (int i = 1; i < N.x - 1; i++)
            {
                Vector2f currVelocity = cells[index(i, j)].velocity;
                cells[index(i, j)].velocity = (cells[index(i, j)].previousVelocity
                    + a * (cells[index(i + 1, j)].velocity
                         + cells[index(i - 1, j)].velocity
                         + cells[index(i, j + 1)].velocity
                         + cells[index(i, j - 1)].velocity)) * cRecip;

                cells[index(i, j)].previousVelocity = currVelocity;
            }
        }

        //set_bnd(b, x);
    }
}

void Mesh::linear_solve(int b, float a, float c)
{
    float cRecip = 1.0 / c;

    for (int k = 0; k < iter; k++) 
    {
        for (int j = 1; j < N.y - 1; j++)
        {
            for (int i = 1; i < N.x - 1; i++)
            {
                float toto = cells[index(i, j)].density;
                cells[index(i, j)].density = (cells[index(i, j)].previousDensity
                                + a * (cells[index(i + 1, j    )].density
                                     + cells[index(i - 1, j    )].density
                                     + cells[index(i    , j + 1)].density
                                     + cells[index(i    , j - 1)].density)) * cRecip;
                cells[index(i, j)].previousDensity = toto;
            }
        }
        
        //set_bnd(b, x);
    }
}

void Mesh::set_bnd(int b, float* x)
{
    for (int i = 1; i < N.x - 1; i++) {
        x[index(i, 0    )] = b == 2 ? -x[index(i, 1)]     : x[index(i, 1)];
        x[index(i, N.y - 1)] = b == 2 ? -x[index(i, N.y - 2)] : x[index(i, N.y - 2)];
    }
    
    for (int j = 1; j < N.y - 1; j++) {
        x[index(0, j)]     = b == 1 ? -x[index(1, j)]     : x[index(1, j)];
        x[index(N.x - 1, j)] = b == 1 ? -x[index(N.x - 2, j)] : x[index(N.x - 2, j)];
    }
    

    x[index(0      , 0    )]   = 0.5f * (x[index(1      , 0    )]   + x[index(0      , 1      )]);
    x[index(0      , N.y - 1)] = 0.5f * (x[index(1      , N.y - 1)] + x[index(0      , N.y - 2)]);
    x[index(N.x - 1, 0    )]   = 0.5f * (x[index(N.x - 2, 0    )]   + x[index(N.x - 1, 1      )]);
    x[index(N.x - 1, N.y - 1)] = 0.5f * (x[index(N.x - 2, N.y - 1)] + x[index(N.x - 1, N.y - 2)]);
}

void Mesh::project(float* p, float* div)
{
    for (int j = 1; j < N.y - 1; j++) {
        for (int i = 1; i < N.x - 1; i++) {
            div[index(i, j)] = -0.5f * (
                  cells[index(i + 1, j)].velocity.x
                - cells[index(i - 1, j)].velocity.x
                + cells[index(i, j + 1)].velocity.y
                - cells[index(i, j - 1)].velocity.y ) / N.x;
            p[index(i, j)] = 0;
        }
    }
    
    //set_bnd(0, div);
    //set_bnd(0, p);
    linear_solve(0, 1, 6);

    for (int j = 1; j < N.y - 1; j++) {
        for (int i = 1; i < N.x - 1; i++) {
            cells[index(i, j)].velocity.x -= 0.5f * (p[index(i + 1, j    )] - p[index(i - 1, j    )]) * N.x;
            cells[index(i, j)].velocity.y -= 0.5f * (p[index(i    , j + 1)] - p[index(i    , j - 1)]) * N.y;
        }
    }
    //set_bnd(1, velocX);
    //set_bnd(2, velocY);
}

void Mesh::advect()
{
    float i0, i1, j0, j1;

    float dtx = dt * (N.x - 2);
    float dty = dt * (N.y - 2);

    float s0, s1, t0, t1;
    float tmp1, tmp2, x, y;

    float Nfloat = N.x;
    float ifloat, jfloat;
    int i, j;

    for (j = 1, jfloat = 1; j < N.y - 1; j++, jfloat++) {
        for (i = 1, ifloat = 1; i < N.x - 1; i++, ifloat++) {
            tmp1 = dtx * cells[index(i, j)].velocity.x;
            tmp2 = dty * cells[index(i, j)].velocity.y;
            x = ifloat - tmp1;
            y = jfloat - tmp2;

            if (x < 0.5f) x = 0.5f;
            if (x > Nfloat + 0.5f) x = Nfloat + 0.5f;
            i0 = floorf(x);
            i1 = i0 + 1.0f;
            if (y < 0.5f) y = 0.5f;
            if (y > Nfloat + 0.5f) y = Nfloat + 0.5f;
            j0 = floorf(y);
            j1 = j0 + 1.0f;

            s1 = x - i0;
            s0 = 1.0f - s1;
            t1 = y - j0;
            t0 = 1.0f - t1;

            int i0i = i0;
            int i1i = i1;
            int j0i = j0;
            int j1i = j1;

            cells[index(i, j)].velocity =
                s0 * (t0 * cells[index(i0i, j0i)].previousVelocity + t1 * cells[index(i0i, j1i)].previousVelocity)
              + s1 * (t0 * cells[index(i1i, j0i)].previousVelocity + t1 * cells[index(i1i, j1i)].previousVelocity);
        }
    }
    //set_bnd(b, d);
}

void Mesh::advectDensity()
{
    float i0, i1, j0, j1;

    float dtx = dt * (N.x - 2);
    float dty = dt * (N.y - 2);

    float s0, s1, t0, t1;
    float tmp1, tmp2, x, y;

    float Nfloat = N.x;
    float ifloat, jfloat;
    int i, j;

    for (j = 1, jfloat = 1; j < N.y - 1; j++, jfloat++) {
        for (i = 1, ifloat = 1; i < N.x - 1; i++, ifloat++) {
            tmp1 = dtx * cells[index(i, j)].velocity.x;
            tmp2 = dty * cells[index(i, j)].velocity.y;
            x = ifloat - tmp1;
            y = jfloat - tmp2;

            if (x < 0.5f) x = 0.5f;
            if (x > Nfloat + 0.5f) x = Nfloat + 0.5f;
            i0 = floorf(x);
            i1 = i0 + 1.0f;
            if (y < 0.5f) y = 0.5f;
            if (y > Nfloat + 0.5f) y = Nfloat + 0.5f;
            j0 = floorf(y);
            j1 = j0 + 1.0f;

            s1 = x - i0;
            s0 = 1.0f - s1;
            t1 = y - j0;
            t0 = 1.0f - t1;

            int i0i = i0;
            int i1i = i1;
            int j0i = j0;
            int j1i = j1;

            cells[index(i, j)].density =
                  s0 * (t0 * cells[index(i0i, j0i)].previousDensity + t1 * cells[index(i0i, j1i)].previousDensity)
                + s1 * (t0 * cells[index(i1i, j0i)].previousDensity + t1 * cells[index(i1i, j1i)].previousDensity);
        }
    }
    //set_bnd(b, d);
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
        for (int j = 0; j < N.y; j++)
        {
            for (int i = 0; i < N.x; i++)
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
        //Log::print(c.velocity);
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
    if (posX > N.x && posY > N.y)
        return 0;
    return posX + posY * N.y;
}

unsigned int Mesh::indexFromMousePosition(RenderWindow& window)
{
    Vector2i position = Mouse::getPosition(window);
    Vector2f worldPos = window.mapPixelToCoords(position);
    //Log::print(worldPos);

    unsigned int posX = (int)((worldPos.x - offset.x)/ dimensions.x * N.x);
    unsigned int posY = (int)((worldPos.y - offset.y)/ dimensions.y * N.y);

    return index(posX, posY);
}

std::vector<int> Mesh::neighboursIndex(unsigned int posX, unsigned int posY)
{
    std::vector<int> neighboursList;

    if (posX > 0)
    {
        neighboursList.push_back(index(posX - 1, posY));
    }

    if (posX < N.x - 1)
    {
        neighboursList.push_back(index(posX + 1, posY));
    }
    
    if (posY < N.y - 1)
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