#pragma once
#include "entities/Enemy.hpp"
#include "entities/Player.hpp"
#include <vector>
#include <memory>

class CollisionManager{
    public:
        void checkPlayerProjectilesVsEnemies(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies);
        void checkEnemyProjectilesVsPlayer(std::vector<std::unique_ptr<Enemy>>& enemies, Player& player);
        void checkEntityCollisions(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies);
};