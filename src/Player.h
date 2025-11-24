#ifndef Player_H
#define Player_H

#include <vector>
#include <string>
#include <algorithm>

#include "CardDeck.h"
class Game;
class PropertyTile;

class Player {
private:
    std::string name;
    int position;
    int balance = 0;
    int jailStatus = -1; // -1: not in jail, 0: first turn, 1: second turn, 2: third turn
    bool bankrupt = false;
    std::vector<Card*> getOutOfJailCard;
    int availableSE = 3; // Each player can use the strategy engine for at most 3 times

    std::vector<PropertyTile*> properties;
public:
    Player(const std::string& name, int position, int balance);

    // Accessors
    std::string getName() const;
    int getPosition() const;
    int getCash() const;
    std::vector<PropertyTile*> getProperties() const;
    int getJailStatus() const;
    int isBankrupt() const ;
    int getSE() const;

    // Helpers
    void showPlayer() const;
    void setPosition(int newPosition);

    void addMoney(int amount);
    void deductMoney(int amount);

    void addProperty(PropertyTile* property);
    void removeProperty(PropertyTile* property);
    void manageProperties(Game& game);

    void setJailStatus(int status);
    int calculateWealth() const;

    void forceRaiseMoney(Game& game, int amount);
    void declareBankruptcy(Game& game, Player* creditor = nullptr);

    void addGetOutOfJailCard(Card* card);
    bool hasGetOutOfJailCard() const;
    Card* takeOutGetOutOfJailCard();
    
    void getTradeDetails(int& amount, int& playerIndex, bool buyProperty = false);

    void deductSE(int SE);
};

#endif
