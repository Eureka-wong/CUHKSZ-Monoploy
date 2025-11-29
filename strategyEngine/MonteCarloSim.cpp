#include "MonteCarloSim.h"
#include "hint.h"
#include "../src/Game.h"

void MonteCarloSim::run(){
    const int numPlayers = strategies.size();
    std::vector<StrategyPerformance> performances(numPlayers);

    for (int i = 0l; i < numPlayers; ++i){
        performances[i].strategyName = strategies[i]->getName();
    }

    for (int i = 0; i < numSimulations; ++i){
        GameResult result = runSingleSimulation();   
        int winnerID = result.winnerID;
        performances[winnerID].wins += 1;

        for (int j = 0; j < numPlayers; ++j){
            performances[j].gamesPlayed += 1;
        }
        // Process the result as needed
    }
}

void printResults() const{
    
}