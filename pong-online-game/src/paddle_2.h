#pragma once
#include <SFML/graphics.hpp>
#include "paddle.h"

class Paddle2 : public Paddle
{
	sf::RectangleShape m_shape;
	sf::Vector2f m_position;
	float m_speed = 1500.0f;

	bool m_moving_up = false;
	bool m_moving_down = false;

public:
	Paddle2() = default;
	Paddle2(float startX, float startY);
	Paddle2(const Paddle2&) = delete;
	Paddle2(Paddle2&&) = delete;
	Paddle2& operator=(const Paddle2&) = delete;
	Paddle2& operator=(Paddle2&&) = delete;
	~Paddle2() = default;

	sf::RectangleShape getShape();
	sf::FloatRect getPosition();

	void moveUp() override;
	void moveDown() override;
	void stopUp() override;
	void stopDown() override;

	void update(sf::Time dt);

	void draw(sf::RenderWindow&);
};