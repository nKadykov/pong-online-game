#pragma once
#include <SFML/Graphics.hpp>

class Ball
{
	sf::Vector2f m_position;
	sf::CircleShape m_ball_shape;

	float m_speed = 400;
	float m_direction_x = 1.0f;
	float m_direction_y = 1.0f;

public:
	Ball() = default;
	Ball(float startX, float startY);
	Ball(const Ball&) = delete;
	Ball(Ball&&) = delete;
	Ball& operator=(const Ball&) = delete;
	Ball& operator=(Ball&&) = delete;
	~Ball() = default;

	sf::FloatRect getPosition();
	sf::CircleShape getShape();

	float getXVelocity();

	//void bounceSides();
	void bounceTop();
	void bounceDown();
	void hitBall();
	void update(sf::Time dt);
};
