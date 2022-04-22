#ifndef MESH_H
#define MESH_H
#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>


#include "cell.h"
#include "ColorGradient.h"
#include "log.h"

using namespace sf;

class Mesh : public Drawable
{
	Vector2u meshCount;
	Vector2f dimensions;
	Vector2f offset;
	float maxDensity;
	std::vector<Cell> cells;

	ColorGradient colorGradient;
	int currentSourceStyle;
	enum SourceStyle{DOT, CROSS, SQUARE};
	
public:
	Mesh(Vector2f& sceneSize, Vector2f& origin, Vector2u& meshNumber);
	void update(RenderWindow& window);
	

private:
	void addDensity(RenderWindow& window);
	void addVelocity(RenderWindow& window);
	void fill();
	void plot2D();

	std::vector<int> getMeshSource(const Vector2u& meshIndex);
	float getMin();
	float getMax();

	void updateMaxDensity(const float& cellDensity);
	unsigned int index(unsigned int posX, unsigned int posY);
	unsigned int indexFromMousePosition(RenderWindow& window);
	std::vector<int> neighboursIndex(unsigned int posX, unsigned int posY);

	virtual void draw(RenderTarget& target, RenderStates states) const;
};

#endif 
