#include <iostream>
#include <SFML/Graphics.hpp>
#include "game_over_window.h"
#include "menu.h"
#include "game.h"

enum class State { GAME, GAMEOVER, MENU };

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Pong");
    window.setFramerateLimit(60);
    sf::Event event;

    sf::Texture gameover_texture;
    gameover_texture.loadFromFile("resources/back1.jpg");
    sf::Sprite gameover_texture_sprite(gameover_texture);

    sf::Texture button_texture_1;
    sf::Texture button_texture_2;
    button_texture_1.loadFromFile("resources/button2.png");
    button_texture_2.loadFromFile("resources/button3.png");

    Menu menu("resources/back1.jpg");
    menu.addButton(500, 200, button_texture_1);
    menu.addButton(500, 400, button_texture_2);

    GameOverWindow gameover_window;
    gameover_window.setPosition(300, 200);

    State state = State::GAME;
    MenuState menu_state = MenuState::ON;
    GameOverState gameover_state = GameOverState::ON;
    ButtonState button_state = ButtonState::NONE;
    GameState game_state = GameState::ON;

    while (window.isOpen()) {

        window.clear();
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        if (game_state == GameState::MENU || gameover_state == GameOverState::MENU) {
            state = State::MENU;
            gameover_state = GameOverState::ON;
        }

        switch (state) {
        case State::GAMEOVER:
            gameover_window.draw(window);
            break;
        case State::MENU:
            menu.draw(window);
            break;
        }

        window.display();
    }
}