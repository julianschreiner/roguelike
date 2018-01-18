//
// Created by Julian Schreiner on 12.12.17.
//

#include "Enemy.h"
#include <string>

Enemy::Enemy(){
    this->enemy_x = 0;
    this->enemy_y = 0;
}

Enemy::Enemy(std::string name, int level, int health, int attack, int defense, int experience){
    this->enemy_name = name;
    this->enemy_level = level;
    this->enemy_health = health;
    this->enemy_Attack = attack;
    this->enemy_defense = defense;
    this->enemy_experience = experience;
}

void Enemy::setPosition(int enemy_x, int enemy_y){
    this->enemy_x = enemy_x;
    this->enemy_y = enemy_y;
}

void Enemy::getPosition(int &enemy_x, int &enemy_y){
    enemy_x = this->enemy_x;
    enemy_y = this->enemy_y;
}

std::string Enemy::toString() {
    return this->enemy_name + "/" +
            std::to_string(this->enemy_level) + "/" +
            std::to_string(this->enemy_health) + "/" +
            std::to_string(this->enemy_Attack) + "/" +
            std::to_string(this->enemy_defense) + "/" +
            std::to_string(this->enemy_experience);
}

int Enemy::retPosX(){
    return this->enemy_x;
}
int Enemy::retPosY(){
    return this->enemy_y;
}

std::string Enemy::getEnemyName(){
    return this->enemy_name;
}

int Enemy::getEnemyAttackDMG(){
    return this->enemy_Attack;
}
int Enemy::getEnemyHealth(){
    return this->enemy_health;
}

void Enemy::updateEnemyHealth(int newHealth){
    this->enemy_health = newHealth;
}

int Enemy::getEnemyExperience(){
    return this->enemy_experience;
}

int Enemy::getEnemyDefense() {
    return this->enemy_defense;
}