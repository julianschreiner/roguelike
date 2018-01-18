//
// Created by Julian Schreiner on 12.12.17.
//

#include "Level.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cstdio>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>


Level::Level() {

}

void Level::load(std::string fileName, Player &player) {
    std::ifstream file;
    Enemy *en = new Enemy();

    file.open(fileName);
    if (file.fail()) {
        //SHOW HIGHSCORE IF FAILS
        //TODO TELL FRONTEND THAT GAME IS FINISHED TO SHOW HIGHSCORE
        perror(fileName.c_str());
        exit(1);
    }

    std::string line;
    while (getline(file, line)) {
        this->levelData.push_back(line);
    }

    file.close();

    //Process the Level
    char tile;
    for (int i = 0; i < this->levelData.size(); i++) {
        for (int j = 0; j < this->levelData[i].size(); j++) {     //Loop In Row
            tile = this->levelData[i][j];
            /* j = x , i = y */
            switch (tile) {
                case '@':   //Player
                    player.setPosition(j, i);
                    //std::cout << j << "/" << i << std::endl;
                    break;
                case 'R':   //Raeuber
                    this->enemies.push_back(Enemy("Raeuber", 1, 20, 5, 3, 40));
                    this->enemies.back().setPosition(j, i);
                    break;
                case 'S':   //SNAKE
                    this->enemies.push_back(Enemy("Snake", 1, 40, 10, 4, 60));
                    this->enemies.back().setPosition(j, i);
                    break;
                case 'D':      //DRAGON
                    this->enemies.push_back(Enemy("Dragon", 100, 450, 10000, 1000, 10000));
                    this->enemies.back().setPosition(j, i);
                default:
                    break;
            }
        }
    }
}

std::string Level::printLevel() {
    std::string ret;
    for (int i = 0; i < this->levelData.size(); i++) {
        ret += this->levelData[i] + "\n";
    }
    // std::cout << ret << std::endl;
    return ret;
}

char Level::getTile(int x, int y) {
    return this->levelData[y][x];
}

void Level::setTile(int x, int y, char tile) {
    this->levelData[y][x] = tile;
}


void Level::movePlayer(char input, Player &player) {
    int playerX;
    int playerY;
    player.getPosition(playerX, playerY); //gives our Local Variables the Values
    this->nextLevel = false;

    switch (input) {
        case 'w':   //up
        case 'W':
            this->processPlayerMove(player, playerX, playerY - 1);
            break;
        case 's':   //Down
        case 'S':
            this->processPlayerMove(player, playerX, playerY + 1);
            break;
        case 'a':   //Left
        case 'A':
            this->processPlayerMove(player, playerX - 1, playerY);
            break;
        case 'd':   //Right
        case 'D':
            this->processPlayerMove(player, playerX + 1, playerY);
            break;
        default:
            printf("Invalid Input\n");
            this->messages.push_back("Invalid Input");
            break;
    }

}

void Level::processPlayerMove(Player &player, int targetX, int targetY) {
    int playerX;
    int playerY;
    int deadControl = 0;
    player.getPosition(playerX, playerY);
    this->nextLevel = false;

    char moveTile = getTile(targetX, targetY);

    switch (moveTile) {
        case '#':
            break;
        case '.':
            player.setPosition(targetX, targetY);
            setTile(playerX, playerY, '.');
            setTile(targetX, targetY, '@');
            break;
        case '*':
            //GIVE +10 GOLD
            player.setGold(10);
            player.setPosition(targetX, targetY);
            setTile(playerX, playerY, '.');
            setTile(targetX, targetY, '@');
            this->messages.push_back("+10 Gold earned");
            break;
        case '+':
            //ENTER NEW WORLD
            this->levelData.clear();
            this->enemies.clear();
            player.resetHealth();
            this->nextLevel = true;

        default:
            //ATTACKING MONSTER
            std::cout << "Player is battling Enemy" << std::endl;
            this->messages.push_back("Player attacks enemy");
            deadControl = this->battleMonster(player, targetX, targetY);
            if (deadControl) {       //if RET eq 1 ->ded
                std::cout << "battledMonster!" << std::endl;
                this->messages.push_back("Monster died");
                setTile(targetX, targetY, '.');
            }
            break;
    }
    this->moveEnemy(this->enemies, player);
}

void Level::moveEnemy(std::vector<Enemy> enemies, Player &player) {
    int enemyX;
    int enemyY;
    int random_int;

    std::vector<char> keys;
    keys.push_back('w');
    keys.push_back('a');
    keys.push_back('s');
    keys.push_back('d');

    srand(time(NULL));

    for (int i = 0; i < enemies.size(); i++) {
        this->enemies[i].getPosition(enemyX, enemyY); //gives our Local Variables the Values

        random_int = rand() % 4; // 0 - 3
        //std::cout << enemyX << "/" << enemyY << std::endl;

        switch (keys[random_int]) {
            case 'w':   //up
                this->processEnemyMove(this->enemies[i], enemyX, enemyY - 1, player);
                break;
            case 's':   //Down
                this->processEnemyMove(this->enemies[i], enemyX, enemyY + 1, player);
                break;
            case 'a':   //Left
                this->processEnemyMove(this->enemies[i], enemyX - 1, enemyY, player);
                break;
            case 'd':   //Right
                this->processEnemyMove(this->enemies[i], enemyX + 1, enemyY, player);
                break;
            default:
                printf("Invalid Input\n");
                break;
        }
    }

}

void Level::processEnemyMove(Enemy &enemy, int targetX, int targetY, Player &player) {
    int enemyX;
    int enemyY;
    int deadControl = 0;

    enemy.getPosition(enemyX, enemyY); //gives our Local Variables the Values

    char moveTile = getTile(targetX, targetY);

    switch (moveTile) {
        case '.':
            enemy.setPosition(targetX, targetY);
            setTile(enemyX, enemyY, '.');
            setTile(targetX, targetY, enemy.getEnemyName().at(0));
            break;
        case '@':
            std::cout << "Enemy is battling Player" << std::endl;
            this->messages.push_back("Enemy attacks " + player.getName());
            deadControl = this->battlePlayer(enemy, player, targetX, targetY);
            if (deadControl) {        //if RET eq 1 -> ded
                std::cout << "battledPlayer!" << std::endl;
                this->messages.push_back(player.getName() + " died.");
                setTile(targetX, targetY, '.');
                player.setDead(true);
            }
            break;
        default:
            break;
    }
}

int Level::battleMonster(Player &player, int targetX, int targetY) {
    int playerDamage = player.getPlayerAttack();
    int en_x, en_y, en_hp, playerDefense, en_dead = 0;

    //GET ENEMY IN POS TARGETX / TARGET Y
    for (int i = 0; i < this->enemies.size(); i++) {
        en_x = this->enemies[i].retPosX();
        en_y = this->enemies[i].retPosY();
        //  std::cout << en_x << en_y << std::endl;
        if (targetX == en_x && targetY == en_y) {
            en_hp = this->enemies[i].getEnemyHealth();
            playerDefense = this->enemies[i].getEnemyDefense();
            playerDamage -= playerDefense;

            if(playerDamage >= 0){
                en_hp -= playerDamage;

            }
            else{
                std::cout << "Good Defense!" << std::endl;
                en_hp = this->enemies[i].getEnemyHealth();
            }

            this->enemies[i].updateEnemyHealth(en_hp);
            std::cout << "Enemy HP: " << en_hp << std::endl;

            if (en_hp <= 0) {
                //MONSTER DIED
                //Upd Experience to give user power ups
                this->messages.push_back(
                      player.updateExperience(this->enemies[i].getEnemyExperience())
                );

                this->messages.push_back(
                        "+ " + std::to_string(this->enemies[i].getEnemyExperience()) + " XP"
                );

                en_dead = 1;

                this->enemies.erase(this->enemies.begin() + i);
            }
        }
    }

    return en_dead;
}

int Level::battlePlayer(Enemy &enemy, Player &player, int targetX, int targetY) {
    int enemyDamage = enemy.getEnemyAttackDMG();
    int playerDefense = player.getDefense();

    int pl_hp, pl_dead = 0;

    pl_hp = player.getPlayerHealth();
    enemyDamage -= playerDefense;
//    std::cout << "Enemy DMG: " << enemyDamage << std::endl;

    if(enemyDamage >= 0){
        pl_hp -= enemyDamage;
    }
    else{
        std::cout << "Good Defense!" << std::endl;
        pl_hp = player.getPlayerHealth();
    }

    player.updatePlayerHealth(pl_hp);
    std::cout << "Player HP: " << pl_hp << std::endl;

    if (pl_hp <= 0) {
        //Player Died
        pl_dead = 1;
    }

    return pl_dead;
}

void Level::initMessages(){
    //WELCOME MSG
    this->messages.push_back("Welcome to the Game.");
}

std::vector<std::string> Level::getMessages(){
    return this->messages;
}

int Level::countAliveEnemies(){
    return this->enemies.size();
}

bool Level::getNextLevel(){
    return this->nextLevel;
}