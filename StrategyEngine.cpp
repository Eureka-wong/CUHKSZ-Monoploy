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

vector<MonopolyStatus> StrategyEngine::opponentReminder(const Player& currentPlayer){
    const std::vector<Player>& playerlist = game->getplayerlist();
    std::vector<MonopolyStatus> monopolyList;
    playerstate* currentPlayerinfo = playerGameinfo(currentPlayer);

    for (const Player& player : playerlist){
        if (player.getName() == currentPlayer.getName())
            continue;

        playerstate* info = playerGameinfo(player);

        std::unordered_map<std::string, std::vector<PropertyTile*>> groupMap;
        for (PropertyTile* prop : info->playerprops){
            groupMap[prop->getGroup()].push_back(prop);
        }

        for (auto& entry : groupMap){
            const std::string& group = entry.first;
            const auto& ownedProps = entry.second;

            int owned = ownedProps.size();
            int groupSize = (group == "Brown" || group == "Dark Blue") ? 2 : 3;

            if (groupSize == 0) continue;

            if (owned == groupSize || owned == groupSize - 1){
                MonopolyStatus status;
                status.player = &player;
                status.group = group;
                status.owned = owned;
                status.groupSize = groupSize;
                status.hasMonopoly = (owned == groupSize);

                if (!status.hasMonopoly){
                    std::vector<PropertyTile*> missing;
                    auto groupTiles = game->getPropertiesByGroup(group);

                    for (auto* tile : groupTiles){
                        if (!tile->ownedByPlayer(player)){
                            missing.push_back(tile);
                        }
                    }

                    status.missingProperties = missing;
                }

                monopolyList.push_back(status);
            }
        }
    }

    return monopolyList;
}

vector<MonopolyStatus> StrategyEngine::playerMonopolyReminder(const Player& currentPlayer){
    std::vector<MonopolyStatus> monopolyList;
    playerstate* info = playerGameinfo(currentPlayer);

    std::unordered_map<std::string, std::vector<PropertyTile*>> groupMap;
    for (auto* prop : info->playerprops){
        groupMap[prop->getGroup()].push_back(prop);
    }

    for (auto& entry : groupMap){
        const string& group = entry.first;
        const auto& ownedProps = entry.second;

        int owned = ownedProps.size();
        int groupSize = (group == "Brown" || group == "Dark Blue") ? 2 : 3;

        if (groupSize == 0) continue;

        if (owned == groupSize || owned == groupSize - 1){
            MonopolyStatus status;
            status.player = &currentPlayer;
            status.group = group;
            status.owned = owned;
            status.groupSize = groupSize;
            status.hasMonopoly = (owned == groupSize);

            if (!status.hasMonopoly){
                std::vector<PropertyTile*> missing;
                auto groupTiles = game->getPropertiesByGroup(group);

                for (auto* tile : groupTiles){
                    if (!tile->ownedByPlayer(currentPlayer)){
                        missing.push_back(tile);
                    }
                }

                status.missingProperties = missing;
            }

            monopolyList.push_back(status);
        }
    }

    return monopolyList;
}

QString formatMissingProperties(const std::vector<PropertyTile*>& missing){
    if (missing.empty()) return "None";
    QStringList list;
    for (auto* tile : missing){
        list << QString::fromStdString(tile->getName());
    }
    return list.join(", ");
}

QString scoreToHint(int score){
    if (score >= 80) return "Highly recommended";
    if (score >= 60) return "Recommended";
    if (score >= 40) return "Consider it carefully";
    if (score >= 20) return "Risky";
    return "Not recommended";
}

QString getTradeRecommendation(const std::vector<PropertyTile*>& missing){
    QStringList output;
    if (missing.empty()) output << "None\n";

    for (auto* tile : missing){
        Player* owner = tile->getOwner();

        QString tileName = QString::fromStdString(tile->getName());
        QString group = QString::fromStdString(tile->getGroup());

        if (!owner){
            output << QString("%1 (unowned). Consider purchasing it to complete Monopoly on %2!")
            .arg(tileName).arg(group);
        } else {
            QString ownerName = QString::fromStdString(owner->getName());
            output << QString("Consider trading %1 with %2 to complete Monopoly on %3!")
                          .arg(tileName).arg(ownerName).arg(group);
        }
    }

    return output.join("\n");
}

QString DenyGetTradeRecommendation(const std::vector<PropertyTile*>& missing, QString opponent){
    QStringList output;
    if (missing.empty()) output << "None\n";

    for (auto* tile : missing){
        Player* owner = tile->getOwner();

        QString tileName = QString::fromStdString(tile->getName());
        QString group = QString::fromStdString(tile->getGroup());

        if (!owner){
            output << QString("%1 (unowned). Consider buying it to stop %2 from completing a Monopoly on %3!")
            .arg(tileName).arg(opponent).arg(group);
        } else {
            QString ownerName = QString::fromStdString(owner->getName());
            output << QString("Watch out for possible trade: %1 → %2. %3 may complete a Monopoly on %4!")
                          .arg(tileName).arg(ownerName).arg(opponent).arg(group);
        }
    }

    return output.join("\n");
}
/*
QString StrategyEngine::getHintResultforQt(const Player& player){
    QString results;
    playerstate* playerinfo = playerGameinfo(player);
    std::vector<possibleActions*> actions = getActions(player, playerinfo);
    std::vector<possibleActions*> action_with_socres = scoreActions(actions, playerinfo);

    results += "Property Purchase Suggestions\n";
    for (auto item : action_with_socres){
        if (item->action == "buy"){
            if (item->property == nullptr) continue;

            PropertyTile* property = item->property;
            int idx = property->getIndex();
            int current_idx = player.getPosition();
            int step = (idx - current_idx + 40) % 40;
            QString buyResults = scoreToHint(item->score);

            results += QString("• Roll %1 steps : Buy %2 → %3\n")
                           .arg(step)
                           .arg(QString::fromStdString(property->getName()))
                           .arg(buyResults);

        }
    }

    results += "\nBest Alternative Actions\n";
    std::vector<possibleActions*> nonBuy;
    for (auto* item : action_with_socres) {
        if (item->action != "buy")
            nonBuy.push_back(item);
    }

    if (nonBuy.empty()){
        results += "\n No property to manage \n";

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

        results += QString("• %1 on %2→ %3\n")
                       .arg(actionName)
                       .arg(propName)
                       .arg(hint);
    }

    return results;
}
*/
QString StrategyEngine::getHintResultforQt(const Player& player){
    QString results;

    // ======= Section Title Template =======
    auto sectionTitle = [&](QString title){
        results += QString(
                       "<p style='font-size:16px; font-weight:bold; color:#7EC8FF; "
                       "margin-top:10px;'>%1</p>").arg(title);
    };

    auto bullet = [&](QString text){
        results += QString("<p style='margin-left:15px; color:#FFFFFF;'>• %1</p>").arg(text);
    };

    auto subBullet = [&](QString text){
        results += QString("<p style='margin-left:35px; color:#CCCCCC;'>%1</p>").arg(text);
    };


    sectionTitle("Property Purchase Suggestions");

    playerstate* playerinfo = playerGameinfo(player);
    auto actions = getActions(player, playerinfo);
    auto scored = scoreActions(actions, playerinfo);

    for (auto item : scored){
        if (item->action == "buy" && item->property != nullptr){

            int idx = item->property->getIndex();
            int currentIdx = player.getPosition();
            int step = (idx - currentIdx + 40) % 40;

            QString propName = QString::fromStdString(item->property->getName());
            QString hint = scoreToHint(item->score);

            QString color =
                (item->score >= 80 ? "#00FF7F" :
                     item->score >= 60 ? "#32CD32" :
                     item->score >= 40 ? "#FFB84D" :
                     "#FF4040");

            bullet(QString("<b>Roll %1</b>: Buy <b>%2</b> "
                           "<span style='color:%3;'>→ %4</span>")
                       .arg(step).arg(propName).arg(color).arg(hint));
        }
    }

    sectionTitle("Best Alternative Actions");

    std::vector<possibleActions*> nonBuy;
    for (auto item : scored){
        if (item->action != "buy") nonBuy.push_back(item);
    }

    if (nonBuy.empty()){
        bullet("<span style='color:#888888;'>No property to manage.</span>");
    } else {
        std::sort(nonBuy.begin(), nonBuy.end(), [](auto* a, auto* b){
            return a->score > b->score;
        });

        int limit = std::min(2, (int)nonBuy.size());
        bool allRisky = true;
        for (int i = 0; i < limit; i++){
            if (nonBuy[i]->score > 20){
                allRisky = false;
                break;
            }
        }

        if (allRisky){
            subBullet("<span style='font-size:12px; color:#999999;'>"
                      "All available alternative actions are not recommended, but you may still consider them strategically."
                      "</span>");

            for (int i = 0; i < limit; i++){
                auto* item = nonBuy[i];

                QString actionName = QString::fromStdString(item->action);
                QString propName = item->property ?
                                       QString::fromStdString(item->property->getName()) :
                                       "N/A";
                QString hint = scoreToHint(item->score);

                QString color =
                    (item->score >= 80 ? "#00FF7F" :
                         item->score >= 60 ? "#32CD32" :
                         item->score >= 40 ? "#FFB84D" :
                         "#FF4040");

                bullet(QString("<b>%1 %2</b> → <span style='color:%3;'>%4</span>")
                           .arg(actionName)
                           .arg(propName)
                           .arg(color)
                           .arg(hint));
            }
        }
        else{
            for (int i = 0; i < limit; i++){
                auto* item = nonBuy[i];
                if (item->score <= 20) continue;

                QString actionName = QString::fromStdString(item->action);
                QString propName = item->property ?
                                       QString::fromStdString(item->property->getName()) :
                                       "N/A";
                QString hint = scoreToHint(item->score);

                QString color =
                    (item->score >= 80 ? "#00FF7F" :
                         item->score >= 60 ? "#32CD32" :
                         item->score >= 40 ? "#FFB84D" :
                         "#FF4040");

                bullet(QString("<b>%1 %2</b> → <span style='color:%3;'>%4</span>")
                           .arg(actionName)
                           .arg(propName)
                           .arg(color)
                           .arg(hint));
            }
        }
    }

    sectionTitle("Opponent Monopoly Threats");

    auto opponents = opponentReminder(player);

    if (opponents.empty()) {
        bullet("<span style='color:#888888;'>No opponent is close to forming a Monopoly.</span>");
    } else {
        for (auto& item : opponents){

            QString opponent = QString::fromStdString(item.player->getName());
            QString group = QString::fromStdString(item.group);

            if (item.hasMonopoly){
                bullet(QString("<span style='color:#FF4040; font-weight:bold;'>"
                               "Player %1 already owns a Monopoly on %2!</span>")
                           .arg(opponent).arg(group));
            }
            else {
                QString missing = formatMissingProperties(item.missingProperties);
                QString deny = DenyGetTradeRecommendation(item.missingProperties, opponent);

                bullet(QString("<span style='color:#FFB84D; font-weight:bold;'>"
                               "Player %1 is one step away from a Monopoly on %2.</span>")
                           .arg(opponent).arg(group));

                subBullet(QString("Missing property: <b>%1</b>").arg(missing));
                subBullet(QString("<span style='color:#CCCCCC;'><i>%1</i></span>").arg(deny));
            }
        }
    }

    sectionTitle("Your Monopoly Status");

    auto mine = playerMonopolyReminder(player);

    if (mine.empty()) {
        bullet("<span style='color:#888888;'>You are not close to any Monopoly.</span>");
    } else {
        for (auto& item : mine){
            QString group = QString::fromStdString(item.group);

            if (item.hasMonopoly){
                bullet(QString("<span style='color:#00FF7F; font-weight:bold;'>"
                               "You already own a Monopoly on %1.</span>").arg(group));
            } else {
                QString trade = getTradeRecommendation(item.missingProperties);

                bullet(QString("<b>You are close to a Monopoly on %1.</b>").arg(group));
                subBullet(QString("<span style='color:#7EC8FF;'>Trading suggestion:</span> %1")
                              .arg(trade));
            }
        }
    }

    return results;
}

