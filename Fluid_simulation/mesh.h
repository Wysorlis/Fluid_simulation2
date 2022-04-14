#ifndef MESH_H
#define MESH_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace sf;

class Mesh : public Drawable
{
	int _meshCount;
	std::vector<RectangleShape> quads;
	
public:
	Mesh(Vector2u& windowSize ,int& meshCount);
	void check(RenderWindow& window);
private:

	bool checkMesh(RectangleShape& quad, Vector2i& mousePosition);
	virtual void draw(RenderTarget& target, RenderStates states) const;
};

#endif 
