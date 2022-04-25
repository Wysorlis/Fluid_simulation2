#ifndef SCENE_H
#define SCENE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "mesh.h"
#include "Point.h"
#include "log.h"
#include "TimeManager.h"

using namespace sf;

const unsigned int MESH_X = 50;
const unsigned int MESH_Y = 50;
const float LENGHT_X = 1.0f;
const float LENGHT_Y = 1.0f;
const float ORIGIN_X = 0.0f;
const float ORIGIN_Y = 0.0f;

class Scene
{
	RenderWindow window;
	Vector2f dimensions;
	Vector2f origin;

public:
	Scene(const Vector2u& windowSize);
	int run();
	Vector2f pixToSceneCoordinates(Vector2f pixCoordinates);
private:
	void inputHandler(Event event, RenderWindow& window);
};

#endif
