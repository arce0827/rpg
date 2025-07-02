#include "GameEngine.hpp"
#include "Player.hpp"
#include <iostream>

GameEngine::GameEngine():
    window(sf::VideoMode(1024, 768), "RPG", sf::Style::Close),
    isRunning(true),
    currentState(GameState::MENU){
        window.setFramerateLimit(60);

        player = std::make_unique<Player>(512.0f, 384.0f);
    }

    GameEngine::~GameEngine() = default;

    void GameEngine::run(){
        while(isRunning && window.isOpen()){
            float deltaTime = clock.restart().asSeconds();
            processEvents();
            update(deltaTime);
            render();
        }
    }
    
    void GameEngine::processEvents(){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                isRunning = false;
                window.close();
            }
        

            if(event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::Escape){
                    isRunning = false;
                    window.close();
                }
            }
        }
    }

    void GameEngine::update(float deltaTime){
        if (currentState == GameState::PLAYING){
            player->update(deltaTime);
            // Keep player within window bounds
            sf::Vector2f pos = player->getPosition();
            float radius = player->getRadius();
        
            if (pos.x - radius < 0) pos.x = radius;
            if (pos.x + radius > window.getSize().x) pos.x = window.getSize().x - radius;
            if (pos.y - radius < 0) pos.y = radius;
            if (pos.y + radius > window.getSize().y) pos.y = window.getSize().y - radius;
        
            player->setPosition(pos);
        }
    }

    void GameEngine::render(){
        window.clear(sf::Color::Black);

        if(currentState == GameState::PLAYING){
            player->render(window);
        }

        window.display();
    }
