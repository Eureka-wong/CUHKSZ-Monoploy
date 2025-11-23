#ifndef HINT_TESTER_H
#define HINT_TESTER_H

#include "../src/Game.h"
#include "../src/Board.h"
#include "hint.h"
#include <iostream>

class HintTester{

private:
    int numPlayers;
    
    Board board;
    std::vector<Player> players;

    int round = 0;
    int currentPlayerIndex = 0;

    int availableHouses = 32;
    int availableHotels = 12;

public:
    HintTester();


};

#endif