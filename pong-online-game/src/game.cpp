#include "game.h"

Game::Game() {
    m_texture.loadFromFile("resources/back2.jpg");
    m_sprite.setTexture(m_texture);
}

void Game::setState(GameState state) {
    m_state = state;
}

GameState Game::getState() const {
    return m_state;
}

std::string Game::getName() const {
    return m_name;
}

void Game::setUserInputData() {
    m_ip = "localhost";
    std::cout << std::endl;
    std::cout << "Enter server registration port: ";
    std::cin >> m_port;
    std::cout << std::endl;
    std::cout << "Enter name: ";
    std::cin >> m_name;
}

//void Game::addPaddle(std::string name) {
//    Paddle1 paddle1;
//    if (m_paddle_vector.empty()) {
//        m_paddle_vector.push_back(Paddle1(10, 720 / 2));
//    }
//    if (m_paddle_vector.size() == 1) {
//        m_paddle_vector.push_back(Paddle1(1260, 720 / 2));
//    }
//    m_paddle_vector.back().setName(name);
//}

void Game::setConnection(Paddle1& paddle_1) {

    sf::Packet received_packet;
    sf::Packet send_packet;


    if (m_client.receiveData(received_packet, m_ip, m_port) == sf::Socket::Status::Done) {
        if (received_packet.getDataSize() > 0) {
            std::string str;
            if (received_packet >> str) {
                if (str == "NEW") {
                    if (received_packet >> str) {
                        if (str != m_name) {
                            //addPaddle(str);
                            m_paddle_2.setName(str);
                            std::cout << "New player: " << m_paddle_2.getName() << std::endl;
                        }
                    }
                }
                if (str == "DATA") {
                    while (!received_packet.endOfPacket()) {
                        float x, y;
                        received_packet >> str;
                        received_packet >> x;
                        received_packet >> y;
                        if (str == m_paddle_2.getName()) {
                            m_paddle_2.setPosition(x, y);
                        }
                    }
                }
            }
        }
    }

    send_packet.clear();
    send_packet << "DATA" << paddle_1.getPosition().x << paddle_1.getPosition().y;
    m_client.sendData(send_packet);
}

void Game::Start(sf::RenderWindow& window) {

    sf::Music music;
    music.openFromFile("resources/back1.mp3");

    sf::Clock clock;
    sf::Time dt;
    sf::Event event;
    Ball ball(1280 / 2, 10.0);

    //Paddle1 paddle_1(10, 720 / 2);
    //Paddle2 paddle_2(1260, 720 / 2);

    Paddle1 paddle_1(10, 720 / 2);

    this->setUserInputData();


    m_client.init();
    m_client.registerOnServer(m_ip, m_port, m_name);

    std::vector<std::string> name_vector;
    m_client.receiveConnectedClientName(name_vector);
    //for (int i = 0; i < name_vector.size(); ++i) {
    //    this->addPaddle(name_vector[i]);
    //}
    m_paddle_2.setPosition(1260, 720 / 2);
    m_paddle_2.setName(name_vector[0]);

    while (window.isOpen() && m_state == GameState::ON) {

        this->setConnection(paddle_1);

        dt = clock.restart();

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (ball.getPosition().intersects(paddle_1.getBounds())) {
            ball.hitBall();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
            m_state = GameState::MENU;
        }

        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        //    paddle_1.moveUp();
        //}
        //else {
        //    paddle_1.stopUp();
        //}

        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        //    paddle_1.moveDown();
        //}
        //else {
        //    paddle_1.stopDown();
        //}

        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        //    paddle_2.moveUp();
        //}
        //else {
        //    paddle_2.stopUp();
        //}

        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        //    paddle_2.moveDown();
        //}
        //else {
        //    paddle_2.stopDown();
        //}

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            paddle_1.moveUp();
        }
        else {
            paddle_1.stopUp();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            paddle_1.moveDown();
        }
        else {
            paddle_1.stopDown();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            if (music.Playing) {
                music.pause();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
            if (music.Paused) {
                music.play();
            }
        }

        if (ball.getPosition().left < 0 || ball.getPosition().left + ball.getPosition().width > 1280) {
            m_state = GameState::LOSE;
        }

        if (ball.getPosition().top < 0 || ball.getPosition().top + ball.getPosition().height > 720) {
            ball.bounceSide();
        }

        window.clear();
        ball.update(dt);
        paddle_1.update(dt);
        for (int i = 0; i < m_paddle_vector.size(); ++i) {
            m_paddle_vector[i].update(dt);
        }
        //paddle_1.update(dt);
        //paddle_2.update(dt);

        window.draw(m_sprite);
        ball.draw(window);
        //paddle_1.draw(window);
        //paddle_2.draw(window);
        paddle_1.draw(window);
        for (int i = 0; i < m_paddle_vector.size(); ++i) {
            m_paddle_vector[i].draw(window);
        }
        window.display();
    }
}