#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "GameSystem.h"
#include <QueryBuilder.h>
#include <Connection.h>

#define FRAMERATE_LIMIT 60
#define statsPosXText 350

//GLOBAL
int levelCounter(1);

//using namespace database;


int main(int argc, char **argv) {


    //GET CWD FOR EXECUTEABLE
    std::string cwd = argv[0];
    cwd = cwd.substr(0, cwd.find_last_of("\\/") + 1);


    //DB
    database::Connection *qb;
    database::QueryResult *result;
    database::Entry *en;
    std::map<std::string, std::string> queryData;

    qb = new database::Connection(database::Connection::MYSQL, "127.0.0.1", "roguelike", "root", "julian1");

    if(!qb->Connect()){
        std::cout << "Cannot establish connection" << std::endl;
    }
    else{
        //qb->createQueryBuilder()->Select();
        std::cout << "DB Connection established" << std::endl;
    }


    result = qb->createQueryBuilder()
            ->Select("*")
            ->From("highscore")
            ->Execute();

    while(!result->eof()){
        en = result->GetEntry();
        queryData.insert(
                std::pair<std::string,std::string>(
                        en->GetStringField("name"),en->GetStringField("points")
                )
        );

        result->nextRow();
    }


    for(auto i : queryData){
        std::cout << i.first << ">" << i.second << std::endl;
    }



    //(878, 585) is std window resolution

    // Declare and load a font
    sf::Font font;
    font.loadFromFile(cwd + "mono.ttf");

    bool nameWindowClosed = false;

    /*Enter Name Window*/

    //USER INPUT INFO
    sf::Text usernameInfo("ENTER USERNAME", font);
    usernameInfo.setCharacterSize(25);
    usernameInfo.setStyle(sf::Text::Regular);
    usernameInfo.setFillColor(sf::Color::Green);
    usernameInfo.setPosition(statsPosXText, 320);

    //USER INPUT DISPLAY
    sf::Text usernameDisplay("", font);
    usernameDisplay.setCharacterSize(20);
    usernameDisplay.setStyle(sf::Text::Bold);
    usernameDisplay.setFillColor(sf::Color::Green);
    usernameDisplay.setPosition(statsPosXText, 350);


    //Storing User Input
    std::string username;

    sf::RenderWindow nameWindow(sf::VideoMode(878, 700), "Roguelike", sf::Style::Default);
    nameWindow.setPosition(sf::Vector2i(250, 50));
    nameWindow.setFramerateLimit(FRAMERATE_LIMIT);

    while (nameWindow.isOpen()) {
        sf::Event event;
        while (nameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                //nameWindowClosed = true;
                nameWindow.close();
            }

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    std::string name = usernameDisplay.getString();
                    if (event.text.unicode == 13) // return key
                    {
                        // finished entering name
                    } else if (event.text.unicode == 8) { // backspace
                        if (name.size() > 0) name.resize(name.size() - 1);
                    } else {
                        name += static_cast<char>(event.text.unicode);
                    }
                    usernameDisplay.setString(name);
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Escape:
                        //nameWindowClosed = true;
                        nameWindow.close();
                        break;
                    case sf::Keyboard::Return:
                        nameWindowClosed = true;
                        username = usernameDisplay.getString();
                        nameWindow.close();
                    default:
                        break;
                }
            }
        }

        nameWindow.clear();
        nameWindow.draw(usernameDisplay);
        nameWindow.draw(usernameInfo);
        nameWindow.display();
    }   //nameWindow

    /*GAME WINDOW*/

    if (nameWindowClosed) {
        sf::RenderWindow window(sf::VideoMode(878, 700), "Roguelike", sf::Style::Default);
        window.setPosition(sf::Vector2i(250, 50));
        window.setFramerateLimit(FRAMERATE_LIMIT);
        //window.setMouseCursorVisible(false);
        GameSystem gameSystem(cwd + "levels/level1.txt", username);
        std::string level = gameSystem.getLevel();

        /* Variable Setup */
        char input;

        //Window Handler Variables
        bool gameEnded = false;
        bool gameRestart = false;
        bool fullScreened = false;

        //Output
        std::vector<std::string> output;


        /* LVL 1 */
        sf::Text level1(level, font);
        level1.setCharacterSize(20);
        level1.setStyle(sf::Text::Regular);

        /* DEAD SCREEN */
        sf::Text deadMessage("", font);
        deadMessage.setCharacterSize(20);
        deadMessage.setStyle(sf::Text::Regular);
        deadMessage.setPosition(195, 320);


        /* Highscore */
        sf::Text highscoreMessage("", font);
        highscoreMessage.setCharacterSize(20);
        highscoreMessage.setStyle(sf::Text::Regular);
        highscoreMessage.setPosition(240, 340);


        /* PLAYER INFO */
        // 1-HEALTH, 2-AD, 3-EXP, 4-GOLD, 5-NAME

        //HEALTH
        sf::Text playerHealth("HEALTH: ", font);
        playerHealth.setCharacterSize(15);
        playerHealth.setStyle(sf::Text::Regular);
        playerHealth.setFillColor(sf::Color::Red);

        auto bounds = playerHealth.getLocalBounds();
        auto pos = playerHealth.getPosition();

        sf::RectangleShape rect(sf::Vector2f(878, 105));
        rect.setPosition(0.f, 590);
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(sf::Color::White);
        rect.setOutlineThickness(3.f);
        rect.setOrigin(playerHealth.getOrigin());

        //MAKE CENTER BOTTOM LEFT
        playerHealth.setOrigin(0.f, bounds.top + bounds.height);
        playerHealth.setPosition(statsPosXText, 610);

        //HEALTH VALUE
        sf::Text playerHealthValue("", font);
        playerHealthValue.setCharacterSize(15);
        playerHealthValue.setStyle(sf::Text::Bold);
        playerHealthValue.setFillColor(sf::Color::Red);
        playerHealthValue.setOrigin(0.f, bounds.top + bounds.height);
        playerHealthValue.setPosition(statsPosXText + 60, 610);

        //LEVEL DISPLAY
        sf::Text levelDisplay("LEVEL: ", font);
        levelDisplay.setCharacterSize(15);
        levelDisplay.setStyle(sf::Text::Regular);
        levelDisplay.setFillColor(sf::Color::Green);
        levelDisplay.setOrigin(0.f, bounds.top + bounds.height);
        levelDisplay.setPosition(statsPosXText - 348, 610);

        //LEVEL VALUE DISPLAY
        sf::Text levelDisplayValue(std::to_string(::levelCounter), font);
        levelDisplayValue.setCharacterSize(15);
        levelDisplayValue.setStyle(sf::Text::Bold);
        levelDisplayValue.setFillColor(sf::Color::Green);
        levelDisplayValue.setOrigin(0.f, bounds.top + bounds.height);
        levelDisplayValue.setPosition(statsPosXText - 290, 610);

        //AD
        sf::Text playerAD("AD: ", font);
        playerAD.setCharacterSize(15);
        playerAD.setStyle(sf::Text::Regular);
        playerAD.setFillColor(sf::Color::Green);

        playerAD.setOrigin(0.f, bounds.top + bounds.height);
        playerAD.setPosition(statsPosXText, 625);

        //AD VALUE
        sf::Text playerADValue("", font);
        playerADValue.setCharacterSize(15);
        playerADValue.setStyle(sf::Text::Bold);
        playerADValue.setFillColor(sf::Color::Green);
        playerADValue.setOrigin(0.f, bounds.top + bounds.height);
        playerADValue.setPosition(statsPosXText + 60, 625);


        //EXP
        sf::Text playerEXP("EXP: ", font);
        playerEXP.setCharacterSize(15);
        playerEXP.setStyle(sf::Text::Regular);
        playerEXP.setFillColor(sf::Color::Green);

        playerEXP.setOrigin(0.f, bounds.top + bounds.height);
        playerEXP.setPosition(statsPosXText, 640);

        //EXP VALUE
        sf::Text playerEXPValue("", font);
        playerEXPValue.setCharacterSize(15);
        playerEXPValue.setStyle(sf::Text::Bold);
        playerEXPValue.setFillColor(sf::Color::Green);
        playerEXPValue.setOrigin(0.f, bounds.top + bounds.height);
        playerEXPValue.setPosition(statsPosXText + 60, 640);


        //GOLD
        sf::Text playerGold("GOLD: ", font);
        playerGold.setCharacterSize(15);
        playerGold.setStyle(sf::Text::Regular);
        playerGold.setFillColor(sf::Color::Green);

        playerGold.setOrigin(0.f, bounds.top + bounds.height);
        playerGold.setPosition(statsPosXText, 655);

        //GOLD VALUE
        sf::Text playerGoldValue("", font);
        playerGoldValue.setCharacterSize(15);
        playerGoldValue.setStyle(sf::Text::Bold);
        playerGoldValue.setFillColor(sf::Color::Green);
        playerGoldValue.setOrigin(0.f, bounds.top + bounds.height);
        playerGoldValue.setPosition(statsPosXText + 60, 655);

        //NAME
        sf::Text playerName("NAME: ", font);
        playerName.setCharacterSize(15);
        playerName.setStyle(sf::Text::Regular);
        playerName.setFillColor(sf::Color::Green);

        playerName.setOrigin(0.f, bounds.top + bounds.height);
        playerName.setPosition(statsPosXText, 670);

        //NAME VALUE
        sf::Text playerNameValue("", font);
        playerNameValue.setCharacterSize(15);
        playerNameValue.setStyle(sf::Text::Bold);
        playerNameValue.setFillColor(sf::Color::Green);
        playerNameValue.setOrigin(0.f, bounds.top + bounds.height);
        playerNameValue.setPosition(statsPosXText + 60, 670);


        std::map<int, std::string> playerStats = gameSystem.getPlayerStats();

        playerHealthValue.setString(playerStats.at(1));
        playerADValue.setString(playerStats.at(2));
        playerEXPValue.setString(playerStats.at(3));
        playerGoldValue.setString(playerStats.at(4));
        playerNameValue.setString(username);

        //ACTIONS
        sf::Text actions("ACTIONS", font);
        actions.setCharacterSize(15);
        actions.setStyle(sf::Text::Bold);
        actions.setFillColor(sf::Color::Green);
        actions.setOrigin(0.f, bounds.top + bounds.height);
        actions.setPosition(statsPosXText + 350, 610);

        //ACTIONS
        sf::Text actionsValue("", font);
        actionsValue.setCharacterSize(15);
        actionsValue.setStyle(sf::Text::Bold);
        actionsValue.setFillColor(sf::Color::Green);
        actionsValue.setOrigin(0.f, bounds.top + bounds.height);
        actionsValue.setPosition(statsPosXText + 300, 625);


        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                if (event.type == sf::Event::KeyPressed) {
                    char pressed = event.key.code;

                    switch (event.key.code) {
                        case sf::Keyboard::W:
                            std::cout << "W" << std::endl;
                            input = 'W';
                            break;
                        case sf::Keyboard::A:
                            std::cout << "A" << std::endl;
                            input = 'A';
                            break;
                        case sf::Keyboard::S:
                            std::cout << "S" << std::endl;
                            input = 'S';
                            break;
                        case sf::Keyboard::D:
                            std::cout << "D" << std::endl;
                            input = 'D';
                            break;
                        case sf::Keyboard::Escape:
                            window.close();
                            break;
                        case sf::Keyboard::R:
                            std::cout << "R" << std::endl;
                            gameSystem.clearGame();
                            gameEnded = false;
                            break;
                        case sf::Keyboard::F10:
                            if (!fullScreened) {
                                window.close();
                                window.create(sf::VideoMode(878, 700), "Roguelike", sf::Style::Fullscreen);
                                fullScreened = true;
                                break;
                            } else {
                                //Make it normal again
                                window.close();
                                window.create(sf::VideoMode(878, 700), "Roguelike", sf::Style::Default);
                            }

                    }

                    if (gameSystem.allEnemiesDead()) {
                        //SET ONE TILE TO '+' to make a door to next level..
                        gameSystem.makeDoor();
                    }

                    output = gameSystem.getActions();
                    //Display 3 LAST Elements and \n after every Message
                    std::string lastOutput;
                    int i;

                    for (i = 0; i < output.size(); i++) {
                        if (output[i] != "") {

                            if (i <= 3) lastOutput += output[i] + "\n";

                            if (i > 3) {
                                //SLICE everything first 4 elements in vector then reset i to 0
                                output.erase(output.begin(), output.begin() + 3);
                                i = 0;
                                lastOutput = output[i] + "\n";
                            }
                        }
                    }

                    std::string playerName = gameSystem.getPlayerStats().at(5);
                    std::string highscore;

                    //UPDATE MESSAGES
                    actionsValue.setString(lastOutput);

                    if (gameSystem.gameEnded()) {
                        std::cout << "Player died!" << std::endl;
                        level1.setString("");
                        deadMessage.setString("Game Ended. " + playerName +  " Died! Press R to Restart");




                        for(auto k : queryData){
                            highscore += k.first + " | " + k.second + "\n";
                        }

                        std::cout << highscore;


                        highscoreMessage.setString(highscore);
                        gameEnded = true;
                    } else {
                        gameSystem.playerMove(input);
                        level = gameSystem.getLevel();
                        level1.setString(level);
                    }

                    /*
                    * PLAYER INFO BOX
                    * playerHealthValue, playerADValue, playerEXPValue, playerGoldValue, playerNameValue
                    * 1                  2              3               4                5
                    * GETTING VALUES FROM MAP IN GameSystem::getPlayerStats
                    * AND UPDATE ON KEY STROKE
                    */

                    playerStats = gameSystem.getPlayerStats();


                    playerHealthValue.setString(playerStats.at(1));
                    playerADValue.setString(playerStats.at(2));
                    playerEXPValue.setString(playerStats.at(3));
                    playerGoldValue.setString(playerStats.at(4));
                    playerNameValue.setString(playerStats.at(5));

                    //gameSystem.getActions();

                    //CHECK FOR NEW LEVEL
                    if (gameSystem.getStartNextLevel()) {
                        ::levelCounter++;
                        gameSystem.nextLevel();
                        level = gameSystem.getLevel();
                        level1.setString(level);
                        levelDisplayValue.setString(std::to_string(::levelCounter));
                    }
                }   //event keyPressed
            }   //while polEvent

            if (!gameEnded) {
                window.clear();
                window.draw(level1);
                window.draw(rect);

                window.draw(levelDisplay);
                window.draw(levelDisplayValue);


                window.draw(playerHealth);
                window.draw(playerHealthValue);

                window.draw(playerAD);
                window.draw(playerADValue);

                window.draw(playerEXP);
                window.draw(playerEXPValue);

                window.draw(playerGold);
                window.draw(playerGoldValue);

                window.draw(playerName);
                window.draw(playerNameValue);

                window.draw(actions);
                window.draw(actionsValue);

                window.display();
            } else {
                window.clear();
                window.draw(deadMessage);
                window.draw(highscoreMessage);
                window.display();
            }

        }//While window
    }
    return 0;
}