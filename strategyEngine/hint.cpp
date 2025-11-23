#include "hint.h"
#include "../src/Board.h"
#include "../src/Player.h"
#include "../src/Game.h"
#include "../src/Tile.h"

#include <iostream>
#include <algorithm>

using namespace std;

// Only trigger Hint module during "manage properties"

Hint::Hint(Game* g){
    this->game = g;
}

playerstate* Hint::playerGameinfo(const Player& player) const{
    playerstate* info = new playerstate();  

    // get the position, cash and properties of the player
    info->playerPos = player.getPosition();
    info->playerCash = player.getCash();
    info->playerprops = player.getProperties();  

    return info;  
}

vector<possibleActions*> Hint::getActions(const Player& player){
// list all the possible actions
    vector<possibleActions*> player_actions;
    playerstate* state = playerGameinfo(player);
    Board board;

    int playerCash = state->playerCash;
    int playerPos = state->playerPos;
    vector<PropertyTile*> playerProps = state->playerprops;

    // check the tile the player is currently on and offer a "buy" if it's an unowned PropertyTile
    
    if (playerCash > 0){
        Tile& currentTile = board.getTile(playerPos);
        for (size_t i = 1 ; i < 7; ++i){
            PropertyTile* possibleprop = dynamic_cast<PropertyTile*>(&currentTile+i);
            if (possibleprop && possibleprop->getOwner() == nullptr){
                possibleActions* action = new possibleActions();
                action->action = "buy";
                action->property = possibleprop;
                player_actions.push_back(action);
            }
        }
        
    }

    // iterate owned properties to collect sell/mortgage/unmortgage/upgrade actions
    for (PropertyTile* property : playerProps){ 

        // add "sell" to possible action list if the player has some properties
        if (!playerProps.empty()){
            possibleActions* action = new possibleActions();
            action->action = "sell";
            action->property = property;
            player_actions.push_back(action);
        }
        
        // add "mortgage" to possible action if some property is undeveloped
        if (property->getHouses() == 0){
            possibleActions* action = new possibleActions();
            action->action = "mortgage";
            action->property = property;
            player_actions.push_back(action);
        }

        if (property->isMortgaged()){
            possibleActions* action = new possibleActions();
            action->action = "unmortgage";
            action->property = property;
            player_actions.push_back(action);

        }

        bool ownColorGroup = property->ownColorGroup();
        bool isStationOrUtility = property->isStationOrUtility();
        int houseprice = property->getHousePrice();

        if (ownColorGroup && !isStationOrUtility && playerCash > houseprice){
            possibleActions* action = new possibleActions();
            action->action = "upgrade";
            action->property = property;
            player_actions.push_back(action);
        }

    }

    return player_actions;
    
}

vector<possibleActions*> Hint::scoreActions(std::vector<possibleActions*> actions, playerstate* state){
// update the score each actions
    for (possibleActions* action : actions){
        int actionScore = 0;

        if (action->action=="sell"){
            actionScore = scoreSellAction(*action, state);
            action->score = actionScore;
            cout << "[DEBUG] Scored 'sell': " << actionScore << endl;
        };

        if (action->action=="mortgage"){
            actionScore = scoreMortgageAction(*action, state);
            action->score = actionScore;
            cout << "[DEBUG] Scored 'mortgage': " << actionScore << endl;
        };

        if (action->action=="upgrade"){
            actionScore = scoreUpgradeAction(*action, state);
            action->score = actionScore;
            cout << "[DEBUG] Scored 'upgrade': " << actionScore << endl;
        };

        if (action->action=="buy" || action->action=="unmortgage"){
            actionScore = scoreBuyAction(*action, state);
            action->score = actionScore;
            cout << "[DEBUG] Scored 'buy': " << actionScore << endl;
        };


    
    }

    return actions;

}


int Hint::scoreSellAction(const possibleActions& action, playerstate* state){
// Calculate score for "sell"   
    double score = 0;
    PropertyTile* property = action.property;
    int propPrice = property->getPrice();
    int propidx = property->getIndex();
    int propHouses = property->getHouses();
    int houseprice = property->getHousePrice();

    Weights& weights = getActionWeights();
    std::vector<double> sellWeights = weights.getSellWeights();

    // 1. calculate position score 
    double positionScore = 0;
    std::vector<int> special_loc_idx = {1, 3, 6, 11, 13, 14, 16}; 
        auto it = std::find(special_loc_idx.begin(), special_loc_idx.end(), propidx);
        if (it != special_loc_idx.end()) {
            positionScore = 0.8;
        } else {
            positionScore = 1.2;
        }
    score += positionScore * sellWeights[0];
    
    // 2. calculate cashScore
    double cashScore = (state->playerCash < 200) ? 1.0 : -1.0;
    score += cashScore * sellWeights[1];

    // 3. calculate monopolyScore
    double monopolyScore = property->ownColorGroup() ? 0.3 : 1.5;
    score += monopolyScore * sellWeights[2];
    
    // 4. calculate priceScore 
    double priceScore = 0;
    if (propHouses == 0){
        double priceScore = 0.8 * (propPrice / 100);
    }else{double priceScore = (houseprice / 100) / 2;}
    score += priceScore * sellWeights[3];

    return static_cast<int>(score * 100);
   
}

int Hint::scoreMortgageAction(const possibleActions& action, playerstate* state){
// Calculate score for "mortgage"  
    double score = 0;
    PropertyTile* property = action.property;
    int playerCash = state->playerCash;
    int propidx = property->getIndex();
    int propPrice = property->getPrice();

    Weights& weights = getActionWeights();
    std::vector<double> mortgageWeights = weights.getMortgageWeights();

    // 1. calculate position score 
    double positionScore = 0;
    std::vector<int> special_loc_idx = {1, 3, 6, 11, 13, 14, 16}; 
        auto it = std::find(special_loc_idx.begin(), special_loc_idx.end(), propidx);
        if (it != special_loc_idx.end()) {
            positionScore = 0.8;
        } else {
            positionScore = 1.2;
        }
    score += positionScore * mortgageWeights[0];

    // 2. calculate cashScore
    double cashScore = (state->playerCash < 200) ? 1.0 : -1.0;
    score += cashScore * mortgageWeights[1];

    // 3. calculate monopolyScore
    double monopolyScore = property->ownColorGroup() ? 0.3 : 1.5;
    score += monopolyScore * mortgageWeights[2];
    
    // 4. calculate priceScore 
    double priceScore = propPrice / 100;
    score += priceScore * mortgageWeights[3];
    
    return static_cast<int>(score * 100);
    
}

int Hint::scoreUpgradeAction(const possibleActions& action, playerstate* state){
// Calculate score for "upgrade"  
    double score = 0;
    PropertyTile* property = action.property;
    int playerCash = state->playerCash;
    int propidx = property->getIndex();
    int propPrice = property->getPrice();
    int step = 0;
    int rentDue = 0;
    property->calculateRent(step, rentDue);


    Weights& weights = getActionWeights();
    std::vector<double> upgradeWeights = weights.getUpgradeWeights();

    // 1. calculate position score 
    double positionScore = 0;
    std::vector<int> special_loc_idx = {1, 3, 6, 11, 13, 14, 16}; 
        auto it = std::find(special_loc_idx.begin(), special_loc_idx.end(), propidx);
        if (it != special_loc_idx.end()) {
            positionScore = 1.2;
        } else {
            positionScore = 0.8;
        }
    score += positionScore * upgradeWeights[0];

    // 2. calculate cashScore
    double cashScore = (state->playerCash - propPrice > 200) ? 1.0 : -1.0;
    score += cashScore * upgradeWeights[1];

    // 3. calculate monopolyScore
    double monopolyScore = property->ownColorGroup() ? 1.5 : 0.3;
    score += monopolyScore * upgradeWeights[2];

    // 4. calculate rentScore
    double rentScore = 0;
    int houses = property->getHouses();
    std::array<int, 6> rent = property->getRent();
    int afterRent;
    if (property->ownColorGroup()){
        int afterRent = rent[houses+1] * 2;
    }else{
        int afterRent = rent[houses+1];
    }
    int houseprice = property->getHousePrice();

    rentScore = (afterRent - rentDue) / houseprice;
    rentScore = std::min(2.0, std::max(0.0, rentScore));
    score += rentScore * upgradeWeights[3];
    
    return static_cast<int>(score * 100);

}

int Hint::scoreBuyAction(const possibleActions& action, playerstate* state){
// Calculate score for "buy"
    double score = 0;
    PropertyTile* property = action.property;
    int playerCash = state->playerCash;
    int propidx = state->playerPos;
    int propPrice = property->getPrice();

    Weights& weights = getActionWeights();
    std::vector<double> buyWeights = weights.getBuyWeights();

    
    // 1. calculate cashScore
    double cashScore = (playerCash - propPrice > 200) ? 1.0 : -1.0;
    score += cashScore * buyWeights[1];

    // 2. calculate position score 
    double positionScore = 0;
    std::vector<int> special_loc_idx = {1, 3, 6, 11, 13, 14, 16}; 
        auto it = std::find(special_loc_idx.begin(), special_loc_idx.end(), propidx);
        if (it != special_loc_idx.end()) {
            positionScore = 1.2;
        } else {
            positionScore = 0.8;
        }
    score += positionScore * buyWeights[0];

    // 3. calculate monopolyScore
    double monopolyScore = 0;
    int numOfPropInGroup = property->countOwnedPropertiesInGroup();
    string propGroup = property->getGroup();
    double monopolyPotential;
    if (propGroup == "Brown" || propGroup == "Dark Blue"){
        double monopolyPotential = numOfPropInGroup / 2;
    }else{double monopolyPotential = numOfPropInGroup / 3;}

    monopolyScore = (monopolyPotential == 1.0) ? 2.0 : monopolyPotential;

    score += monopolyScore * buyWeights[2];
    
    // 4. calculate rentScore 
    double rentScore = 0;
    std::array<int, 6> rent = property->getRent();
    int afterRent;
    if (propGroup == "Station" || propGroup == "Utility"){
        rentScore = 1.2;   
    }
    else{
        double roi = rent[0] / propPrice;
        rentScore = std::min(2.0, std::max(0.0, roi * 10.0));
    }
    score += rentScore * buyWeights[3];
 

    return static_cast<int>(score * 100);
}



void Hint::getHintResult(const Player& player){
// Give the best hint based on scores and give explanations
    playerstate* playerinfo = playerGameinfo(player);
    std::vector<possibleActions*> actions = getActions(player);
    std::vector<possibleActions*> action_with_socres = scoreActions(actions, playerinfo);
    string best_action;
    int best_score = 0;
    for (auto item : action_with_socres){
        int score = item->score;
        string action = item->action;
        if (score > best_score){
            best_score = score;
            best_action = action;
        }

    }

    cout << "best action is" << best_action << endl;
}

