#include "main.h"


int main()
{
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML works!");
    int N = 10;
    Vector2u windowSize = window.getSize();
    Mesh mesh(windowSize, N);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            
        }

        window.clear();
        mesh.check(window);
        window.draw(mesh);
        window.display();
    }

    return 0;
}