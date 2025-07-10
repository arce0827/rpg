#include "entities/Enemy.hpp"
#include <cmath>
#include <iostream>

// "Enemy::" is added to the constructor
Enemy::Enemy(float x, float y)
    : position(x, y),
      speed(100.0f),
      health(100.0f),
      maxHealth(100.0f),
      shootCooldown(2.0f)

{
    sprite.setRadius(20.0f);
    sprite.setFillColor(sf::Color::Red);
    sprite.setOrigin({sprite.getRadius(), sprite.getRadius()});
    sprite.setPosition(position);
}

// "Enemy::" is added to handleMovement
void Enemy::handleMovement(float deltaTime, const sf::Vector2f& targetPosition) {
    sf::Vector2f direction = targetPosition - position;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        direction /= length;
    }
    velocity = direction * speed;
    position += velocity * deltaTime;
}

// "Enemy::" is added to update
void Enemy::update(float deltaTime, const sf::Vector2f& targetPosition) {
    if (!isAlive()){
        //DEBUG
        static int deadCount = 0;
        if (deadCount % 60 == 0){
            std::cout <<"Enemy is dead "<<std::endl;
        }
        deadCount++;
        return;
    }

    handleMovement(deltaTime, targetPosition);
    handleShooting();
    sprite.setPosition(position);
}

// "Enemy::" is added to render
void Enemy::render(sf::RenderWindow& window) {
    if (isAlive()) {
        window.draw(sprite);
        //DEBUG
        static int renderCount = 0;
        if (renderCount % 60 == 0){
            std::cout<<"Rendering enemy at position: "<< position.x << ", " << position.y << std::endl;
        }
        renderCount++;
    }
}

// "Enemy::" is added to handleShooting
void Enemy::handleShooting() {
    if (shootClock.getElapsedTime().asSeconds() >= shootCooldown) {
        sf::Vector2f projectileDirection(0.f, 1.f);
        float projectileSpeed = 400.0f;
        projectiles.push_back(std::make_unique<Projectile>(position, projectileDirection, projectileSpeed));
        shootClock.restart();
    }
}

// "Enemy::" is added to takeDamage
void Enemy::takeDamage(float damage) {
    health -= damage;
    if (health <= 0) {
        health = 0;
    }
}

// "Enemy::" is added to heal
void Enemy::heal(float amount) {
    health += amount;
    if (health > maxHealth) {
        health = maxHealth;
    }
}

// "Enemy::" is added to reset
void Enemy::reset() {
    health = maxHealth;
    position = sf::Vector2f(0, 0);
    velocity = sf::Vector2f(0, 0);
    sprite.setFillColor(sf::Color::Red);
    sprite.setPosition(position);
}

// "Enemy::" is added to getProjectiles
std::vector<std::unique_ptr<Projectile>>& Enemy::getProjectiles() {
    return projectiles;
}

// "Enemy::" is added to getBounds
sf::FloatRect Enemy::getBounds() const {
    return sprite.getGlobalBounds();
}

// "Enemy::" is added to setPosition
void Enemy::setPosition(const sf::Vector2f& pos) {
    position = pos;
    sprite.setPosition(position);
}

// "Enemy::" is added to setPosition
void Enemy::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
    sprite.setPosition(position);
}

// "Enemy::" is added to setHealth
void Enemy::setHealth(float newHealth) {
    health = newHealth;
    if (health > maxHealth) health = maxHealth;
    if (health < 0) health = 0;
}

// "Enemy::" is added to setMaxHealth
void Enemy::setMaxHealth(float newMaxHealth) {
    maxHealth = newMaxHealth;
    if (health > maxHealth) {
        health = maxHealth;
    }
}

// "Enemy::" is added to setSpriteRadius
void Enemy::setSpriteRadius(float radius) {
    sprite.setRadius(radius);
    sprite.setOrigin({radius, radius});
}