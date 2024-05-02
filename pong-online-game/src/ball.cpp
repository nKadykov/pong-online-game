#include "ball.h"

Ball::Ball(float startX, float startY) {
	m_position.x = startX;
	m_position.y = startY;

	m_shape.setRadius(15);
	m_shape.setFillColor(sf::Color::Green);
	m_shape.setOutlineThickness(2);
	m_shape.setOutlineColor(sf::Color::White);
	m_shape.setPosition(m_position);
}

sf::FloatRect Ball::getPosition() {
	return m_shape.getGlobalBounds();
}

sf::CircleShape Ball::getShape() {
	return m_shape;
}

float Ball::getXVelocity() {
	return m_direction_x;
}

void Ball::bounceSide() {
	m_direction_y = -m_direction_y;
}

void Ball::update(sf::Time dt) {
	m_position.x += m_direction_x * m_speed * dt.asSeconds();
	m_position.y += m_direction_y * m_speed * dt.asSeconds();

	m_shape.setPosition(m_position);
}

void Ball::hitBall() {
	m_direction_x = -m_direction_x;
}

void Ball::draw(sf::RenderWindow& window) {
	window.draw(m_shape);
}