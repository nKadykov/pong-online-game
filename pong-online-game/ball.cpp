#include "ball.h"

Ball::Ball(float startX, float startY) {
	m_position.x = startX;
	m_position.y = startY;

	m_ball_shape.setRadius(15);
	m_ball_shape.setFillColor(sf::Color::Green);
	m_ball_shape.setOutlineThickness(2);
	m_ball_shape.setOutlineColor(sf::Color::White);
	m_ball_shape.setPosition(m_position);
}

sf::FloatRect Ball::getPosition() {
	return m_ball_shape.getGlobalBounds();
}

sf::CircleShape Ball::getShape() {
	return m_ball_shape;
}

float Ball::getXVelocity() {
	return m_direction_x;
}

//void Ball::bounceSides() {
//	m_direction_x = -m_direction_x;
//}

void Ball::bounceTop() {
	m_direction_y = -m_direction_y;
}

void Ball::bounceDown() {
	m_direction_y = -m_direction_y;
}

void Ball::update(sf::Time dt) {
	m_position.x += m_direction_x * m_speed * dt.asSeconds();
	m_position.y += m_direction_y * m_speed * dt.asSeconds();

	m_ball_shape.setPosition(m_position);
}

void Ball::hitBall() {
	m_direction_y = -m_direction_y;
}