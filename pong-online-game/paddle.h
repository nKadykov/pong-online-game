#pragma once
class Paddle
{
public:
	virtual ~Paddle() {};
	virtual void moveUp() = 0;
	virtual void stopUp() = 0;
	virtual void moveDown() = 0;
	virtual void stopDown() = 0;
	virtual void update(sf::RenderWindow) = 0;
};

