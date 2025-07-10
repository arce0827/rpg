#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Projectile.hpp" // Assumed to be correctly defined

class Enemy {
private:
    sf::CircleShape sprite;
    sf::Vector2f position;
    sf::Vector2f velocity;
    float speed;
    float health;
    float maxHealth;

    // Combat
    sf::Clock shootClock;
    float shootCooldown;
    std::vector<std::unique_ptr<Projectile>> projectiles;

    void handleMovement(float deltaTime, const sf::Vector2f& targetPosition);

public:
    // Constructor
    Enemy(float x, float y);
    ~Enemy() = default;

    // Core Functions
    void update(float deltaTime, const sf::Vector2f& targetPosition);
    void render(sf::RenderWindow& window);

    // Movement and Position
    void setPosition(const sf::Vector2f& pos);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const { return position; }
    void setVelocity(const sf::Vector2f& vel) { velocity = vel; }
    sf::Vector2f getVelocity() const { return velocity; }
    void setSpeed(float newSpeed) { speed = newSpeed; }
    float getSpeed() const { return speed; }

    // Health and Combat
    void takeDamage(float damage);
    void heal(float amount);
    bool isAlive() const { return health > 0; }
    void setHealth(float newHealth);
    float getHealth() const { return health; }
    void setMaxHealth(float newMaxHealth);
    float getMaxHealth() const { return maxHealth; }
    void handleShooting();

    // Projectiles
    std::vector<std::unique_ptr<Projectile>>& getProjectiles();
    void setShootCooldown(float cooldown) { shootCooldown = cooldown; }

    // Utility
    sf::FloatRect getBounds() const;
    void reset();

    // Sprite Properties
    sf::CircleShape& getSprite() { return sprite; }
    void setSpriteColor(const sf::Color& color) { sprite.setFillColor(color); }
    void setSpriteRadius(float radius);
};