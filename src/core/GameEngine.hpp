#pragma once
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Player;
class Enemy;

class GameEngine {
    private:
        sf::RenderWindow window;
        std::unique_ptr<Player> player;
        std::vector<std::unique_ptr<Enemy>> enemies;
        sf::Clock clock;
        bool isRunning;

        enum class GameState {
            MENU,
            PLAYING,
            PAUSED,
            GAME_OVER
        };
        GameState currentState;

        void spawnEnemies();
        void updateEnemies(float deltaTime);
        void renderEnemies();

        public:
        GameEngine();
        ~GameEngine();

        void run();
        void processEvents();
        void update(float deltaTime);
        void render();

        sf::RenderWindow& getWindow() {return window;}
};
