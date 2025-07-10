#include "managers/CollisionManager.hpp"

void CollisionManager::checkPlayerProjectilesVsEnemies(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies){
    auto& projectiles = player.getProjectiles();
    for (auto& projectile : projectiles){
        if (!projectile->getIsActive()){
            continue;
        }
        for (auto& enemy : enemies){
            if(!enemy->isAlive()){
                continue;
            }
            if(projectile->getBounds().findIntersection(enemy->getBounds()).has_value()){
                enemy->takeDamage(projectile->getDamage());
                projectile->setActive(false);
                break;
            }
        }

    }
}
void CollisionManager::checkEnemyProjectilesVsPlayer(std::vector<std::unique_ptr<Enemy>>& enemies, Player& player){
    if(!player.isAlive()){
        return;
    }
    for (auto& enemy : enemies){
        auto& projectiles = enemy->getProjectiles();
        for(auto& projectile : projectiles){
            if(!projectile->getIsActive()){
                continue;
            }
            if(projectile->getBounds().findIntersection(player.getBounds()).has_value()){
                player.takeDamage(projectile->getDamage());
                projectile->setActive(false);
                break;
            }
        }
    }
}
void CollisionManager::checkEntityCollisions(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies){
    if(!player.isAlive()){
        return;
    }
    for (auto& enemy : enemies){
        if(!enemy->isAlive()){
            continue;
        }
        if(enemy->getBounds().findIntersection(player.getBounds()).has_value()){
            player.takeDamage(0.5f);
            enemy->takeDamage(1.0f);
        }
    }
}