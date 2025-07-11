#include "core/GameEngine.hpp"
#include "entities/Player.hpp"
#include "entities/Enemy.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <optional>
#include <cstdlib>
#include <ctime>

GameEngine::GameEngine():
    window(sf::VideoMode({1024, 768}), "RPG", sf::Style::Close),
    isRunning(true),
    currentState(GameState::PLAYING)
{
    srand(static_cast<unsigned int>(time(0))); // Seed random number generator
    window.setFramerateLimit(60);
    player = std::make_unique<Player>(512.0f, 384.0f);
    createWorld();
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
        if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
            if(keyPressed->code == sf::Keyboard::Key::Tab){
                showMinimap = !showMinimap;
            }
        }
    }
}

void GameEngine::update(float deltaTime) {
    if (currentState == GameState::PLAYING) {
        player->update(deltaTime, window);

        Room& currentRoom = *world[currentRoomY][currentRoomX];
        currentRoom.update(deltaTime, player->getPosition());


        collisionManager.checkPlayerProjectilesVsEnemies(*player, currentRoom.getEnemies());
        collisionManager.checkEnemyProjectilesVsPlayer(currentRoom.getEnemies(), *player);
        collisionManager.checkEntityCollisions(*player, currentRoom.getEnemies());


        // Keep player within window bounds
        sf::Vector2f pos = player->getPosition();
        float radius = player->getRadius();
        sf::Vector2u windowSize = window.getSize();
        const int worldWidth = world[0].size();
        const int worldHeight = world.size();

        bool transitioned = false;


            // Move to room on the right
        if (pos.x > windowSize.x && currentRoomX < worldWidth - 1) {
            currentRoomX++;
            pos.x = radius; // Appear on the left side of the new room
            transitioned = true;
        }
        // Move to room on the left
        else if (pos.x < 0 && currentRoomX > 0) {
            currentRoomX--;
            pos.x = windowSize.x - radius; // Appear on the right side
            transitioned = true;
        }
        // Move to room below
        else if (pos.y > windowSize.y && currentRoomY < worldHeight - 1) {
            currentRoomY++;
            pos.y = radius; // Appear on the top side
            transitioned = true;
        }
        // Move to room above
        else if (pos.y < 0 && currentRoomY > 0) {
            currentRoomY--;
            pos.y = windowSize.y - radius; // Appear on the bottom side
            transitioned = true;
        }
        if(transitioned == true){
            player->setPosition(pos);
        }
    }
}

void GameEngine::render() {
    Room& currentRoom = *world[currentRoomY][currentRoomX];
    sf::Color bgColor = currentRoom.getBackgroundColor();

    window.clear(bgColor);

    if (currentState == GameState::PLAYING) {
        currentRoom.render(window);
        player->render(window);
    }

    if (showMinimap) {
        const float mapScale = 10.0f;
        const float mapPadding = 5.0f;

        // Position the whole map in the top-right corner
        const float mapBaseX = window.getSize().x - (world[0].size() * (mapScale + mapPadding)) - mapPadding;
        const float mapBaseY = mapPadding;

        // Draw each room's rectangle
        for (int y = 0; y < minimapRects.size(); ++y) {
            for (int x = 0; x < minimapRects[y].size(); ++x) {
                sf::Vector2f pos(mapBaseX + x * (mapScale + mapPadding), mapBaseY + y * (mapScale + mapPadding));
                minimapRects[y][x].setPosition(pos);
                window.draw(minimapRects[y][x]);
            }
        }

        // Draw the player icon on the current room
        playerMapIcon.setPosition({
            mapBaseX + currentRoomX * (mapScale + mapPadding),
            mapBaseY + currentRoomY * (mapScale + mapPadding)
        });
        window.draw(playerMapIcon);
    }

    window.display();
}

void GameEngine::createWorld(){
    const int worldWidth = 3;
    const int worldHeight = 3;
    world.resize(worldHeight);

    for(int y = 0; y < worldHeight; ++y){
        world[y].resize(worldWidth);
        for(int x = 0; x < worldWidth; ++x){
            // Create a room with a random background color
            sf::Color bgColor = sf::Color(rand() % 256, rand() % 256, rand() % 256);
            world[y][x] = std::make_unique<Room>(bgColor);

            // Add some enemies to the room
            if (rand() % 2 == 0) { // Randomly decide to add an enemy
                world[y][x]->addEnemy(static_cast<float>(x * 200 + 100), static_cast<float>(y * 200 + 100));
            }
        }
    }
    currentRoomX = 1;
    currentRoomY = 1;

    const float mapScale = 10.0f;
    const float mapPadding = 5.0f;
    minimapRects.resize(worldHeight);
    for (int y = 0; y < worldHeight; ++y) {
        minimapRects[y].resize(worldWidth);
        for (int x = 0; x < worldWidth; ++x) {
            minimapRects[y][x].setSize({mapScale, mapScale});
            minimapRects[y][x].setFillColor(world[y][x]->getBackgroundColor() + sf::Color(50, 50, 50)); // Slightly brighter than room
            minimapRects[y][x].setOutlineThickness(1.0f);
            minimapRects[y][x].setOutlineColor(sf::Color::White);
        }
    }
    playerMapIcon.setSize({mapScale, mapScale});
    playerMapIcon.setFillColor(sf::Color::Blue);

    showMinimap = false;
}