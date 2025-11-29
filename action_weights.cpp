#include "action_weights.h"

Weights::Weights(){
    sellWeights = {0.1, 0.4, 0.2, 0.3};
    mortgageWeights = {0.1, 0.4, 0.2, 0.3};
    upgradeWeights = {0.2, 0.3, 0.3, 0.2};
    buyWeights = {0.2, 0.3, 0.3, 0.2};
}

void Weights::setWeights(const std::vector<double>& sell, 
                         const std::vector<double>& mortgage, 
                         const std::vector<double>& upgrade, 
                         const std::vector<double>& buy) {
    sellWeights = sell;
    mortgageWeights = mortgage;
    upgradeWeights = upgrade;
    buyWeights = buy;
}

std::vector<double> Weights::getSellWeights() const {
    return sellWeights;
}

std::vector<double> Weights::getMortgageWeights() const {
    return mortgageWeights;
}

std::vector<double> Weights::getUpgradeWeights() const {
    return upgradeWeights;
}

std::vector<double> Weights::getBuyWeights() const {
    return buyWeights;
}