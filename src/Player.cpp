#include <iostream>
#include <string>

#include "Player.h"
#include "Tile.h"
#include "Game.h"

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

void Player::showPlayer() const {
    cout << "[ Player: " << name << ", Position: " << position << ", Cash: $" << balance << "," << endl;
    cout << "  Properties: ";
    if (properties.empty()) {
        cout << "None ";
    } else {
        for (const auto& property : properties) {
            cout << property->getName() << " (" << property->getIndex() << ") (Houses: " << property->getHouses() << "), ";
        }
    }
    cout << "]" << endl;
}

void Player::setPosition(int newPosition) {
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
    cout << "Press B(Index) if you would like to upgrade your property with houses or hotel." << endl;
    cout << "Press S(Index) if you would like to do sell your property (works hierarchly from hotel, houses, to the tile itself)." << endl;
    cout << "Press M(Index) if you would like to mortgage or unmortgage properties." << endl;
    cout << "Press E to end property management." << endl;

    cout << endl;
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
            bool owned = false;
            for (auto& property : properties) {
                // Check if player owned the property they want to modify.
                if (property->getIndex() == index) {
                    if (command[0] == 'B') {
                        property->buyBuilding(*this, game);
                    } else if (command[0] == 'S') {
                        if (property->getHouses() == 0) {
                            property->sellProperty(*this);
                        } else {
                            property->sellBuilding(*this, game);
                        }
                    } else if (command[0] == 'M') {
                        property->mortgageProperty(*this);
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
}