#include "paddle_1.h"

Paddle1::Paddle1(float startX, float startY) {
	m_position.x = startX;
	m_position.y = startY;

	m_shape.setSize(sf::Vector2f(15, 70));
	m_shape.setFillColor(sf::Color::Blue);
	m_shape.setOutlineColor(sf::Color::Black);
	m_shape.setOutlineThickness(2);

	m_shape.setPosition(m_position);
}

void Paddle1::setName(std::string name) {
	m_name = name;
}

void Paddle1::setPosition(int x, int y) {
	m_position.x = x;
	m_position.y = y;
}

sf::FloatRect Paddle1::getBounds() {
	return m_shape.getGlobalBounds();
}

sf::RectangleShape Paddle1::getShape() {
	return m_shape;
}

sf::Vector2f Paddle1::getPosition() {
	return m_position;
}

std::string Paddle1::getName() {
	return m_name;
}

void Paddle1::moveUp() {
	m_moving_up = true;
}

void Paddle1::moveDown() {
	m_moving_down = true;
}

void Paddle1::stopUp() {
	m_moving_up = false;
}

void Paddle1::stopDown() {
	m_moving_down = false;
}

void Paddle1::update(sf::Time dt) {
	if (m_moving_up) {
		m_position.y -= m_speed * dt.asSeconds();
	}
	if (m_moving_down) {
		m_position.y += m_speed * dt.asSeconds();
	}
	if (m_position.y < 0) {
		m_position.y = 0;
	}
	if (m_position.y > 620) {
		m_position.y = 620;
	}
	m_shape.setPosition(m_position);
}

void Paddle1::draw(sf::RenderWindow& window) {
	window.draw(m_shape);
}