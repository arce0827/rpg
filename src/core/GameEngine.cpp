#include "core/GameEngine.hpp"
#include "entities/Player.hpp"
#include "entities/Enemy.hpp"
#include <iostream>
#include <optional>

GameEngine::GameEngine():
    window(sf::VideoMode({1024, 768}), "RPG", sf::Style::Close),
    isRunning(true),
    currentState(GameState::PLAYING)
{
    window.setFramerateLimit(60);
    player = std::make_unique<Player>(512.0f, 384.0f);
    spawnEnemies();
}

GameEngine::~GameEngine() = default;

void GameEngine::run() {
    sf::Clock clock;
    while(isRunning && window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        processEvents();
        update(deltaTime);
        render();
    }
}

void GameEngine::processEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            isRunning = false;
            window.close();
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::Escape) {
                isRunning = false;
                window.close();
            }
        }
    }
}

void GameEngine::update(float deltaTime) {
    if (currentState == GameState::PLAYING) {
        player->update(deltaTime, window);
        updateEnemies(deltaTime);

        // Keep player within window bounds
        sf::Vector2f pos = player->getPosition();
        float radius = player->getRadius();
        sf::Vector2u windowSize = window.getSize();

        if (pos.x - radius < 0.f) pos.x = radius;
        if (pos.x + radius > windowSize.x) pos.x = windowSize.x - radius;
        if (pos.y - radius < 0.f) pos.y = radius;
        if (pos.y + radius > windowSize.y) pos.y = windowSize.y - radius;

        player->setPosition(pos);
    }
}

void GameEngine::render() {
    window.clear(sf::Color::Black);

    if (currentState == GameState::PLAYING) {
        player->render(window);
        // FIX: You were missing the call to render the enemies
        renderEnemies();
    }

    window.display();
}

void GameEngine::spawnEnemies(){
    enemies.push_back(std::make_unique<Enemy>(100, 100));
    enemies.push_back(std::make_unique<Enemy>(800, 200));
    enemies.push_back(std::make_unique<Enemy>(200, 600));
    enemies.push_back(std::make_unique<Enemy>(700, 500));
}

void GameEngine::updateEnemies(float deltaTime){
    sf::Vector2u windowSize = window.getSize();
    sf::FloatRect bounds({0.0f, 0.0f}, {static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)});
    
    for (auto& enemy : enemies){
        if (enemy->isAlive()){
            enemy->update(deltaTime, player->getPosition());

            auto& enemyProjectiles = enemy->getProjectiles();
            for (auto it = enemyProjectiles.begin(); it != enemyProjectiles.end();){
                (*it)->update(deltaTime, bounds);

                // Use the projectile's own isActive flag instead of re-checking position
                if (!(*it)->getIsActive()) {
                    it = enemyProjectiles.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }
    // FIX: THE EXTRA BRACE WAS REMOVED FROM HERE
}

void GameEngine::renderEnemies() {
    int aliveCount = 0; // FIX: Define aliveCount
    for (const auto& enemy : enemies){
        if(enemy->isAlive()){
            enemy->render(window);
            aliveCount++; // Increment if alive

            for(auto& projectile : enemy->getProjectiles()){
                projectile->render(window);
            }
        }
    }

    static int frameCount = 0;
    if (frameCount % 60 == 0) {
        std::cout << "Alive enemies: " << aliveCount << " / " << enemies.size() << std::endl;
    }
    frameCount++;
}