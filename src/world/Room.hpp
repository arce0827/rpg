#pragma once
#include "entities/Enemy.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Room{
    public:
        Room(const sf::Texture& backgroundTexture, sf::Color mapColor);
        ~Room() = default;

        void update(float deltaTime, const sf::Vector2f& playerPosition);
        void render(sf::RenderWindow& window);
        void visit();
        bool hasBeenVisited() const;
        void addEnemy(float x, float y);
        sf::Color getMinimapColor() const { return minimapColor; }

        std::vector<std::unique_ptr<Enemy>>& getEnemies();

    private:
        sf::Sprite backgroundSprite;
        std::vector<std::unique_ptr<Enemy>> enemies;
        bool isVisited;
        sf::Color minimapColor;
};