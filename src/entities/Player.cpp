#include "Player.hpp"
#include "Projectile.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

Player::Player(float x, float y) :
    position(x, y),
    velocity(0.0f, 0.0f),
    speed(200.0f),
    health(100.0f),
    maxHealth(100.0f),
    shootCooldown(0.3f) {
    
    // Setup sprite
    sprite.setRadius(20.0f);
    sprite.setFillColor(sf::Color::Blue);
    sprite.setOrigin(sprite.getRadius(), sprite.getRadius());
    sprite.setPosition(position);
}

Player::~Player() = default;

void Player::update(float deltaTime) {
    handleInput(deltaTime);
    handleShooting();
    
    // Update position
    position += velocity * deltaTime;
    sprite.setPosition(position);
    
    // Update projectiles
    for (auto it = projectiles.begin(); it != projectiles.end();) {
        (*it)->update(deltaTime);
        
        // Remove projectiles that are off-screen
        sf::Vector2f projPos = (*it)->getPosition();
        if (projPos.x < -50 || projPos.x > 1074 || projPos.y < -50 || projPos.y > 818) {
            it = projectiles.erase(it);
        } else {
            ++it;
        }
    }
}

void Player::handleInput(float deltaTime) {
    velocity = sf::Vector2f(0.0f, 0.0f);
    
    // Movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        velocity.y -= speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        velocity.y += speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        velocity.x -= speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        velocity.x += speed;
    }
    
    // Normalize diagonal movement
    if (velocity.x != 0 && velocity.y != 0) {
        velocity.x *= 0.707f;
        velocity.y *= 0.707f;
    }
}

void Player::handleShooting() {
    if (shootClock.getElapsedTime().asSeconds() >= shootCooldown) {
        sf::Vector2f shootDirection(0.0f, 0.0f);
        
        // Shooting with arrow keys
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            shootDirection.y = -1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            shootDirection.y = 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            shootDirection.x = -1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            shootDirection.x = 1.0f;
        }
        
        // If shooting diagonally, normalize
        if (shootDirection.x != 0 && shootDirection.y != 0) {
            shootDirection.x *= 0.707f;
            shootDirection.y *= 0.707f;
        }
        
        // Create projectile if shooting
        if (shootDirection.x != 0 || shootDirection.y != 0) {
            projectiles.push_back(std::make_unique<Projectile>(position, shootDirection, 400.0f));
            shootClock.restart();
        }
    }
}

void Player::setPosition(const sf::Vector2f& pos) {
    position = pos;
    sprite.setPosition(position);
}

void Player::render(sf::RenderWindow& window) {
    window.draw(sprite);
    
    // Render projectiles
    for (auto& projectile : projectiles) {
        projectile->render(window);
    }
}

void Player::takeDamage(float damage) {
    health -= damage;
    if (health < 0) health = 0;
}

void Player::heal(float amount) {
    health += amount;
    if (health > maxHealth) health = maxHealth;
}