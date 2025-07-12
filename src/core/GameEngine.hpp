#pragma once
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "managers/CollisionManager.hpp"
#include "entities/Player.hpp"
#include "entities/Enemy.hpp"
#include "world/Room.hpp"
#include "world/PlatformerLevel.hpp"

class Player;
class Enemy;

class GameEngine {
    private:
        sf::RenderWindow window;
        std::unique_ptr<Player> player;
        sf::Clock clock;
        bool isRunning;
        CollisionManager collisionManager;

        //MENU
        sf::Font font;
        std::unique_ptr<sf::Text> titleText;
        std::unique_ptr<sf::Text> playButtonText;
        std::unique_ptr<sf::Text> exitButtonText;

        //Paused
        sf::RectangleShape pauseOverlay;
        std::unique_ptr<sf::Text> pauseText;

        void updateMenu();
        void renderMenu();

        std::vector<std::vector<std::unique_ptr<Room>>> world;
        int currentRoomX;
        int currentRoomY;

        std::unique_ptr<PlatformerLevel> platformerLevel;
        sf::View platformerView;

        enum class GameState {
            MENU,
            PLAYING,
            PAUSED,
            GAME_OVER,
            PLATFORMER
        };
        GameState currentState;

        void createWorld();

        bool showMinimap;
        std::vector<std::vector<sf::RectangleShape>> minimapRects;
        sf::RectangleShape playerMapIcon;

        //Platformer
        sf::CircleShape darkHole;
        int holeRoomX;
        int holeRoomY;
        bool isPlayerOnPortal;

        public:
        GameEngine();
        ~GameEngine();

        void run();
        void processEvents();
        void update(float deltaTime);
        void render();

        sf::RenderWindow& getWindow() {return window;}
};
