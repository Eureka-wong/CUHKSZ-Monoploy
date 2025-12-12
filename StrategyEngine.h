#ifndef STRATEGYENGINE_H
#define STRATEGYENGINE_H
#include "Player.h"
#include "Game.h"
#include "Tile.h"
#include "action_weights.h"

using namespace std;

class Player;
class Board;


struct possibleActions{
    std::string action = "none"; // what to do with the property
    PropertyTile* property = nullptr; // information of the property
    int score = 0;
};

struct playerstate{
    int playerPos;
    int playerCash = 0;
    std::vector<PropertyTile*> playerprops;
};

struct MonopolyStatus {
    const Player* player;
    std::string group;
    int owned;
    int groupSize;
    bool hasMonopoly;
    std::vector<PropertyTile*> missingProperties;
};

class StrategyEngine{
private:
    // playerstate* Hint::playerGameinfo(const Player& player) const;
    std::vector<possibleActions*> actions;
    Weights actionWeights;
    Game* game;

public:
    StrategyEngine (Game* g);
    playerstate* playerGameinfo(const Player& player) const;
    std::vector<possibleActions*> getActions(const Player& player, playerstate* state);
    Weights& getActionWeights(){return actionWeights;}
    std::vector<possibleActions*> scoreActions(std::vector<possibleActions*> actions, playerstate* state);
    int scoreSellAction(const possibleActions& action, playerstate* state);
    int scoreMortgageAction(const possibleActions& action, playerstate* state);
    int scoreUpgradeAction(const possibleActions& action, playerstate* state);
    int scoreBuyAction(const possibleActions& action, playerstate* state);
    vector<MonopolyStatus> opponentReminder(const Player& currentPlayer);
    vector<MonopolyStatus> playerMonopolyReminder(const Player& currentPlayer);
    void getHintResult(const Player& player);
    QString getHintResultforQt(const Player& player);
};

#endif // STRATEGYENGINE_H
