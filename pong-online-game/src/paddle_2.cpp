#include "paddle_2.h"

Paddle2::Paddle2(float startX, float startY) {
	m_position.x = startX;
	m_position.y = startY;

	m_shape.setSize(sf::Vector2f(15, 70));
	m_shape.setFillColor(sf::Color::Blue);
	m_shape.setOutlineColor(sf::Color::Black);
	m_shape.setOutlineThickness(2);

	m_shape.setPosition(m_position);
}

void Paddle2::setName(std::string name) {
	m_name = name;
}

void Paddle2::setPosition(int x, int y) {
	m_position.x = x;
	m_position.y = y;
}

sf::FloatRect Paddle2::getBounds() {
	return m_shape.getGlobalBounds();
}

sf::RectangleShape Paddle2::getShape() {
	return m_shape;
}

sf::Vector2f Paddle2::getPosition() {
	return m_position;
}

std::string Paddle2::getName() {
	return m_name;
}

void Paddle2::moveUp() {
	m_moving_up = true;
}

void Paddle2::moveDown() {
	m_moving_down = true;
}

void Paddle2::stopUp() {
	m_moving_up = false;
}

void Paddle2::stopDown() {
	m_moving_down = false;
}

void Paddle2::update(sf::Time dt) {
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

void Paddle2::draw(sf::RenderWindow& window) {
	window.draw(m_shape);
}