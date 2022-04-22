#ifndef FLUID_H
#define FLUID_H

#include <SFML/Graphics.hpp>

using namespace sf;

class Fluid
{
public:
	float density;
	float diffusion;
	float viscosity;
	Vector2f velocity;
	Vector2f previousVelocity;

	Fluid();


};

#endif 

