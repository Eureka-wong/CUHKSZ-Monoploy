#include <iostream>
#include <deque>
#include <vector>

#include "CardDeck.h"
#include "Game.h"
#include "Player.h"

using namespace std;

Card::Card(string description, CardType type, int amount, int targetTile, int step,
           string nearest, int houseCost, int hotelCost, string sourceDeck) :
    description(std::move(description)), type(type), amount(amount), targetTile(targetTile),
    step(step), nearest(std::move(nearest)), houseCost(houseCost), hotelCost(hotelCost),
    sourceDeck(sourceDeck) {}

void Card::execute(Player& player, Game& game) {
    // Implementation of card effects goes here
    cout << player.getName() << " drew a card: " << description << endl << endl;
    switch (type) {
    case CardType::MOVE_TO: {
        int step = 0;
        if (targetTile >= player.getPosition()) {
            step = targetTile - player.getPosition();
        } else {
            step = 40 - player.getPosition() + targetTile;
        }
        game.movePlayer(game.getCurrentPlayerIndex(), step);
        Tile& currentTile = game.getBoard().getTile(player.getPosition());
        currentTile.onLand(player,game,step);
        break;
    }

    case CardType::MOVE: {
        game.movePlayer(game.getCurrentPlayerIndex(), step);
        Tile& currentTile = game.getBoard().getTile(player.getPosition());
        currentTile.onLand(player,game,step);
        break;
    }

    case CardType::MOVE_TO_NEAREST: {
        if (nearest == "Station") {
            std::array<int, 5> stationPositions = {5, 15, 25, 35, 45};
            for (int pos : stationPositions) {
                if (pos > player.getPosition()) {
                    int step = pos - player.getPosition();
                    game.movePlayer(game.getCurrentPlayerIndex(), step);
                    Tile& currentTile = game.getBoard().getTile(player.getPosition());
                    currentTile.onLand(player,game,step);
                    break;
                }
            }
        } else if (nearest == "Utility") {
            std::array<int, 3> utilityPositions = {12, 28, 52};
            for (int pos : utilityPositions) {
                if (pos > player.getPosition()) {
                    int step = pos - player.getPosition();
                    game.movePlayer(game.getCurrentPlayerIndex(), step);
                    Tile& currentTile = game.getBoard().getTile(player.getPosition());
                    currentTile.onLand(player,game,step);
                    break;
                }
            }
        }
        break;
    }

    case CardType::COLLECT_MONEY: {
        player.addMoney(amount);
        cout << player.getName() << " collects $" << amount << "." << endl << endl;
        break;
    }

    case CardType::PAY_MONEY: {
        if (game.playerCanPay(game.getCurrentPlayerIndex(), amount)) {
            player.deductMoney(amount);
            cout << player.getName() << " pays $" << amount << "." << endl << endl;
        }
        break;
    }

    case CardType::COLLECT_FROM_PLAYERS: {
        game.receiveFromPlayers(game.getCurrentPlayerIndex(), amount);
        break;
    }

    case CardType::PAY_TO_PLAYERS: {
        game.payToPlayers(game.getCurrentPlayerIndex(), amount);
        break;
    }

    case CardType::REPAIRS: {
        int totalHouses = 0;
        int totalHotels = 0;
        for (const auto& property : player.getProperties()) {
            if (property->getHouses() == 5) {
                totalHotels++;
            }
            else {
                totalHouses += property->getHouses();
            }
        }
        int totalCost = (totalHouses * houseCost) + (totalHotels * hotelCost);
        if (game.playerCanPay(game.getCurrentPlayerIndex(), totalCost)) {
            player.deductMoney(totalCost);
            cout << player.getName() << " pays $" << totalCost << " for repairs." << endl << endl;
        }
        break;
    }

    case CardType::GET_OUT_OF_JAIL: {
        player.addGetOutOfJailCard(this);
        cout << player.getName() << " received a Get Out of Jail Free card." << endl << endl;
        break;
    }

    case CardType::GO_TO_JAIL: {
        cout << player.getName() << " goes to Jail." << endl << endl;
        player.setJailStatus(0);
        int step = 0;
        if (10 >= player.getPosition()) {
            step = 10 - player.getPosition();
        } else {
            step = 40 - player.getPosition() + 10;
        }
        game.movePlayer(game.getCurrentPlayerIndex(), step);
        break;
    }

    // Other card types to be implemented...
    default:
        break;
    }
    return;
}

CardDeck::CardDeck(string deckType) {
    if (deckType == "Community Chest") {
        vector<Card> communityChestCards = {
            Card("Advance to Go", CardType::MOVE_TO, 0, 0),
            Card("Bank error in your favor. Collect $200.", CardType::COLLECT_MONEY, 200),
            Card("Doctor's fees. Pay $50.", CardType::PAY_MONEY, 50),
            Card("Get $50 from sale of stock", CardType::COLLECT_MONEY, 50),
            Card("Get Out of Office Free. This card may be kept until needed or sold.", CardType::GET_OUT_OF_JAIL, 0, -1, 0, "", 0, 0, "Community Chest"),
            Card("Go to Presidential's Office. Go directly to Presidential's Office. Do not pass Go, do not collect $200.", CardType::GO_TO_JAIL),
            Card("Grand Opera Night. Collect $50 from every player for opening night seats.", CardType::COLLECT_FROM_PLAYERS, 50),
            Card("Holiday Fund matures. Receive $100.", CardType::COLLECT_MONEY, 100),
            Card("Income tax refund. Collect $20.", CardType::COLLECT_MONEY, 20),
            Card("It's your birthday! Collect $10 from each player.", CardType::COLLECT_FROM_PLAYERS, 10),
            Card("Life insurance matures. Collect $100.", CardType::COLLECT_MONEY, 100),
            Card("Pay hospital fees of $100.", CardType::PAY_MONEY, 100),
            Card("Pay school fees of $150.", CardType::PAY_MONEY, 150),
            Card("Receive $25 for services.", CardType::COLLECT_MONEY, 25),
            Card("Street repairs: Pay $40 per house and $115 per hotel you own.", CardType::REPAIRS, 0, -1, 0, "", 40, 115),
            Card("You have won second prize in a beauty contest. Collect $10.", CardType::COLLECT_MONEY, 10),
        };
        deck = std::deque<Card>(communityChestCards.begin(), communityChestCards.end());
    } else if (deckType == "Chance") {
        vector<Card> chanceCards = {
            Card("Advance to Go", CardType::MOVE_TO, 0, 0),
            Card("Advance to 24. If you pass Go, collect $200.", CardType::MOVE_TO, 0, 24),
            Card("Advance to 11. If you pass Go, collect $200.", CardType::MOVE_TO, 0, 11),
            Card("Advance to the nearest Station.", CardType::MOVE_TO_NEAREST, 0, -1, 0, "Station"),
            Card("Advance to the nearest Utility.", CardType::MOVE_TO_NEAREST, 0, -1, 0, "Utility"),
            Card("Advance to 39.", CardType::MOVE_TO, 0, 39),
            Card("Advance to 5. If you pass Go, collect $200.", CardType::MOVE_TO, 0, 5),
            Card("Bank pays you dividend of $50.", CardType::COLLECT_MONEY, 50),
            Card("Get Out of Presidential's Office Free. This card may be kept until needed or sold.", CardType::GET_OUT_OF_JAIL, 0, -1, 0, "", 0, 0, "Chance"),
            Card("Go Back 3 Spaces.", CardType::MOVE, 0, -1, -3),
            Card("Go to  Presidential's Office. Go directly to  Presidential's Office. Do not pass Go, do not collect $200.", CardType::GO_TO_JAIL),
            Card("Pay poor tax of $15.", CardType::PAY_MONEY, 15),
            Card("Take a trip to Bus Stop 1. If you pass Go, collect $200.", CardType::MOVE_TO, 0, 5),
            Card("Take a walk on Liwen Hall. Advance to Boardwalk.", CardType::MOVE_TO, 0, 39),
            Card("You have been elected Chairman of the Student Organization. Pay each player $50.", CardType::PAY_TO_PLAYERS, 50),
            Card("Your building loan matures. Collect $150.", CardType::COLLECT_MONEY, 150),
        };
        deck = std::deque<Card>(chanceCards.begin(), chanceCards.end());
    }

    // Shuffle the deck upon creation
    std::vector<Card> temp(deck.begin(), deck.end());
    std::shuffle(temp.begin(), temp.end(), rng);
    deck = std::deque<Card>(temp.begin(), temp.end());
}

Card CardDeck::draw() {
    Card card = deck.front();
    deck.pop_front();
    return card;
}

void CardDeck::returnCard(const Card& card) {
    deck.push_back(card);
}
