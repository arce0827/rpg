#include "entities/Projectile.hpp"

Projectile::Projectile(const sf::Vector2f& startPos, const sf::Vector2f& direction, float speed) :
    position(startPos),
    velocity(direction * speed),
    damage(10.0f),
    isActive(true) {

    // Setup sprite
    sprite.setRadius(5.0f);
    sprite.setFillColor(sf::Color::Yellow);
    sprite.setOrigin({sprite.getRadius(), sprite.getRadius()});
    sprite.setPosition(position);
}

Projectile::~Projectile() = default;

void Projectile::update(float deltaTime) {
    if (!isActive) return;

    // Update position
    position += velocity * deltaTime;
    sprite.setPosition(position);

    // Deactivate if off-screen
    if (position.x < -50 || position.x > 1074 || position.y < -50 || position.y > 818) {
        isActive = false;
    }
}

void Projectile::render(sf::RenderWindow& window) {
    if (isActive) {
        window.draw(sprite);
    }
}