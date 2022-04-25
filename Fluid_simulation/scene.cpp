#include "scene.h"


Scene::Scene(const Vector2u& windowSize)
    : dimensions(LENGHT_X, LENGHT_Y), origin(ORIGIN_X, ORIGIN_Y)
{
    window.create(VideoMode(windowSize.x, windowSize.y), "By Cavindel");

    // Setting t
    float factor = 1.0f;
    Vector2f viewSize = Vector2f(factor * dimensions.x, factor * dimensions.y);
    View view(origin, viewSize);
    window.setView(view);
    window.setFramerateLimit(30);
    window.setVerticalSyncEnabled(true);
}

int Scene::run()
{
    Vector2u meshNumber(MESH_X, MESH_Y);

    TimeManager timeManager;

    Mesh mesh(dimensions, origin, meshNumber);

    //Point point(origin, dimensions);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            inputHandler(event, window);
        }

        window.clear();

        mesh.update(window);

        window.draw(mesh);
        //window.draw(point);
        
        window.display();

        timeManager.update();
        timeManager.showFps(); 
    }

    return 0;
}


Vector2f Scene::pixToSceneCoordinates(Vector2f pixCoordinates)
{
    float sceneX = pixCoordinates.x / window.getSize().x * dimensions.x;
    float sceneY = pixCoordinates.y / window.getSize().y * dimensions.y;
    return Vector2f(sceneX, sceneY);
}

void Scene::inputHandler(Event event, RenderWindow& window)
{
    if (event.type == Event::Closed)
    {
        window.close();
    }

}