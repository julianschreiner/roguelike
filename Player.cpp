//
// Created by Julian Schreiner on 12.12.17.
//

#include "Player.h"
#include <random>
#include <ctime>
#include <map>
#include <iostream>

Player::Player(){
    this->x = 0;
    this->y = 0;
}
void Player::init(std::string name, int level, int health, int attack, int defense, int experience, int gold, bool dead){
    this->name = name;
    this->level = level;
    this->health = health;
    this->playerAttack = attack;
    this->defense = defense;
    this->experience = experience;
    this->gold = gold;
    this->dead = dead;

}
void Player::setPosition(int x, int y){
    this->x = x;
    this->y = y;
}
void Player::getPosition(int &x, int &y){
    x = this->x;
    y = this->y;
}
int Player::getPlayerAttack() {
    return this->playerAttack;
}
int Player::getPlayerHealth(){
    return this->health;
}

int Player::getExperience(){
    return this->experience;
}

int Player::getDefense(){
    return this->defense;
}

void Player::setGold(int gold){
    this->gold += gold;
}
int Player::getGold(){
    return this->gold;
}

void Player::setDead(bool dead){

    if(dead){
        this->dead = true;
    }
    else{
        this->dead = false;
    }
}

bool Player::getDead(){
    return this->dead;
}

std::string Player::updateExperience(int exp){
    this->experience += exp;
    int goalss[] = {100,200,500,1000, 2500};
    std::string ret;
    /*
     * TYPEDEF FOR map<int,int>goals
     * [KEY]EXPERIENCEPOINTS | [VALUE] ATTACKDAMAGE
    */

    std::map<int, int> goals;
    //    XP     AD(BONUS)
    goals[100] = 50;
    goals[200] = 450;
    goals[500] = 600;
    goals[1000] = 800;
    goals[2500] = 10000;

    std::map<int, int>::iterator first_goal = goals.find(100);
    std::map<int, int>::iterator second_goal = goals.find(200);
    std::map<int, int>::iterator third_goal = goals.find(500);
    std::map<int, int>::iterator fourth_goal = goals.find(1000);
    std::map<int, int>::iterator fifth_goal = goals.find(2500);

    if(this->experience >= first_goal->first && this->experience < second_goal->first){        //100XP - 199XP
        this->level++;
        this->playerAttack += goals.at(100);
        ret =  "+ " + std::to_string(goals.at(100)) + " AD - LVL UP!";
    }
    else if(this->experience >= second_goal->first && this->experience < third_goal->first){   //200XP - 499XP
        this->level++;
        this->playerAttack += goals.at(200);
        ret =  "+ " + std::to_string(goals.at(200)) + " AD - LVL UP!";
    }
    else if(this->experience == third_goal->first && this->experience < fourth_goal->first){   //500XP - 999XP
        this->level++;
        this->playerAttack += goals.at(500);
        ret =  "+ " + std::to_string(goals.at(500)) + " AD - LVL UP!";
    }
    else if(this->experience == fourth_goal->first && this->experience < fifth_goal->first){   //1000XP - 2499XP
        this->level++;
        this->playerAttack += goals.at(1000);
        ret =  "+ " + std::to_string(goals.at(1000)) + " AD - LVL UP!";
    }
    else if(this->experience >= fifth_goal->first){                                  //2500XP to endless
        this->level++;
        this->playerAttack += goals.at(2500);
        ret =  "+ " + std::to_string(goals.at(2500)) + " AD - LVL UP!";
    }


    return ret;
}

void Player::updatePlayerHealth(int newHealth){
    this->health = newHealth;
}


std::string Player::getName(){
    return this->name;
}

void Player::resetHealth(){
    this->health = 100;
}

int Player::getLevel(){
    return this->level;
}