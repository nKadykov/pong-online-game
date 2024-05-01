#pragma once
#include <SFML/graphics.hpp>
#include "paddle.h"

class Paddle1 : public Paddle
{
	sf::RectangleShape m_shape;
	sf::Vector2f m_position;
	float m_speed = 1500.0f;

	bool m_moving_up = false;
	bool m_moving_down = false;

public:
	Paddle1() = default;
	Paddle1(float startX, float startY);
	Paddle1(const Paddle1&) = delete;
	Paddle1(Paddle1&&) = delete;
	Paddle1& operator=(const Paddle1&) = delete;
	Paddle1& operator=(Paddle1&&) = delete;
	~Paddle1() = default;

	sf::RectangleShape getShape();
	sf::FloatRect getPosition();

	void moveUp() override;
	void moveDown() override;
	void stopUp() override;
	void stopDown() override;

	void update(sf::Time dt);
};