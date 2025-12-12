#ifndef CARDDECK_H
#define CARDDECK_H

#include <string>
#include <deque>
#include <random>

class Game;
class Player;

enum class CardType {
    MOVE_TO,
    MOVE,
    MOVE_TO_NEAREST,
    COLLECT_MONEY,
    PAY_MONEY,
    COLLECT_FROM_PLAYERS,
    PAY_TO_PLAYERS,
    REPAIRS,
    GET_OUT_OF_JAIL,
    GO_TO_JAIL
};

struct Card {
    std::string description;
    CardType type;

    int amount;
    int targetTile;
    int step;
    std::string nearest;

    int houseCost;
    int hotelCost;

    std::string sourceDeck;

    Card(std::string description, CardType type, int amount = 0, int targetTile = -1, int step = 0,
         std::string nearest = "", int houseCost = 0, int hotelCost = 0, std::string sourceDeck = "");

    void execute(Player& player, Game& game);
};

class CardDeck {
private:
    std::deque<Card> deck;
    std::mt19937 rng{ std::random_device{}() };
public:
    CardDeck(std::string deckType); // Constructor

    Card draw();
    void returnCard(const Card& card);
};

#endif
