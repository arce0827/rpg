#pragma once
#include "entities/Enemy.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Room{
    public:
        Room(sf::Color backgroundColor = sf::Color::Black);
        ~Room() = default;

        void update(float deltaTime, const sf::Vector2f& playerPosition);
        void render(sf::RenderWindow& window);

        void addEnemy(float x, float y);

        std::vector<std::unique_ptr<Enemy>>& getEnemies();
        sf::Color getBackgroundColor() const;

    private:
        sf::Color backgroundColor;
        std::vector<std::unique_ptr<Enemy>> enemies;
};