//
// Created by Julian Schreiner on 12.12.17.
//

#ifndef ROGUELIKE_SFML_GAMESYSTEM_H
#define ROGUELIKE_SFML_GAMESYSTEM_H
#include<string>
#include "Level.h"
#include "Player.h"
#include "Enemy.h"
#include <string>
#include <map>

class GameSystem {
public:
        GameSystem(std::string levelFileName, std::string username);
        bool gameEnded();
        std::string getLevel();
        void playerMove(char input);
        void clearGame();
        std::map<int, std::string> getPlayerStats();
        std::vector<std::string> getActions();
        bool allEnemiesDead();
        void makeDoor();
        void nextLevel();
        bool getStartNextLevel();

private:
        Level level;
        Player player;
        bool startNextLevel;
};


#endif //ROGUELIKE_SFML_GAMESYSTEM_H
