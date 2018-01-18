//
// Created by Julian Schreiner on 12.12.17.
//

#include "GameSystem.h"
#include <iostream>

#define LEVEL1 "levels/level1.txt"
#define LEVEL2 "levels/level2.txt"

int levelCounterInc(1);

GameSystem::GameSystem(std::string levelFileName, std::string username) {
    this->player.init(username, 1, 100, 9, 4, 0, 0, false);
    this->level.load(levelFileName, this->player);
    this->level.initMessages();
}

bool GameSystem::gameEnded() {
    //CHECK if player died
    return this->player.getDead();
}

std::string GameSystem::getLevel() {
    return this->level.printLevel();
}

void GameSystem::playerMove(char input) {
    this->level.movePlayer(input, this->player);
    //this->level.processPlayerInstance(this->player*);

    if(this->level.getNextLevel()){
        this->startNextLevel = true;
       // this->nextLevel();
    }
}

void GameSystem::clearGame(){
    std::string username = this->player.getName();
    this->player = Player();
    this->level = Level();

    this->player.init(username, 1, 100, 9, 4, 0, 0, false);
    this->level.load(LEVEL1, this->player);
}

std::map<int,std::string> GameSystem::getPlayerStats(){
    std::map<int, std::string> stats;

    stats[1] = std::to_string(this->player.getPlayerHealth());
    stats[2] = std::to_string(this->player.getPlayerAttack());
    stats[3] = std::to_string(this->player.getExperience());
    stats[4] = std::to_string(this->player.getGold());
    stats[5] = this->player.getName();

    return stats;
}

std::vector<std::string> GameSystem::getActions(){
    return this->level.getMessages();
}

bool GameSystem::allEnemiesDead(){
    return this->level.countAliveEnemies() == 0;
}

void GameSystem::makeDoor(){
    this->level.setTile(30,0,'+');
}

void GameSystem::nextLevel(){
    ::levelCounterInc++;
    std::string levelName;
    this->startNextLevel = false;
    this->player.init(
            this->player.getName(),
            this->player.getLevel(),
            this->player.getPlayerHealth(),
            this->player.getPlayerAttack(),
            this->player.getDefense(),
            this->player.getExperience(),
            this->player.getGold(),
            false
    );

    levelName = "levels/level" + std::to_string(::levelCounterInc) + ".txt";
    std::cout << levelName << std::endl;

    this->level.load(levelName, this->player);
    //this->level.initMessages();
}

bool GameSystem::getStartNextLevel(){
    return this->startNextLevel;
}