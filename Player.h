//
// Created by Julian Schreiner on 12.12.17.
//

#ifndef ROGUELIKE_SFML_PLAYER_H
#define ROGUELIKE_SFML_PLAYER_H
#include <string>

class Player {
public:
    Player();
    void init(std::string name, int level, int health, int attack, int defense, int experience, int gold, bool dead);
    void setPosition(int x, int y);
    void getPosition(int &x, int &y);
    int getPlayerAttack();
    std::string updateExperience(int exp);
    int getExperience();
    int getPlayerHealth();
    void updatePlayerHealth(int newHealth);
    int getDefense();
    void setGold(int gold);
    int getGold();
    void setDead(bool dead);
    bool getDead();
    std::string getName();
    void resetHealth();
    int getLevel();

private:
    std::string name;
    int level;
    int health;
    int playerAttack;
    int defense;
    int experience;
    int gold;
    bool dead;
    int x;
    int y;

};


#endif //ROGUELIKE_SFML_PLAYER_H
