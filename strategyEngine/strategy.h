#pragma once
#include "../src/Player.h"
#include "hint.h"
#include "action_weights.h"

class Strategy{
public:
    virtual ~Strategy() =default;

    virtual possibleActions* chooseAction(const Player& player, Hint& hint) =0;

    virtual const char* getName() const =0;

    
};

class BalancedStrategy : public Strategy{
public:
    const char* getName() const override{ return "BalancedStrategy"; }
    possibleActions* chooseAction(const Player& player, Hint& hint) override;
    
};

class RandomStrategy : public Strategy{
public:
    const char* getName() const override{ return "RandomStrategy"; }
    possibleActions* chooseAction(const Player& player, Hint& hint) override;
};
