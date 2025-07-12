#include "entities/Player.hpp"
#include "entities/Projectile.hpp"
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
    sprite.setOrigin({sprite.getRadius(), sprite.getRadius()});
    sprite.setPosition(position);

    float healthBarWidth = 200.0f;
    float healthBarHeight = 15.0f;
    healthBarBackground.setSize({healthBarWidth, healthBarHeight});
    healthBarBackground.setFillColor(sf::Color(100, 100, 100, 200));
    healthBarFill.setSize({healthBarWidth, healthBarHeight});
    healthBarFill.setFillColor(sf::Color::Green);
}

void Player::update(float deltaTime, const sf::RenderWindow& window) {
    handleInput(deltaTime);
    handleShooting();
    
    // Update position
    position += velocity * deltaTime;
    sprite.setPosition(position);
    
    sf::FloatRect bounds({0.0f, 0.0f}, {static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)});

    // Update projectiles
    for (auto it = projectiles.begin(); it != projectiles.end();) {
        (*it)->update(deltaTime, bounds);
        
        // Remove projectiles that are off-screen
        if (!(*it)->getIsActive()) {
            it = projectiles.erase(it);
        } else {
            ++it;
        }
    }

    healthBarBackground.setPosition({120.f, 25.f});
    healthBarFill.setPosition({120.f, 25.f});

    float healthPercent = health / maxHealth;
    healthBarFill.setSize({healthBarBackground.getSize().x * healthPercent, healthBarFill.getSize().y});
}

void Player::handleInput(float deltaTime) {
    velocity = sf::Vector2f(0.0f, 0.0f);
    
    // Movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
        velocity.y -= speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
        velocity.y += speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) {
        velocity.x -= speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up)) {
            shootDirection.y = -1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down)) {
            shootDirection.y = 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left)) {
            shootDirection.x = -1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right)) {
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

    // Render health bar
    window.draw(healthBarBackground);
    window.draw(healthBarFill);
}

void Player::takeDamage(float damage) {
    health -= damage;
    if (health < 0) health = 0;
}

void Player::heal(float amount) {
    health += amount;
    if (health > maxHealth) health = maxHealth;
}

bool Player::isAlive() const{
    return health > 0;
}

sf::FloatRect Player::getBounds() const{
    return sprite.getGlobalBounds();
}

void Player::updatePlatformer(float deltaTime, const std::vector<sf::RectangleShape>& platforms, const std::vector<sf::RectangleShape>& spikes) {
    const float gravity = 1100.0f;
    const float jumpStrength = 650.0f;
    const float moveSpeed = 300.0f;

    platformerVelocity.x = 0;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)){
        platformerVelocity.x -= moveSpeed; 
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)){
        platformerVelocity.x += moveSpeed;
    }

    sprite.move({platformerVelocity.x * deltaTime, 0});

        // Horizontal Collision Check
    for (const auto& platform : platforms) {
        if (sprite.getGlobalBounds().findIntersection(platform.getGlobalBounds()).has_value()) {
            // Collision detected, move player back
            if (platformerVelocity.x > 0) { // Moving right
                sprite.setPosition({platform.getPosition().x - sprite.getRadius(), sprite.getPosition().y});
            } else if (platformerVelocity.x < 0) { // Moving left
                sprite.setPosition({platform.getPosition().x + platform.getSize().x + sprite.getRadius(), sprite.getPosition().y});
            }
        }
    }

    // --- Vertical Movement ---
    platformerVelocity.y += gravity * deltaTime;
    sprite.move({0, platformerVelocity.y * deltaTime});
    isOnGround = false; // Assume not on ground until proven otherwise

    // Vertical Collision Check
    for (const auto& platform : platforms) {
        if (sprite.getGlobalBounds().findIntersection(platform.getGlobalBounds()).has_value()) {
            if (platformerVelocity.y > 0) { // Moving down
                sprite.setPosition({sprite.getPosition().x, platform.getPosition().y - sprite.getRadius()});
                platformerVelocity.y = 0;
                isOnGround = true;
            } else if (platformerVelocity.y < 0) { // Moving up (bonking head)
                sprite.setPosition({sprite.getPosition().x, platform.getPosition().y + platform.getSize().y + sprite.getRadius()});
                platformerVelocity.y = 0;
            }
        }
    }

    // --- Jumping ---
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space) && isOnGround) {
        platformerVelocity.y = -jumpStrength;
        isOnGround = false;
    }
    
    // --- Spike Collision ---
    for (const auto& spike : spikes) {
        if (sprite.getGlobalBounds().findIntersection(spike.getGlobalBounds()).has_value()) {
            sprite.setPosition({100, 600});
            platformerVelocity = {0,0};
        }
    }
}