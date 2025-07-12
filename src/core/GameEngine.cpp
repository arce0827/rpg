#include "core/GameEngine.hpp"
#include "entities/Player.hpp"
#include "entities/Enemy.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <optional>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <SFML/Config.hpp>

GameEngine::GameEngine():
    window(sf::VideoMode({1024, 768}), "RPG", sf::Style::Close),
    isRunning(true),
    currentState(GameState::MENU),
    titleText(),
    playButtonText(),
    exitButtonText()
{
    srand(static_cast<unsigned int>(time(0))); // Seed random number generator
    window.setFramerateLimit(60);
    player = std::make_unique<Player>(512.0f, 384.0f);
    createWorld();
 
    if(!font.openFromFile("C:/users/LENOVO/rpg/src/fonts/QuinqueFive.ttf")){
        std::cerr << "Error loading font\n"<<std::endl;
    }

    // Title
    titleText = std::make_unique<sf::Text>(font, "Gytis LOX", 60);
    titleText->setFillColor(sf::Color::White);
    sf::FloatRect titleRect = titleText->getLocalBounds();
    titleText->setOrigin({titleRect.size.x / 2.0f, titleRect.size.y / 2.0f});
    titleText->setPosition({window.getSize().x / 2.0f, window.getSize().y / 4.0f});

    // Play Button
    playButtonText = std::make_unique<sf::Text>(font, "Play", 40);
    playButtonText->setFillColor(sf::Color::White);
    sf::FloatRect playRect = playButtonText->getLocalBounds();
    playButtonText->setOrigin({playRect.size.x / 2.0f, playRect.size.y / 2.0f});
    playButtonText->setPosition({window.getSize().x / 2.0f, window.getSize().y / 2.0f});

    // Exit Button
    exitButtonText = std::make_unique<sf::Text>(font, "Exit", 40);
    exitButtonText->setFillColor(sf::Color::White);
    sf::FloatRect exitRect = exitButtonText->getLocalBounds();
    exitButtonText->setOrigin({exitRect.size.x / 2.0f, exitRect.size.y / 2.0f});
    exitButtonText->setPosition({window.getSize().x / 2.0f, playButtonText->getPosition().y + 80.f});


    pauseOverlay.setSize(sf::Vector2f(window.getSize()));
    pauseOverlay.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparent black

    pauseText = std::make_unique<sf::Text>(font, "Paused", 60);
    pauseText->setFillColor(sf::Color::White);
    sf::FloatRect pauseRect = pauseText->getLocalBounds();
    pauseText->setOrigin({pauseRect.size.x / 2.0f, pauseRect.size.y / 2.0f});
    pauseText->setPosition({window.getSize().x / 2.0f, window.getSize().y / 4.0f});

    platformerLevel = std::make_unique<PlatformerLevel>();
    platformerView.setSize({static_cast<sf::Vector2f>(window.getSize())});
}


GameEngine::~GameEngine() = default;

void GameEngine::updateMenu(){
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if(playButtonText->getGlobalBounds().contains(mousePos)){
        playButtonText->setFillColor(sf::Color::Yellow);
    }
    else{
        playButtonText->setFillColor(sf::Color::White);
    }

    if(exitButtonText->getGlobalBounds().contains(mousePos)){
        exitButtonText->setFillColor(sf::Color::Yellow);
    }
    else{
        exitButtonText->setFillColor(sf::Color::White);
    }
}

void GameEngine::renderMenu(){
    window.clear(sf::Color(40, 40, 60));//dark blue-grey
    window.draw(*titleText);
    window.draw(*playButtonText);
    window.draw(*exitButtonText);
}

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
                if(currentState == GameState::PLAYING) {
                    currentState = GameState::PAUSED;
                } else if (currentState == GameState::PAUSED) {
                    currentState = GameState::PLAYING;
                }
            }
        }
        if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
            if(keyPressed->code == sf::Keyboard::Key::Tab){
                showMinimap = !showMinimap;
            }
            if(keyPressed->code == sf::Keyboard::Key::Enter && currentState == GameState::PLATFORMER){
                if(player->getBounds().findIntersection(platformerLevel->getEndDoor().getGlobalBounds()).has_value()){
                    currentState = GameState::PLAYING;
                    player->setPosition({(holeRoomX * 1024.f) + 512.f, (holeRoomY * 768.f) + 450.f});
                    window.setView(window.getDefaultView());
                    render();
                    return;
                }   
            }
        }
        if (event->is<sf::Event::MouseButtonPressed>()) {
            if (currentState == GameState::MENU) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                
                // Check if the "Play" button was clicked
                if (playButtonText->getGlobalBounds().contains(mousePos)) {
                    currentState = GameState::PLAYING;
                }

                // Check if the "Exit" button was clicked
                if (exitButtonText->getGlobalBounds().contains(mousePos)) {
                    isRunning = false;
                    window.close();
                }
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

        isPlayerOnPortal = false;
        darkHole.setOutlineColor(sf::Color(80, 0, 120));

        if(currentRoomX == holeRoomX && currentRoomY == holeRoomY){
            if(darkHole.getGlobalBounds().findIntersection(player->getBounds()).has_value()){
                isPlayerOnPortal = true;
                darkHole.setOutlineColor(sf::Color::Magenta);
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)){
                    currentState = GameState::PLATFORMER;
                }
            }
        }
    }
    else if(currentState == GameState::MENU){
        updateMenu();
    }
    else if(currentState == GameState::PLATFORMER){
        player->updatePlatformer(deltaTime, platformerLevel->getPlatforms(), platformerLevel->getSpikes());
        
        float cameraX = std::max(player->getPosition().x, window.getSize().x / 2.f);
        platformerView.setCenter({cameraX, window.getSize().y / 2.f});
        
    }
}

void GameEngine::render() {
    Room& currentRoom = *world[currentRoomY][currentRoomX];
    sf::Color bgColor = currentRoom.getBackgroundColor();

    window.clear(bgColor);

    if (currentState == GameState::PLAYING) {
        currentRoom.render(window);
        if(currentRoomX == holeRoomX && currentRoomY == holeRoomY){
            window.draw(darkHole);
        }
        player->render(window);
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
    }


    if (currentState == GameState::MENU) {
        renderMenu();
        window.display();
        return;
    }

    if(currentState == GameState::PAUSED) {
        window.draw(pauseOverlay);
        window.draw(*pauseText);
    }

    if(currentState == GameState::PLATFORMER){
        window.clear(sf::Color(135, 206, 235));
         // --- Set the camera view ---
        window.setView(platformerView);

        platformerLevel->render(window);
        player->render(window);

        player->render(window);
    }

    window.setView(window.getDefaultView());


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

    do {
        holeRoomX = rand() % worldWidth;
        holeRoomY = rand() % worldHeight;
    } while (holeRoomX == currentRoomX && holeRoomY == currentRoomY); // Ensure it's not in the start room

    darkHole.setRadius(30.f);
    darkHole.setFillColor(sf::Color::Black);
    darkHole.setOutlineThickness(2.f);
    darkHole.setOutlineColor(sf::Color(80, 0, 120)); // Dark purple
    darkHole.setOrigin({darkHole.getRadius(), darkHole.getRadius()});
    darkHole.setPosition({1024 / 2.f, 768 / 2.f});

    isPlayerOnPortal = false;
}