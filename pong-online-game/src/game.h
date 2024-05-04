#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ball.h"
#include "paddle_1.h"
#include "paddle_2.h"
#include "network_client.h"

enum class GameState { ON, LOSE, MENU };

class Game
{
private:
	std::string m_name;
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	GameState m_state = GameState::ON;
	std::vector<Paddle1> m_paddle_vector;

	NetworkClient m_client;
	sf::IpAddress m_ip;
	unsigned short m_port;

	Paddle2 m_paddle_2;
public:
	Game();
	Game(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(const Game&) = delete;
	Game& operator=(Game&&) = delete;
	~Game() = default;

	void setState(GameState);
	GameState getState() const;
	std::string getName() const;

	void setUserInputData();
	void addPaddle(std::string name);

	void setConnection(Paddle1&);

	void Start(sf::RenderWindow& sf);
};

