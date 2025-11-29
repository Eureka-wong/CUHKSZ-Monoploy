#pragma once
#include "../src/Player.h"
#include "../src/Game.h"
#include "../src/Tile.h"
#include "strategy.h"  

struct GameResult{
    int winnerID;
    int totalRounds;
    std::vector<int> finalCash;
    std::vector<bool> isBankrupt;
};

struct StrategyPerformance{
    string strategyName;
    int wins;
    int gamesPlayed;
};

class MonteCarloSim{
public:
    MonteCarloSim(
        int numSimulations,
        const std::vector<Strategy*>& strategies
    );

    void run();
    void printResults() const;

private:
    int numSimulations;
    std::vector<Strategy*> strategies;
    GameResult runSingleSimulation();

};
