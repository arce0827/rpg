#pragma once
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "managers/CollisionManager.hpp"
#include "entities/Player.hpp"
#include "entities/Enemy.hpp"
#include "world/Room.hpp"

class Player;
class Enemy;

class GameEngine {
    private:
        sf::RenderWindow window;
        std::unique_ptr<Player> player;
        sf::Clock clock;
        bool isRunning;
        CollisionManager collisionManager;

        std::vector<std::vector<std::unique_ptr<Room>>> world;
        int currentRoomX;
        int currentRoomY;

        enum class GameState {
            MENU,
            PLAYING,
            PAUSED,
            GAME_OVER
        };
        GameState currentState;

        void createWorld();

        bool showMinimap;
        std::vector<std::vector<sf::RectangleShape>> minimapRects;
        sf::RectangleShape playerMapIcon;

        public:
        GameEngine();
        ~GameEngine();

        void run();
        void processEvents();
        void update(float deltaTime);
        void render();

        sf::RenderWindow& getWindow() {return window;}
};
