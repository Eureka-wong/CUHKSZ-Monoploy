#include <iostream>
#include <string>

#include "Player.h"
#include "CardDeck.h"
#include "Game.h"
#include "Tile.h"

using namespace std;

Player::Player(const string& name, int position, int balance)
    : name(name), position(position), balance(balance) {}

string Player::getName() const {
    return name;
}

int Player::getPosition () const {
    return position;
}

int Player::getCash() const {
    return balance;
}

vector<PropertyTile*> Player::getProperties() const {
    return properties;
}

int Player::getJailStatus() const {
    return jailStatus;
}

int Player::isBankrupt() const {
    return bankrupt;
}

void Player::showPlayer() const {
    cout << "[ Player: " << name << ", Position: " << position << ", Cash: $" << balance << ", Wealth: $" << this->calculateWealth() << "," << endl;
    cout << "  Properties: ";
    if (properties.empty()) {
        cout << "None, ";
    } else {
        for (const auto& property : properties) {
            cout << property->getName() << " (" << property->getIndex() << ") (Houses: " << property->getHouses() << "), ";
        }
    }
    if (!getOutOfJailCard.empty()) {
        cout << "Get Out of Jail Cards: " << getOutOfJailCard.size() << " ";
    } else {
        cout << "Get Out of Jail Cards: None ";
    }
    cout << "]" << endl;
}

void Player::setPosition(int newPosition) {
    if (newPosition < position && jailStatus == -1 && newPosition != 0) {
        balance += 200;
        cout << name << " passed Go." << endl;
        cout << name << " received $200." << endl << endl;
    }
    position = newPosition;
}

void Player::addMoney(int amount) {
    balance += amount;
}

void Player::deductMoney(int amount) {
    balance -= amount;
}

void Player::addProperty(PropertyTile* property) {
    properties.push_back(property);
}

void Player::removeProperty(PropertyTile* property) {
    auto eraseIndex = find(properties.begin(), properties.end(), property);
    if (eraseIndex != properties.end()) {
        properties.erase(eraseIndex);
    }
}

void Player::manageProperties(Game& game) {
    if (properties.empty()) {
        cout << name << " has no properties to manage." << endl;
        return;
    }

    cout << name << " is managing their properties." << endl;
    cout << "Type B(Index) if you would like to upgrade your property with houses or hotel." << endl;
    cout << "Type S(Index) if you would like to do sell your property (works hierarchly from hotel, houses, to the tile itself)." << endl;
    cout << "Type M(Index) if you would like to mortgage or unmortgage properties." << endl;
    cout << "Type T(Index) if you would like to sell a property to another player." << endl;
    cout << "Type t(Index) if you would like to buy a property to another player." << endl;
    cout << "For trade with another player, index 40 corresponds to \"Get out of Jail for Free\" card." << endl;
    cout << "Type E to end property management." << endl << endl;

    showPlayer();

    string command;
    while (true) {
        cout << endl;
        cout << "Enter command: ";
        getline(cin, command);
        if (command.empty()) {
            cout << "Invalid command. Please try again." << endl;
            continue;
        }
        if (command == "E") {
            int availableHouses = game.getAvailableBuildings(true);
            if (availableHouses < 0) {
                cout << "You are violating the 32 houses limit rule. Please sell your houses before ending your turn." << endl;
                cout << "Available Houses: " << availableHouses << endl;
                continue;
            }
            cout << "Ending property management." << endl;
            break;
        }
        int index;
        try {
            index = stoi(command.substr(1));
            if (index < 0 || index > 40) {
                cout << "Invalid index. Please try again." << endl;
                continue;
            }
            if (command[0] == 't') {
                //game.manageBuyTrade(*this, index);
                continue;
            }
            if (command[0] == 'T') {
                //game.manageSellTrade(*this, index);
                continue;
            }
            bool owned = false;
            for (auto& property : properties) {
                // Check if player owned the property they want to modify.
                if (property->getIndex() == index) {
                    if (command[0] == 'B') {
                        property->buyBuilding(*this, game);
                    } else if (command[0] == 'S') {
                        if (property->getHouses() == 0) {
                            property->sellProperty(*this, game);
                        } else {
                            property->sellBuilding(*this, game);
                        }
                    } else if (command[0] == 'M') {
                        property->mortgageProperty(*this, game);
                    } else {
                        cout << "Invalid command. Please try again." << endl;
                    }
                    owned = true;
                    cout << endl;
                    showPlayer();
                    break;
                }
            }
            if (!owned) {
                cout << "You do not own a property at index " << index << "." << endl;
            }
        } catch (const invalid_argument&) {
            cout << "Invalid index. Please try again." << endl;
            continue;
        }
    }
}

void Player::setJailStatus(int status) {
    jailStatus = status;
}

int Player::calculateWealth() const {
    int wealth = 0;
    wealth += balance;
    for (auto& property : properties) {
        if (!property->isMortgaged())
            wealth += property->calculateValue();
    }
    return wealth;
}

void Player::forceRaiseMoney(Game& game, int amount) {
    cout << name << " must raise money by selling or mortgaging properties." << endl << endl;
    cout << "Type S(Index) if you would like to do sell your property (works hierarchly from hotel, houses, to the tile itself)." << endl;
    cout << "Type M(Index) if you would like to mortgage or unmortgage properties." << endl;

    showPlayer();

    string command;
    while (balance < amount) {
        cout << endl;
        cout << "Enter command: ";
        getline(cin, command);
        if (command.empty()) {
            cout << "Invalid command. Please try again." << endl;
            continue;
        }
        int index;
        try {
            index = stoi(command.substr(1));
            bool owned = false;
            for (auto& property : properties) {
                // Check if player owned the property they want to modify.
                if (property->getIndex() == index) {
                    if (command[0] == 'S') {
                        if (property->getHouses() == 0) {
                            property->sellProperty(*this, game);
                        } else {
                            property->sellBuilding(*this, game);
                        }
                    } else if (command[0] == 'M') {
                        property->mortgageProperty(*this, game);
                    } else {
                        cout << "Invalid command. Please try again." << endl;
                        break;
                    }
                    owned = true;
                    cout << endl;
                    showPlayer();
                    break;
                }
            }
            if (!owned) {
                cout << "You do not own a property at index " << index << "." << endl;
            }
        } catch (const invalid_argument&) {
            cout << "Invalid index. Please try again." << endl;
            continue;
        }
    }
    cout << endl;
}

void Player::declareBankruptcy(Game& game, Player* creditor) {
    cout << name << " is bankrupt!" << endl << endl;
    bankrupt = true;

    if (creditor == nullptr) {
        for (auto& property : properties) {
            property->liquidateBuildings(game, nullptr);
            property->transferOwnership(*this, nullptr, game);
        }
        cout << name << "'s assets are returned to the bank." << endl;
        balance = 0;
        cout << endl << name << " has been removed from the game." << endl << endl;
        return;
    }

    // Creditor must immediately pay mortgaged properties' transfer fee
    // If they can't they are also declared bankrupt
    int transferFee = 0;
    for (auto& property : properties) {
        if (property->isMortgaged()) {
            transferFee += static_cast<int>((property->getPrice() / 2) * 0.1);
        }
    }

    if (transferFee > 0) {
        cout << name << " has mortgaged property," << endl;
        cout << creditor->getName() << " must pay transfer fee of $" << transferFee << "." << endl;
        // 这里原本是*creditor
        if (game.playerCanPay(game.getCurrentPlayerIndex(), transferFee)) {
            creditor->deductMoney(transferFee);
            cout << creditor->getName() << " paid a transfer fee of $" << transferFee << "." << endl << endl;
        }
        else {
            for (auto& property : properties) {
                property->liquidateBuildings(game, nullptr);
                property->transferOwnership(*this, nullptr, game);
            }
            cout << name << "'s assets are returned to the bank." << endl;
            balance = 0;
            cout << endl << name << " has been removed from the game." << endl << endl;
            return;
        }
    }

    // Transfer properties
    cout << "All " << name << "'s assets are being transferred to " << creditor->getName() << "." << endl << endl;
    for (auto& property : properties) {
        property->liquidateBuildings(game, creditor);
        property->transferOwnership(*this, creditor, game);
    }

    // Transfer money and cards
    creditor->addMoney(balance);
    while (!getOutOfJailCard.empty()) {
        Card* card = getOutOfJailCard.back();
        getOutOfJailCard.pop_back();
        creditor->addGetOutOfJailCard(card);
    }

    balance = 0;
    cout << "All " << name << "'s assets have been transferred to " << creditor->getName() << "." << endl << endl;
    cout << name << " has been removed from the game." << endl << endl;
}

void Player::addGetOutOfJailCard(Card* card) {
    getOutOfJailCard.push_back(card);
}

bool Player::hasGetOutOfJailCard() const {
    return !getOutOfJailCard.empty();
}

int Player::ownedGetOutOfJailCard() const {
    if (hasGetOutOfJailCard()) {
        return getOutOfJailCard.size();
    }
    return 0;
}

Card* Player::takeOutGetOutOfJailCard() {
    if (getOutOfJailCard.empty()) {
        return nullptr;
    }
    Card* card = getOutOfJailCard.back();
    getOutOfJailCard.pop_back();
    return card;
}

void Player::getTradeDetails(int& playerIndex, int& amount, bool buyProperty) {
    cout << "Type the player index and amount to propose a trade." << endl;
    cout << "(If buying a property, fill player index with any number.)" << endl << endl;

    if (!buyProperty) {
        while (true) {
            cout << "Enter player index: ";
            string indexInput;
            getline(cin, indexInput);
            try {
                playerIndex = stoi(indexInput);
                break;
            }
            catch (const invalid_argument&) {
                cout << "Invalid input. Please enter a valid integer." << endl << endl;
                continue;
            }
        }
        cout << endl;
    }
    else {
        playerIndex = -1;
    }

    while (true) {
        cout << "Enter amount: ";
        string amountInput;
        getline(cin, amountInput);
        try {
            amount = stoi(amountInput);
            break;
        }
        catch (const invalid_argument&) {
            cout << "Invalid input. Please enter a valid integer." << endl << endl;
            continue;
        }
    }
}
