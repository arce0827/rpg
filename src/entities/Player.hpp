#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Projectile;

class Player {
private:
    sf::CircleShape sprite;
    sf::Vector2f position;
    sf::Vector2f velocity;
    float speed;
    float health;
    float maxHealth;
    
    std::vector<std::unique_ptr<Projectile>> projectiles;
    sf::Clock shootClock;
    float shootCooldown;
    
    void handleInput(float deltaTime);
    void handleShooting();

    sf::RectangleShape healthBarBackground;
    sf::RectangleShape healthBarFill;

public:
    Player(float x, float y);
    ~Player() = default;
    
    void update(float deltaTime, const sf::RenderWindow& window);
    void render(sf::RenderWindow& window);
    
    sf::Vector2f getPosition() const { return position; }
    void setPosition(const sf::Vector2f& pos);
    float getRadius() const { return sprite.getRadius(); }
    float getHealth() const { return health; }
    float getMaxHealth() const { return maxHealth; }
    
    // Combat
    void takeDamage(float damage);
    void heal(float amount);
    std::vector<std::unique_ptr<Projectile>>& getProjectiles() { return projectiles; }
    bool isAlive() const;
    sf::FloatRect getBounds() const;
};