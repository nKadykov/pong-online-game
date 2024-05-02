#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ball.h"
#include "paddle_1.h"
#include "paddle_2.h"

enum class GameState { ON, LOSE, MENU };

class Game
{
private:
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	GameState m_state = GameState::ON;
public:
	Game();
	Game(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(const Game&) = delete;
	Game& operator=(Game&&) = delete;
	~Game() = default;

	void setState(GameState);
	GameState getState() const;

	void Start(sf::RenderWindow& sf);
};

