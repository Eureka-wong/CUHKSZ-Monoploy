#include <iostream>
#include <string>
#include <map>
#include <cmath>

#include "Tile.h"
#include "Player.h"
#include "Game.h"

using namespace std;

TileInfo::TileInfo(string tileType, string name, int index, int price, int housePrice,
             array<int, 6> rent, string group) :
            tileType(std::move(tileType)), name(std::move(name)), index(index), price(price), housePrice(housePrice), rent(rent), group(std::move(group)) {};


Tile::Tile(const string& name, int index) :
    name(name), index(index) {}

string Tile::getName() const {
    return name;
}

int Tile::getIndex() const {
    return index;
}


GoTile::GoTile() :
    Tile("Go", 0) {}

void GoTile::onLand(Player& player, Game& game, int step) {
    cout << player.getName() << " landed on " << name << " (Go)" << endl;
    cout << player.getName() << " collects $200 for passing Go!" << endl;
    game.showPlayers();
    game.showBoard();
    player.addMoney(200);
}


PropertyTile::PropertyTile(TileInfo const& info) :
    Tile(info.name, info.index), price(info.price), housePrice(info.housePrice), rent(info.rent), group(info.group) {}

void PropertyTile::onLand(Player& player, Game& game, int step) {
    cout << player.getName() << " landed on " << name << " (Property)" << endl;
    cout << endl;
    game.showPlayers();
    game.showBoard();

    if (owner == nullptr) {
        char choice;
        /*
        Property Purchase Rule: 
           - Players must immediately decide to buy or not buy an unowned property once they land on it.
           - The decision is final for the turn as no auctions or second chances will be provided. 
        */
        cout << name << " is unowned. Do you want to buy it for $" << price << "? (Y/N): ";
        while (true) {
            cin >> choice;
            cin.ignore(10000, '\n');
            if (choice == 'Y' || choice == 'N') {
                break;
            }
            cout << "Invalid input. Please enter Y or N: ";
            cin.clear();
        }
        if (choice == 'Y') {
            if (player.getCash() >= price) {
                buyProperty(player);
            } else {
                cout << player.getName() << " does not have enough money to buy " << name << "." << endl;
            }
        }
    } else if (owner != &player && !mortgaged) {
        // Pay rent to other player
        cout << name << " is owned by " << owner->getName() << "." << endl;

        int rentDue;
        calculateRent(step, rentDue);
        player.deductMoney(rentDue);
        owner->addMoney(rentDue);

        cout << player.getName() << " paid $" << rentDue << " rent to " << owner->getName() << "." << endl;
    } else if (mortgaged) {
        cout << name << " is owned by " << owner->getName() << "." << endl;
        cout << name << " is mortgaged. No rent is due." << endl;
    }
    cout << endl;
}

void PropertyTile::buyProperty(Player &player) {
    if (owner != nullptr) {
        cout << name << " is already owned." << endl;
        return;
    }

    player.deductMoney(price);

    // Assign ownership
    owner = &player;
    player.addProperty(this);

    cout << player.getName() << " bought " << name << " for $" << price << "." << endl;
}

void PropertyTile::sellProperty(Player &player) {
    if (!ownedByPlayer(player)) {
        return;
    }

    int sellPrice = price / 2;
    player.addMoney(sellPrice);

    // Remove ownership
    owner = nullptr;
    player.removeProperty(this);

    cout << player.getName() << " sold " << name << " for $" << sellPrice << "." << endl;
}

void PropertyTile::buyBuilding(Player &player, Game& game) {
    if (!ownedByPlayer(player) || isStationOrUtility()) {
        return;
    }
    if (player.getCash() < housePrice) {
        cout << "You do not have enough money to buy a house on " << name << "." << endl;
        return;
    }
    if (!ownColorGroup()) {
        cout << "You must own all properties in the " << group << " group to buy houses." << endl;
        return;
    }
    if (!allowHouseTransactions(true)) {
        cout << "You must build houses evenly across the " << group << " group." << endl;
        return;
    }
    if (houses == 5) {
        cout << name << " already has the maximum number of houses/hotel." << endl;
        return;
    }

    if (houses + 1 == 5) {
        if (game.getAvailableBuildings(false) == 0) {
            cout << "No hotels are available to build at the moment." << endl;
            return;
        }

        houses++;
        player.deductMoney(housePrice);

        game.modifyAvailableBuildings(true, 4);
        game.modifyAvailableBuildings(false, -1);

        cout << player.getName() << " built a hotel on " << name << "." << endl;
    } else {
        if (game.getAvailableBuildings(true) == 0) {
            cout << "No houses are available to build at the moment." << endl;
            return;
        }

        houses++;
        player.deductMoney(housePrice);

        game.modifyAvailableBuildings(true, -1);

        cout << player.getName() << " built a house on " << name << ". Total houses: " << houses << "." << endl;
    }
}

void PropertyTile::sellBuilding(Player &player, Game& game) {
    if (!ownedByPlayer(player) || isStationOrUtility()) {
        return;
    }
    if (houses == 0) {
        cout << name << " has no houses to sell." << endl;
        return;
    }
    if (!allowHouseTransactions(false)) {
        cout << "You must sell houses evenly across the " << group << " group." << endl;
        return;
    }

    if (houses - 1 == 4) {
        houses--;
       
        int sellPrice = housePrice / 2;
        player.addMoney(sellPrice);

        game.modifyAvailableBuildings(false, 1);
        game.modifyAvailableBuildings(true, -4);

        cout << player.getName() << " reverted a hotel back to 4 houses on " << name << "." << endl;
    } else {
        houses--;

        int sellPrice = housePrice / 2;
        player.addMoney(sellPrice);

        game.modifyAvailableBuildings(true, 1);
        cout << player.getName() << " sold a house on " << name << ". Total houses: " << houses << "." << endl;
    }
}

void PropertyTile::mortgageProperty(Player &player) {
    if (!ownedByPlayer(player)) {
        return;
    }

    if (!mortgaged) {
        int mortgageValue = price / 2;
        player.addMoney(mortgageValue);
        mortgaged = true;
        cout << player.getName() << " mortgaged " << name << " for $" << mortgageValue << "." << endl;
    }
    else if (mortgaged) {
        int unmortgageCost = static_cast<int>((price / 2) * 1.1);
        if (player.getCash() < unmortgageCost) {
            cout << "You do not have enough money to unmortgage " << name << "." << endl;
            return;
        }
        player.deductMoney(unmortgageCost);
        mortgaged = false;
        cout << player.getName() << " unmortgaged " << name << " by paying $" << unmortgageCost << "." << endl;
    }
}

int PropertyTile::getHouses() const {
    return houses;
}

int PropertyTile::getPrice() const{
    return price;
}

int PropertyTile::getHousePrice() const{
    return housePrice;
}

bool PropertyTile::isMortgaged() const{
    return mortgaged;
}

std::array<int, 6> PropertyTile::getRent() const{
    return rent;
}

string PropertyTile::getGroup() const{
    return group;
}

Player* PropertyTile::getOwner() const{
    return owner;
}

int PropertyTile::countOwnedPropertiesInGroup() const {
    int count = 0;
    for (const auto& property : owner->getProperties()) {
        if (property->group == group) {
            count++;
        }
    }
    return count;
}

bool PropertyTile::ownColorGroup() const {
    int ownedPropertiesInGroup = countOwnedPropertiesInGroup();
    if (group == "Brown" || group == "Dark Blue") {
        return 2 == ownedPropertiesInGroup;
    } else {
        return 3 == ownedPropertiesInGroup;
    }
}

bool PropertyTile::allowHouseTransactions(bool buy) const {
    for (const auto& property : owner->getProperties()) {
        if (property->group == group && property != this) {
            // Make sure no property of the same group are built 2 tier ahead.
            if (buy && houses + 1 - property->houses > 1) {
                return false;
            }
            else if (!buy && property->houses - (houses - 1) > 1) {
                return false;
            }
        }
    }
    return true;
}

void PropertyTile::calculateRent(int step, int& rentDue) const {
    int ownedPropertiesInGroup = countOwnedPropertiesInGroup();
    if (group == "Station") {
        rentDue = rent[ownedPropertiesInGroup - 1];
    } else if (group == "Utility") {
        rentDue = (ownedPropertiesInGroup == 1) ? step * 4 : step * 10;
    } else {
        if (houses == 0) {
            if (ownColorGroup()) {
                rentDue = rent[0] * 2;
            } else {
                rentDue = rent[0];
            }
        }
        else {
            rentDue = rent[houses];
        }
    }
}

bool PropertyTile::ownedByPlayer(Player& player) const {
    if (owner != &player) {
        cout << "You do not own " << name << "." << endl;
        return false;
    }
    return true;
}

bool PropertyTile::isStationOrUtility() const {
    if (group == "Station" || group == "Utility") {
        cout << "Buildings cannot be built on station or utility tiles" << endl;
        return true;
    }
    return false;
}


FreeParkingTile::FreeParkingTile(TileInfo const& info) :
    Tile(info.name, info.index) {}

void FreeParkingTile::onLand(Player &player, Game &game, int step) {
    cout << player.getName() << " landed on " << name << " (Free Parking)" << endl;
    cout << "Nothing happens. Enjoy your free parking!" << endl;
    game.showPlayers();
    game.showBoard();
}


TaxTile::TaxTile(const TileInfo& info) :
    Tile(info.name, info.index), tax(info.price) {}

void TaxTile::onLand(Player &player, Game &game, int step) {
    cout << player.getName() << " landed on " << name << " (Tax)" << endl;
    cout << player.getName() << " must pay $" << tax << " in taxes." << endl;
    game.showPlayers();
    game.showBoard();
    player.deductMoney(tax);
}
