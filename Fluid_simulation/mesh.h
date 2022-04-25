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
	Vector2u N;
	Vector2f dimensions;
	Vector2f offset;
	Vector2f previousMousePosition;
	float maxDensity;
	std::vector<Cell> cells;

	ColorGradient colorGradient;
	int currentSourceStyle;
	enum SourceStyle{DOT, CROSS, SQUARE};
	int iter;
	float dt, diff;
	
public:
	Mesh(Vector2f& sceneSize, Vector2f& origin, Vector2u& meshNumber);
	void update(RenderWindow& window);
	

private:
	void addDensity(RenderWindow& window);
	void addVelocity(RenderWindow& window);
	void step();
	void diffuse();
	void diffuseVelocity();
	void advect();
	void advectDensity();
	void project(float* p, float* div);
	void fill();
	void plot2D();

	void linear_solve(int b, float a, float c);
	void linear_solve_Velocity(int b, float a, float c);
	void set_bnd(int b, float* x);

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
