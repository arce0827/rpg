#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Player;

class GameEngine {
    private:
        sf::RenderWindow window;
        std::unique_ptr<Player> player;
        sf::Clock clock;
        bool isRunning;

        enum class GameState {
            MENU,
            PLAYING,
            PAUSED,
            GAME_OVER
        };
        GameState currentState;
        public:
        GameEngine();
        ~GameEngine();

        void run();
        void processEvents();
        void update(float deltaTime);
        void render();

        sf::RenderWindow& getWindow() {return window;}
};
