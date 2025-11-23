#pragma once
#include "hint.h"
#include <xgboost/c_api.h>
#include <vector>
#include <map>
#include <string>

class XGBoostModelManager {
private:
    BoosterHandle sell_model;
    BoosterHandle mortgage_model;
    BoosterHandle upgrade_model;
    BoosterHandle buy_model;
    bool models_loaded;

public:
    XGBoostModelManager();
    ~XGBoostModelManager();
    
    bool loadModels();
    float predict(const std::vector<float>& features, const std::string& action_type);
};

class FeatureExtractor {
public:
    static std::vector<float> extractSellFeatures(const possibleActions& action, playerstate* state);
    static std::vector<float> extractMortgageFeatures(const possibleActions& action, playerstate* state);
    static std::vector<float> extractUpgradeFeatures(const possibleActions& action, playerstate* state);
    static std::vector<float> extractBuyFeatures(const possibleActions& action, playerstate* state);
    
private:
    static float calculatePositionScore(int property_idx);
    static float calculateMonopolyScore(PropertyTile* property);
    static float calculateCashScore(int player_cash, int threshold = 200);
    static float calculateRentROI(PropertyTile* property);
};