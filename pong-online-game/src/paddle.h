#pragma once
#include <SFML/Graphics.hpp>

class Paddle
{
public:
	virtual void moveUp() = 0;
	virtual void stopUp() = 0;
	virtual void moveDown() = 0;
	virtual void stopDown() = 0;
	virtual void update(sf::Time) = 0;
	virtual void draw(sf::RenderWindow&) = 0;
};

