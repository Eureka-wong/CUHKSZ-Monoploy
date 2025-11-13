#ifndef GAME_H
#define GAME_H

#include <vector>

#include "Board.h"
#include "Player.h"

class Game {
private:
    int numPlayers;
    int bankruptcyCount = 0;

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
    void gameTurn(Player& currentPlayer);
    void normalTurn(Player& currentPlayers, int rolled_twelve);
    void jailTurn(Player& currentPlayers, int jailStatus);
    void rollToGetOutofJail(Player& currentPlayer);
    void payToGetOutofJail(Player& currentPlayer);
    void endTurn(Player& currentPlayer);
    void endGame(Player* currentPlayer = nullptr);

    // Helpers
    int rollDice();
    void showPlayers() const;
    void showBoard() const;
    int getAvailableBuildings(bool houses) const;
    void modifyAvailableBuildings(bool house, int count);
    static char getChoice(char a, char b, char c = '\0', char d = '\0');
    void addBankruptcyCount();
};

#endif
