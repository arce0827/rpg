#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class PlatformerLevel {
    public:
        PlatformerLevel();
        ~PlatformerLevel() = default;

        void render(sf::RenderWindow& window);

        const std::vector<sf::RectangleShape>& getPlatforms() const;
        const std::vector<sf::RectangleShape>& getSpikes() const;
        const sf::RectangleShape& getEndDoor() const;
    private:
        std::vector<sf::RectangleShape> platforms;
        std::vector<sf::RectangleShape> spikes;
        sf::RectangleShape endDoor;
};