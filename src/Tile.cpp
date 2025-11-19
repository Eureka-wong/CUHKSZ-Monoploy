#include <iostream>
#include <string>
#include <map>
#include <cmath>

#include "Tile.h"
#include "CardDeck.h"
#include "Game.h"
#include "Player.h"

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
    cout << player.getName() << " collects $200 for passing Go!" << endl << endl;
    player.addMoney(200);
    game.showPlayers();
    game.showBoard();
}


PropertyTile::PropertyTile(TileInfo const& info) :
    Tile(info.name, info.index), price(info.price), housePrice(info.housePrice), rent(info.rent), group(info.group) {}

void PropertyTile::onLand(Player& player, Game& game, int step) {
    cout << player.getName() << " landed on " << name << " (Property)" << endl;
    cout << endl;
    game.showPlayers();
    game.showBoard();

    if (owner == nullptr) {
        /*
        Property Purchase Rule: 
           - Players must immediately decide to buy or not buy an unowned property once they land on it.
           - The decision is final for the turn, as no auctions or second chances will be provided. 
        */
        cout << name << " is unowned. Do you want to buy it for $" << price << "? (Y/N): ";
        char choice;
        while (true) {
            cin >> choice;
            cin.ignore(10000, '\n');
            if (choice == 'Y' || choice == 'N') {
                break;
            }
            cout << "Invalid input. Please enter Y or N: ";
            cin.clear();
        }
        cout << endl;
        if (choice == 'Y') {
            if (player.getCash() >= price) {
                buyProperty(player);
            } else {
                cout << player.getName() << " does not have enough money to buy " << name << "." << endl << endl;
            }
        }
    } else if (owner != &player && !mortgaged) {
        // Pay rent to other player
        cout << name << " is owned by " << owner->getName() << "." << endl;

        int rentDue;
        calculateRent(step, rentDue);

        if (game.playerCanPay(player, rentDue, owner)) {
            player.deductMoney(rentDue);
            owner->addMoney(rentDue);
            cout << player.getName() << " paid $" << rentDue << " rent to " << owner->getName() << "." << endl << endl;
        }
    } else if (mortgaged) {
        cout << name << " is owned by " << owner->getName() << "." << endl;
        cout << name << " is mortgaged. No rent is due." << endl << endl;
    }
}

void PropertyTile::buyProperty(Player &player) {
    if (owner != nullptr) {
        cout << name << " is already owned." << endl;
        return;
    }
    if (player.getCash() < price) {
        cout << "You do not have enough money to buy " << name << "." << endl;
        return;
    }

    player.deductMoney(price);

    // Assign ownership
    owner = &player;
    player.addProperty(this);

    cout << player.getName() << " bought " << name << " for $" << price << "." << endl << endl;
    player.showPlayer();
    cout << endl;
}

void PropertyTile::sellProperty(Player &player) {
    if (!ownedByPlayer(player)) {
        return;
    }
    if (!allPropertyInGroupHasNoHouses()) {
        cout << "You cannot sell if any of the property of the same color group has buildings." << endl;
        return;
    }
    if (mortgaged) {
        cout << "You cannot sell mortgaged property." << endl;
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
        cout << "You either do not own all properties in the " << group << " group or some of them are mortgaged." << endl;
        return;
    }
    if (mortgaged) {
        cout << "You cannot buy building on mortgaged property." << endl;
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

        cout << player.getName() << " built a hotel on " << name << "." << endl;
        game.modifyAvailableBuildings(true, 4);
        game.modifyAvailableBuildings(false, -1);
    } else {
        if (game.getAvailableBuildings(true) == 0) {
            cout << "No houses are available to build at the moment." << endl;
            return;
        }

        houses++;
        player.deductMoney(housePrice);

        cout << player.getName() << " built a house on " << name << ". Total houses: " << houses << "." << endl;
        game.modifyAvailableBuildings(true, -1);
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

        cout << player.getName() << " reverted a hotel back to 4 houses on " << name << "." << endl;
        game.modifyAvailableBuildings(false, 1);
        game.modifyAvailableBuildings(true, -4);
    } else {
        houses--;

        int sellPrice = housePrice / 2;
        player.addMoney(sellPrice);

        cout << player.getName() << " sold a house on " << name << ". Total houses: " << houses << "." << endl;
        game.modifyAvailableBuildings(true, 1);
    }
}

void PropertyTile::mortgageProperty(Player &player) {
    if (!ownedByPlayer(player)) {
        return;
    }
    if (!allPropertyInGroupHasNoHouses()) {
        cout << "You cannot mortgage if any of the property of the same color group has buildings." << endl;
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

void PropertyTile::liquidateBuildings(Game& game, Player* creditor) {
    if (creditor != nullptr) {
        creditor->addMoney((houses * housePrice) / 2);
    }
    if (houses == 5) {
        game.modifyAvailableBuildings(true, 4);
        game.modifyAvailableBuildings(false, 1);
    }
    else if (houses > 0) {
        game.modifyAvailableBuildings(true, houses);
    }
    houses = 0;
}

void PropertyTile::transferOwnership(Player& previousOwner, Player* newOwner) {
    previousOwner.removeProperty(this);
    if (newOwner != nullptr) {
        newOwner->addProperty(this);
        owner = newOwner;
        cout << newOwner->getName() << " acquired " << name << " from " << previousOwner.getName() << endl;
        if (this->mortgaged) {
            int unmortgageCost = static_cast<int>((price / 2) * 1.1);
            cout << name << " is mortgaged. Do you want to unmortgage it for $" << unmortgageCost << "? (Y/N): ";
            char choice;
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
                mortgageProperty(*newOwner);
            }
        }
        cout << endl;
    }
    else {
        mortgaged = false;
        owner = newOwner;
        cout << name << " has been returned to the bank." << endl << endl;
    }
}

int PropertyTile::getPrice() const {
    return price;
}

int PropertyTile::getHouses() const {
    return houses;
}

Player* PropertyTile::getOwner() const {
    return owner;
}

bool PropertyTile::isMortgaged() const {
    return mortgaged;
}

int PropertyTile::countOwnedPropertiesInGroup() const {
    int count = 0;
    for (const auto& property : owner->getProperties()) {
        if (property->group == group && !property->isMortgaged()) {
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

bool PropertyTile::allPropertyInGroupHasNoHouses() const {
    for (const auto& property : owner->getProperties()) {
        if (property->group == group && property->getHouses() > 0) {
            return false;
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

int PropertyTile::calculateValue() const {
    int value = 0;
    value += price / 2;
    value += (houses * housePrice) / 2;
    return value;
}


FreeParkingTile::FreeParkingTile(TileInfo const& info) :
    Tile(info.name, info.index) {}

void FreeParkingTile::onLand(Player &player, Game &game, int step) {
    if (index == 10) {
        cout << player.getName() << " landed on " << name << " (Just Visiting)" << endl << endl;
    }
    else {
        cout << player.getName() << " landed on " << name << " (Free Parking)" << endl;
        cout << "Nothing happens. Enjoy your free parking!" << endl << endl;
    }
    game.showPlayers();
    game.showBoard();
}


TaxTile::TaxTile(const TileInfo& info) :
    Tile(info.name, info.index), tax(info.price) {}

void TaxTile::onLand(Player &player, Game &game, int step) {
    cout << player.getName() << " landed on " << name << " (Tax)" << endl;
    cout << player.getName() << " must pay $" << tax << " in taxes." << endl << endl;
    if (game.playerCanPay(player, tax)) {
        player.deductMoney(tax);
        game.showPlayers();
        game.showBoard();
    }
}

GoToJailTile::GoToJailTile(const TileInfo& info) :
    Tile(info.name, info.index) {}

void GoToJailTile::onLand(Player &player, Game &game, int step) {
    cout << player.getName() << " landed on " << name << " (Go to Jail)." << endl;
    cout << player.getName() << " goes to jail." << endl << endl;
    player.setJailStatus(0);
    player.setPosition(10);
}

ChanceTile::ChanceTile(const TileInfo& info) :
    Tile(info.name, info.index) {}

void ChanceTile::onLand(Player &player, Game &game, int step) {
    cout << player.getName() << " landed on " << name << " (Chance)." << endl;
    game.drawChanceCard(player);
}

CommunityChestTile::CommunityChestTile(const TileInfo& info) :
    Tile(info.name, info.index) {}

void CommunityChestTile::onLand(Player &player, Game &game, int step) {
    cout << player.getName() << " landed on " << name << " (Community Chest)." << endl;
    game.drawCommunityChestCard(player);
}