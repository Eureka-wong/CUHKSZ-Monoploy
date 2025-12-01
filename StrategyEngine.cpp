#include "StrategyEngine.h"
#include "Board.h"
#include "Player.h"
#include "Game.h"
#include "Tile.h"

#include <iostream>
#include <algorithm>

using namespace std;

// Only trigger Hint module during "manage properties"
StrategyEngine::StrategyEngine(Game* g){
    this->game = g;
}

playerstate* StrategyEngine::playerGameinfo(const Player& player) const{
    playerstate* info = new playerstate();

    // get the position, cash and properties of the player
    info->playerPos = player.getPosition();
    info->playerCash = player.getCash();
    info->playerprops = player.getProperties();
    // cout <<"[DEBUG] get player info" << info << endl;

    return info;
}

vector<possibleActions*> StrategyEngine::getActions(const Player& player, playerstate* state){
    // list all the possible actions
    vector<possibleActions*> player_actions;

    int playerCash = state->playerCash;
    int playerPos = state->playerPos;
    vector<PropertyTile*> playerProps = state->playerprops;
    Board& board = game->getBoard();


    // check the tile the player is currently on and offer a "buy" if it's an unowned PropertyTile

    if (playerCash > 0){
        Tile& currentTile = board.getTile(playerPos);
        for (size_t i = 1 ; i < 13; i++){
            int nextIndex = (playerPos + i) % 40;
            Tile& tile = board.getTile(nextIndex);
            if (auto* p = dynamic_cast<PropertyTile*>(&tile)){
                if (p->getOwner() == nullptr){
                    possibleActions* action = new possibleActions();
                    action->action = "buy";
                    action->property = p;
                    player_actions.push_back(action);
                    // cout << "[DEBUG] get action " << action->action << "for property " << action->property << endl;
                }
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
            // cout << "[DEBUG] get sell action" << endl;
        }

        // add "mortgage" to possible action if some property is undeveloped
        if (property->getHouses() == 0){
            possibleActions* action = new possibleActions();
            action->action = "mortgage";
            action->property = property;
            player_actions.push_back(action);
            // cout << "[DEBUG] get mortgage action" << endl;
        }

        if (property->isMortgaged()){
            possibleActions* action = new possibleActions();
            action->action = "unmortgage";
            action->property = property;
            player_actions.push_back(action);
            // cout << "[DEBUG] get unmortgage action" << endl;
        }

        bool ownColorGroup = property->ownColorGroup();
        bool isStationOrUtility = property->isStationOrUtility();
        int houseprice = property->getHousePrice();

        if (ownColorGroup && !isStationOrUtility && playerCash > houseprice){
            possibleActions* action = new possibleActions();
            action->action = "upgrade";
            action->property = property;
            player_actions.push_back(action);
            // cout << "[DEBUG] get upgrade action" << endl;
        }

    }

    return player_actions;

}

vector<possibleActions*> StrategyEngine::scoreActions(std::vector<possibleActions*> actions, playerstate* state){
    // update the score each actions
    for (possibleActions* action : actions){
        int actionScore = 0;

        if (action->action=="sell"){
            actionScore = scoreSellAction(*action, state);
            action->score = actionScore;
            // cout << "[DEBUG] Scored 'sell': " << actionScore << endl;
        };

        if (action->action=="mortgage"){
            actionScore = scoreMortgageAction(*action, state);
            action->score = actionScore;
            // cout << "[DEBUG] Scored 'mortgage': " << actionScore << endl;
        };

        if (action->action=="upgrade"){
            actionScore = scoreUpgradeAction(*action, state);
            action->score = actionScore;
            // cout << "[DEBUG] Scored 'upgrade': " << actionScore << endl;
        };

        if (action->action=="buy" || action->action=="unmortgage"){
            actionScore = scoreBuyAction(*action, state);
            action->score = actionScore;
        };

    }

    return actions;

}


int StrategyEngine::scoreSellAction(const possibleActions& action, playerstate* state){
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
        priceScore = 0.8 * (propPrice / 100);
    }else{priceScore = (houseprice / 100) / 2;}
    score += priceScore * sellWeights[3];

    return static_cast<int>(score * 100);

}

int StrategyEngine::scoreMortgageAction(const possibleActions& action, playerstate* state){
    // Calculate score for "mortgage"
    double score = 0;
    int playerCash = 0;
    int propidx = 0;
    int propPrice = 0;
    PropertyTile* property = action.property;
    playerCash = state->playerCash;
    propidx = property->getIndex();
    propPrice = property->getPrice();

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
    double cashScore = (playerCash < 200) ? 1.0 : -1.0;
    score += cashScore * mortgageWeights[1];

    // 3. calculate monopolyScore
    double monopolyScore = property->ownColorGroup() ? 0.3 : 1.5;
    score += monopolyScore * mortgageWeights[2];

    // 4. calculate priceScore
    double priceScore = propPrice / 100;
    score += priceScore * mortgageWeights[3];

    return static_cast<int>(score * 100);

}

int StrategyEngine::scoreUpgradeAction(const possibleActions& action, playerstate* state){
    // Calculate score for "upgrade"
    double score = 0;
    PropertyTile* property = action.property;
    int playerCash = state->playerCash;
    int propidx = property->getIndex();
    int propPrice = property->getPrice();
    int step = 0;
    int rentDue = 0;
    property->calculateRent(step);


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
    int afterRent = 0;
    if (property->ownColorGroup()){
        afterRent = rent[houses+1] * 2;
    }else{
        afterRent = rent[houses+1];
    }
    int houseprice = property->getHousePrice();

    rentScore = (afterRent - rentDue) / houseprice;
    rentScore = std::min(2.0, std::max(0.0, rentScore));
    score += rentScore * upgradeWeights[3];

    return static_cast<int>(score * 100);

}

int StrategyEngine::scoreBuyAction(const possibleActions& action, playerstate* state){
    // Calculate score for "buy"
    double score = 0;
    PropertyTile* property = action.property;
    int playerCash = state->playerCash;
    int propidx = property->getIndex();
    int propPrice = property->getPrice();

    Weights& weights = getActionWeights();
    std::vector<double> buyWeights = weights.getBuyWeights();

    if (property == nullptr) return 0;

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
    int numOfPropInGroup = 0;
    if (property->getOwner() != nullptr){
        numOfPropInGroup = property->countOwnedPropertiesInGroup();
    }

    string propGroup = property->getGroup();
    int monopolyPotential = 0;

    if (propGroup == "Brown" || propGroup == "Dark Blue"){
        monopolyPotential = numOfPropInGroup / 2.0;
    }else{monopolyPotential = numOfPropInGroup / 3.0;}


    monopolyScore = (monopolyPotential == 1.0) ? 2.0 : monopolyPotential;
    score += monopolyScore * buyWeights[2];
    cout << "monopolyScore is:" << monopolyScore << endl;

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

void StrategyEngine::getHintResult(const Player& player){
    // Give the best hint based on scores and give explanations
    playerstate* playerinfo = playerGameinfo(player);
    std::vector<possibleActions*> actions = getActions(player, playerinfo);
    std::vector<possibleActions*> action_with_socres = scoreActions(actions, playerinfo);

    // enumerate all possibilities for ''buy'' action
    std::vector<possibleActions*> buyActions;
    for (auto item : action_with_socres){
        if (item->action == "buy"){
            if (item->property == nullptr) continue;

            auto property = item->property;
            int idx = property->getIndex();
            int current_idx = player.getPosition();
            int step = (idx - current_idx + 40) % 40;
            cout << "Buy " << property->getName()
                 << " | score = " << item->score
                 << " | roll = " << step
                 << endl;
        }

    }
    string best_action = "none";
    int best_score = -999;
    PropertyTile* best_property = nullptr;
    string prop_name = "none";
    for (auto item : action_with_socres){
        if (item->action == "buy") continue;

        if (item->property == nullptr) continue;

        if (item->score > best_score){
            best_score = item->score;
            best_action = item->action;
            best_property = item->property;
            prop_name = best_property->getName();
        }


    }

    cout << "Best action other than 'buy' is: " << best_action << " " << prop_name << " with score " << best_score << endl;
}

QString scoreToHint(int score){
    if (score >= 80) return "Highly recommended";
    if (score >= 60) return "Recommended";
    if (score >= 40) return "Consider if no risk";
    if (score >= 20) return "Risky";
    return "Not recommended";
}
QString StrategyEngine::getHintResultforQt(const Player& player){
    QString results;
    playerstate* playerinfo = playerGameinfo(player);
    std::vector<possibleActions*> actions = getActions(player, playerinfo);
    std::vector<possibleActions*> action_with_socres = scoreActions(actions, playerinfo);

    results += "<b>Property Purchase Suggestions</b>\n";
    for (auto item : action_with_socres){
        if (item->action == "buy"){
            if (item->property == nullptr) continue;

            PropertyTile* property = item->property;
            int idx = property->getIndex();
            int current_idx = player.getPosition();
            int step = (idx - current_idx + 40) % 40;
            QString buyResults = scoreToHint(item->score);

            results += QString("• Roll %1 steps : Buy <b>%2</b> → <font color='blue'>%3</font>\n")
                           .arg(step)
                           .arg(QString::fromStdString(property->getName()))
                           .arg(buyResults);

        }
    }

    results += "\n<b>Best Alternative Actions</b>\n";
    std::vector<possibleActions*> nonBuy;
    for (auto* item : action_with_socres) {
        if (item->action != "buy")
            nonBuy.push_back(item);
    }

    std::sort(nonBuy.begin(), nonBuy.end(), [](auto* a, auto* b){
        return a->score > b->score;
    });

    int limit = std::min(2, (int)nonBuy.size());
    for (int i = 0; i < limit; i++) {
        auto* item = nonBuy[i];
        QString hint = scoreToHint(item->score);

        QString actionName = QString::fromStdString(item->action);
        QString propName = item->property ?
                               QString::fromStdString(item->property->getName()) :
                               "N/A";

        results += QString("• <b>%1</b> on <b>%2</b> → <font color='green'>%3</font>\n")
                       .arg(actionName)
                       .arg(propName)
                       .arg(hint);
    }

    return results;
}
