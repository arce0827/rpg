#include "world/Room.hpp"
#include <iostream>

Room::Room(const sf::Texture& backgroundTexture, sf::Color mapColor)
    : backgroundSprite(backgroundTexture),
        isVisited(false),
        minimapColor(mapColor)
{
    backgroundSprite.setTexture(backgroundTexture);
}

void Room::addEnemy(float x, float y){
    enemies.push_back(std::make_unique<Enemy>(x, y));
    //DEBUG
    std::cout << "Added enemy at position: (" << x << ", " << y << ")\n";
}

void Room::render(sf::RenderWindow& window){
    window.draw(backgroundSprite);
    for(auto& enemy : enemies){
        if(enemy->isAlive()){
            enemy->render(window);
        }
    }
}

void Room::update(float deltaTime, const sf::Vector2f& playerPosition){
    for(auto& enemy : enemies){
        if(enemy->isAlive()){
            enemy->update(deltaTime, playerPosition);

        }
    }
    // Remove dead enemies
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const std::unique_ptr<Enemy>& enemy) { return !enemy->isAlive(); }), enemies.end());
}

std::vector<std::unique_ptr<Enemy>>& Room::getEnemies(){
    return enemies;
}

void Room::visit(){
    isVisited = true;
}
bool Room::hasBeenVisited() const {
    return isVisited;
}
