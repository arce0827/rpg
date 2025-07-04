#include "core/GameEngine.hpp"
#include "entities/Player.hpp"
#include <iostream>
#include <optional> // Required for SFML 3.0 event handling

GameEngine::GameEngine():
    // Use an initializer list for sf::VideoMode for clarity with modern C++
    window(sf::VideoMode({1024, 768}), "RPG", sf::Style::Close),
    isRunning(true),
    currentState(GameState::PLAYING)
{
    window.setFramerateLimit(60);
    player = std::make_unique<Player>(512.0f, 384.0f);
}

GameEngine::~GameEngine() = default;

void GameEngine::run() {
    // The sf::Clock should be part of the class to measure deltaTime correctly
    // but for now, we'll keep it here as in the original code.
    sf::Clock clock;
    while(isRunning && window.isOpen()) {
        // Restart the clock every frame to get the elapsed time
        float deltaTime = clock.restart().asSeconds();
        processEvents();
        update(deltaTime);
        render();
    }
}

void GameEngine::processEvents() {
    // The correct event loop for SFML 3.0
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        // The sf::Event object itself has a helper function `getIf<T>()`
        // which returns a pointer to the specific event data if the type matches.
        // This is the intended way to check for event types in SFML 3.

        // Check if the window was closed
        if (event->is<sf::Event::Closed>()) {
            isRunning = false;
            window.close();
        }

        // Check if a key was pressed
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            // Check if the specific key was Escape
            if (keyPressed->code == sf::Keyboard::Key::Escape) {
                isRunning = false;
                window.close();
            }
        }
    }
}

void GameEngine::update(float deltaTime) {
    if (currentState == GameState::PLAYING) {
        player->update(deltaTime);

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
    }

    window.display();
}