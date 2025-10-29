#ifndef GAME_H
#define GAME_H

#include <vector>

#include "Board.h"
#include "Player.h"

class Game {
private:
    int numPlayers;
    
    Board board;
    std::vector<Player> players;

    int round = 0;
    int currentPlayerIndex = 0;

    int availableHouses = 32;
    int availableHotels = 12;
public:
    Game();

    // Constructor tools
    static int getNumPlayers();
    void addPlayer(std::string name);
    void testAddPlayer(std::string name);

    // Game loop
    void startGame();

    // Helpers
    int rollDice();
    void showPlayers() const;
    void showBoard() const;
    int getAvailableBuildings(bool houses) const;
    void modifyAvailableBuildings(bool house, int count);
};

#endif
