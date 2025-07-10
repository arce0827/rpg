#pragma once
#include <SFML/Graphics.hpp>

class Projectile {
private:
    sf::CircleShape sprite;
    sf::Vector2f velocity;
    float damage;
    bool isActive;

public:
    Projectile(const sf::Vector2f& startPos, const sf::Vector2f& direction, float speed);
    
    ~Projectile();

    void update(float deltaTime, const sf::FloatRect& bounds);
    void render(sf::RenderWindow& window);

    sf::Vector2f getPosition() const {
        return sprite.getPosition();
    }

    float getDamage() const {
        return damage;
    }

    bool getIsActive() const {
        return isActive;
    }

    sf::FloatRect getBounds() const {
        return sprite.getGlobalBounds();
    }

    void setActive(bool state) {
        isActive = state;
    }
}; 