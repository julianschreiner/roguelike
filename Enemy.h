//
// Created by Julian Schreiner on 12.12.17.
//
#include <string>
#ifndef ROGUELIKE_SFML_ENEMY_H
#define ROGUELIKE_SFML_ENEMY_H


class Enemy {
public:
    Enemy();
    Enemy(std::string name, int level, int health, int attack, int defense, int experience);
    void setPosition(int enemy_x, int enemy_y);
    void getPosition(int &enemy_x, int &enemy_y);
    std::string toString();
    std::string getEnemyName();
    int retPosX();
    int retPosY();
    int getEnemyAttackDMG();
    int getEnemyHealth();
    int getEnemyExperience();
    int getEnemyDefense();
    void updateEnemyHealth(int newHealth);

private:
    std::string enemy_name;
    int enemy_level;
    int enemy_health;
    int enemy_Attack;
    int enemy_defense;
    int enemy_experience;
    int enemy_x;
    int enemy_y;
};


#endif //ROGUELIKE_SFML_ENEMY_H
