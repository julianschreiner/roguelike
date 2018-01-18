//
// Created by Julian Schreiner on 12.12.17.
//

#ifndef ROGUELIKE_SFML_LEVEL_H
#define ROGUELIKE_SFML_LEVEL_H
#include <string>
#include "Player.h"
#include "Enemy.h"
#include <vector>

class Level {
public:
    Level();
    void load(std::string fileName, Player &player);
    std::string printLevel();
    char getTile(int x, int y);
    void setTile(int x, int y, char tile);
    void movePlayer(char input, Player &player);
    void moveEnemy(std::vector<Enemy> enemies, Player &player);
    void initMessages();
    std::vector<std::string> getMessages();
    int countAliveEnemies();
    bool getNextLevel();

private:
    void processPlayerMove(Player &player, int targetX, int targetY);
    void processEnemyMove(Enemy &enemy, int targetX, int targetY, Player &player);
    int battleMonster(Player &player, int targetX, int targetY);
    int battlePlayer(Enemy &enemy, Player &player, int targetX, int targetY);

private:
    std::vector<std::string> levelData;
    std::vector<Enemy> enemies;
    std::vector<std::string> messages;
    bool nextLevel;
};


#endif //ROGUELIKE_SFML_LEVEL_H
