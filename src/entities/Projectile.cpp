#include "entities/Projectile.hpp"

// The constructor no longer needs to initialize a separate 'position' member.
Projectile::Projectile(const sf::Vector2f& startPos, const sf::Vector2f& direction, float speed) :
    velocity(direction * speed),
    damage(10.0f),
    isActive(true) {

    // Setup sprite
    sprite.setRadius(5.0f);
    sprite.setFillColor(sf::Color::Yellow);
    sprite.setOrigin({sprite.getRadius(), sprite.getRadius()});
    sprite.setPosition(startPos); // Set the sprite's position directly.
}

Projectile::~Projectile() = default;

// The update function now takes the screen bounds as an argument.
void Projectile::update(float deltaTime, const sf::FloatRect& bounds) {
    if (!isActive) return;

    // Update position directly on the sprite.
    sprite.move(velocity * deltaTime);

    // Deactivate if off-screen by checking if the bounds contain the sprite's position.
    if (!bounds.contains(sprite.getPosition())) {
        isActive = false;
    }
}

void Projectile::render(sf::RenderWindow& window) {
    if (isActive) {
        window.draw(sprite);
    }
}