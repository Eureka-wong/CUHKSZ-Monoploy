#ifndef HINT_AI_H
#define HINT_AI_H

#include "../src/Board.h"
#include "../src/Player.h"
#include "../src/Game.h"
#include "../src/Tile.h"
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


class Hint{
private:
    // playerstate* Hint::playerGameinfo(const Player& player) const;
    std::vector<possibleActions*> actions;
    Weights actionWeights;
    Game* game;

public:
    Hint (Game* g);
    playerstate* playerGameinfo(const Player& player) const;
    std::vector<possibleActions*> getActions(const Player& player, playerstate* state);
    Weights& getActionWeights(){return actionWeights;}
    std::vector<possibleActions*> scoreActions(std::vector<possibleActions*> actions, playerstate* state);
    int scoreSellAction(const possibleActions& action, playerstate* state);
    int scoreMortgageAction(const possibleActions& action, playerstate* state);
    int scoreUpgradeAction(const possibleActions& action, playerstate* state);
    int scoreBuyAction(const possibleActions& action, playerstate* state);
    void getHintResult(const Player& player);
};

#endif