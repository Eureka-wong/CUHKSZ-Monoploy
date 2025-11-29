#include "strategy.h"
#include "../src/Player.h"
#include "hint.h"
#include <iostream>
#include <random>
using namespace std;

possibleActions* BalancedStrategy::chooseAction(const Player& player, Hint& hint){
    hint.getHintResult(player);
    playerstate* state = hint.playerGameinfo(player);
    vector<possibleActions*> actions = hint.getActions(player, state);
    vector<possibleActions*> scored_actions = hint.scoreActions(actions, state);

    possibleActions* best_action = nullptr;
    int best_score = -9999;
    for (auto action : scored_actions){
        // pass buy actions
        if (action->action == "buy"){
            continue;
        }
        if (action->score > best_score){
            best_score = action->score;
            best_action = action;
        }
    }

    return best_action;
};

possibleActions* RandomStrategy::chooseAction(const Player& player, Hint& hint){
    playerstate* state = hint.playerGameinfo(player);
    vector<possibleActions*> actions = hint.getActions(player, state);

    if (actions.empty()){
        return nullptr;
    }

    // select a random action
    srand(time(0));
    int random_index = rand() % actions.size();
    return actions[random_index];
};