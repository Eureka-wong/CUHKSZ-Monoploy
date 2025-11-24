#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>

#include "Player.h"
#include "Board.h"
#include "CardDeck.h"

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

    CardDeck communityChestDeck;
    CardDeck chanceDeck;
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
    void movePlayer(Player& currentPlayer, int step);

    bool playerCanPay(Player& payer, int amount, Player* receiver = nullptr);
    void payToPlayers(Player& payer, int amount);
    void receiveFromPlayers(Player& receiver, int amount);

    int getAvailableBuildings(bool houses) const;
    void modifyAvailableBuildings(bool house, int count);

    void drawChanceCard(Player& player);
    void drawCommunityChestCard(Player& player);
    void useGetOutOfJailCard(Player& player);
    
    void manageSellTrade(Player& offeringPlayer, int propertyIndex);
    void manageBuyTrade(Player& offeringPlayer, int propertyIndex);
    bool validateTradePlayers(Player* offeringPlayer, Player* targetPlayer, int propertyIndex, bool offerBuy);
    void askTradeDecision(Player* offeringPlayer, Player* targetPlayer, int amount, int propertyIndex, PropertyTile* property, bool offerBuy);
    void executeTrade(Player* buyingPlayer, Player* sellingPlayer, int amount, int propertyIndex, PropertyTile* property);

    void showPlayers() const;
    void showBoard() const;
    static char getChoice(char a, char b, char c = '\0', char d = '\0', char e = '\0');
    std::vector<Player>& getPlayers(); 
    Board& getBoard();
};

#endif
