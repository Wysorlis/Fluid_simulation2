#include "main.h"


int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML works!");
    int N = 10;
    //std::cout << window.get;
    Mesh mesh(N);
    mesh.hello();

    const float radius = 100.0f;
    sf::CircleShape circle(radius);
    circle.setFillColor(sf::Color::Green);
    float origin_x = 0.5 * (float)window.getSize().x;
    float origin_y = 0.5 * (float)window.getSize().y;

    circle.setOrigin(radius, radius);
    circle.setPosition(origin_x, origin_y);

    sf::RectangleShape pixel(sf::Vector2f(1.0f, 1.0f));
    sf::RectangleShape pixelA(sf::Vector2f(2.0f, 2.0f));
    sf::RectangleShape pixelB(sf::Vector2f(3.0f, 3.0f));
    pixel.setFillColor(sf::Color::Red);
    pixelA.setFillColor(sf::Color::Red);
    pixelB.setFillColor(sf::Color::Red);


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(circle);
        window.draw(pixel);
        window.draw(pixelA);
        window.draw(pixelB);
        window.display();
    }

    return 0;
}