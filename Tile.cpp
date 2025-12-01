#include <iostream>
#include <string>
#include <QObject>
#include <cmath>

#include "Tile.h"
#include "CardDeck.h"
#include "Game.h"
#include "Player.h"

using namespace std;

TileInfo::TileInfo(string tileType, string name, int index, int price, int housePrice,
                   array<int, 6> rent, string group) :
    tileType(std::move(tileType)), name(std::move(name)), index(index), price(price), housePrice(housePrice), rent(rent), group(std::move(group)) {};


Tile::Tile(const string& name, int index, QObject* parent):
    QObject(parent), name(name), index(index) {}

string Tile::getName() const {
    return name;
}

int Tile::getIndex() const {
    return index;
}


GoTile::GoTile(QObject* parent) :
    Tile("Go", 0, parent) {}

void GoTile::onLand(Player& player, Game& game, int step) {
    cout << player.getName() << " landed on " << name << " (Go)" << endl;
    cout << player.getName() << " collects $200 for passing Go!" << endl << endl;
    player.addMoney(200);
}


PropertyTile::PropertyTile(TileInfo const& info, QObject* parent) :
    Tile(info.name, info.index, parent), price(info.price), housePrice(info.housePrice), rent(info.rent), group(info.group) {}

void PropertyTile::onLand(Player& player, Game& game, int step) {
    if (owner == nullptr) {
        // This property is unowned. Emit purchaseOpportunity signal to UI to prompt user for purchase decision
        emit game.purchaseOpportunity(QString::fromStdString(name), price, game.getPlayerIndex(player));
    }else if (owner != &player && !mortgaged){
        // This property is owned by someone else, and it isn't motaged, so pay rent to the owner
        int rentAmount = calculateRent(step);
        if (game.playerCanPay(game.getCurrentPlayerIndex(),rentAmount)){
            emit game.rentPaymentRequired(QString::fromStdString(name),rentAmount,game.getCurrentPlayerIndex(),game.getPlayerIndex(*owner));
        } else{
            emit game.playerBankrupt(game.getCurrentPlayerIndex());
        }
    }
    else if (mortgaged) {
        emit game.gameLogMessage(QString::fromStdString("%1 is owned by %2").arg(name).arg(owner->getName()));
        emit game.gameLogMessage(QString::fromStdString("This property is mortaged, no rent is due."));
    }
}

void PropertyTile::buyProperty(Player &player) {
    player.deductMoney(price);
    owner = &player;
    player.addProperty(this);
}

/*
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
}*/

bool PropertyTile::sellProperty(Player &player, Game& game) {
    if (!ownedByPlayer(player)) {
        emit game.warningSignal("You do not own this property.");
        return false;
    }
    if (!allPropertyInGroupHasNoHouses()) {
        emit game.warningSignal("You cannot sell if any of the property of the same color group has buildings.");
        return false;
    }
    if (mortgaged) {
        emit game.warningSignal("You cannot sell mortgaged property.");
        return false;
    }

    int sellPrice = price / 2;
    player.addMoney(sellPrice);
    owner = nullptr;
    player.removeProperty(this);
    emit game.gameLogMessage(QString::fromStdString("%1 sold %2 for $%3").arg(player.getName()).arg(name).arg(sellPrice));
    return true;
}

/*
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
}*/

bool PropertyTile::buyBuilding(Player &player, Game& game) {
    if (!ownedByPlayer(player)) {
        emit game.warningSignal("You do not own this property.");
        return false;
    }
    if (isStationOrUtility()) {
        emit game.warningSignal("You cannot buy buildings on Station or Utility.");
        return false;
    }
    if (player.getCash() < housePrice) {
        emit game.warningSignal("You don't have enough money to buy this building.");
        return false;
    }
    if (!ownColorGroup()) {
        emit game.warningSignal("You don't own all properties in this color group or some are mortgaged.");
        return false;
    }
    if (mortgaged) {
        emit game.warningSignal("Cannot buy building on mortgaged property.");
        return false;
    }
    if (!allowHouseTransactions(true)) {
        emit game.warningSignal("You must build houses evenly across this color group.");
        return false;
    }
    if (houses == 5) {
        emit game.warningSignal("This property already has a hotel (max buildings).");
        return false;
    }

    if (houses + 1 == 5) {
        if (game.getAvailableBuildings(false) == 0) {
            emit game.warningSignal("No hotels available to build.");
            return false;
        }
        houses++;
        player.deductMoney(housePrice);
        game.modifyAvailableBuildings(true, 4);
        game.modifyAvailableBuildings(false, -1);
        emit game.gameLogMessage(QString::fromStdString("%1 built a hotel on %2 for $%3").arg(player.getName()).arg(name).arg(housePrice));
    } else {
        if (game.getAvailableBuildings(true) == 0) {
            emit game.warningSignal("No houses available to build.");
            return false;
        }
        houses++;
        player.deductMoney(housePrice);
        game.modifyAvailableBuildings(true, -1);
        emit game.gameLogMessage(QString::fromStdString("%1 built a house on %2 for $%3").arg(player.getName()).arg(name).arg(housePrice));
    }
    return true;
}

/*void PropertyTile::sellBuilding(Player &player, Game& game) {
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
}*/

bool PropertyTile::sellBuilding(Player &player, Game& game) {
    if (!ownedByPlayer(player)) {
        emit game.warningSignal("You do not own this property");
        return false;
    }
    if (isStationOrUtility()) {
        emit game.warningSignal("Cannot sell buildings on Station or Utility");
        return false;
    }
    if (houses == 0) {
        emit game.warningSignal("This property has no buildings to sell");
        return false;
    }
    if (!allowHouseTransactions(false)) {
        emit game.warningSignal("You must sell houses evenly across this color group");
        return false;
    }

    // Success case - sell house/hotel
    if (houses - 1 == 4) {
        houses--;
        int sellPrice = housePrice / 2;
        player.addMoney(sellPrice);
        game.modifyAvailableBuildings(false, 1);
        game.modifyAvailableBuildings(true, -4);
        emit game.gameLogMessage(QString::fromStdString("%1 reverted a hotel back to 4 houses on %2 for $%3").arg(player.getName()).arg(name).arg(sellPrice));
    } else {
        houses--;
        int sellPrice = housePrice / 2;
        player.addMoney(sellPrice);
        game.modifyAvailableBuildings(true, 1);
        emit game.gameLogMessage(QString::fromStdString("%1 sold a house on %2 for $%3").arg(player.getName()).arg(name).arg(sellPrice));
    }
    return true;
}

/*
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
}*/

bool PropertyTile::mortgageProperty(Player &player, Game& game) {
    if (!ownedByPlayer(player)) {
        emit game.warningSignal("You do not own this property.");
        return false;
    }
    if (!allPropertyInGroupHasNoHouses()) {
        emit game.warningSignal("You cannot mortgage if any of the property of the same color group has buildings.");
        return false;
    }

    if (!mortgaged) {
        int mortgageValue = price / 2;
        player.addMoney(mortgageValue);
        mortgaged = true;
        emit game.gameLogMessage(QString::fromStdString("%1 mortgaged %2 for $%3.").arg(player.getName()).arg(name).arg(mortgageValue));
    }
    else if (mortgaged) {
        int unmortgageCost = static_cast<int>((price / 2) * 1.1);
        if (player.getCash() < unmortgageCost) {
            emit game.warningSignal("You do not have enough money to unmortgage this property");
            return false;
        }
        player.deductMoney(unmortgageCost);
        mortgaged = false;
        emit game.gameLogMessage(QString::fromStdString("%1 unmortgaged %2 for $%3s.").arg(player.getName()).arg(name).arg(unmortgageCost));
    }
    return true;
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

void PropertyTile::transferOwnership(Player& previousOwner, Player* newOwner, Game& game) {
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
                mortgageProperty(*newOwner, game);
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

int PropertyTile::getHousePrice() const{
    return housePrice;
}

std::array<int, 6> PropertyTile::getRent() const{
    return rent;
};

std::string PropertyTile::getGroup() const{
    return group;
};


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

int PropertyTile::calculateRent(int step) const {
    int rentDue;
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
    return rentDue;
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


FreeParkingTile::FreeParkingTile(TileInfo const& info,QObject* parent) :
    Tile(info.name, info.index,parent) {}

void FreeParkingTile::onLand(Player &player, Game &game, int step) {
    if (index == 10) {
        cout << player.getName() << " landed on " << name << " (Just Visiting)" << endl << endl;
    }
    else {
        cout << player.getName() << " landed on " << name << " (Free Parking)" << endl;
        cout << "Nothing happens. Enjoy your free parking!" << endl << endl;
    }
    /*game.showPlayers();
    game.showBoard();*/
}


TaxTile::TaxTile(const TileInfo& info, QObject* parent) :
    Tile(info.name, info.index, parent), tax(info.price) {}

void TaxTile::onLand(Player &player, Game &game, int step) {
    cout << player.getName() << " landed on " << name << " (Tax)" << endl;
    cout << player.getName() << " must pay $" << tax << " in taxes." << endl << endl;
    if (game.playerCanPay(game.getCurrentPlayerIndex(), tax)) {
        player.deductMoney(tax);
    }
}

GoToJailTile::GoToJailTile(const TileInfo& info,QObject* parent) :
    Tile(info.name, info.index,parent) {}

void GoToJailTile::onLand(Player &player, Game &game, int step) {
    cout << player.getName() << " landed on " << name << " (Go to Jail)." << endl;
    cout << player.getName() << " goes to jail." << endl << endl;
    player.setJailStatus(0);
    player.setPosition(10);
}

ChanceTile::ChanceTile(const TileInfo& info,QObject* parent) :
    Tile(info.name, info.index,parent) {}

void ChanceTile::onLand(Player &player, Game &game, int step) {
    cout << player.getName() << " landed on " << name << " (Chance)." << endl;
    game.drawChanceCard(game.getCurrentPlayerIndex());
}

CommunityChestTile::CommunityChestTile(const TileInfo& info, QObject* parent) :
    Tile(info.name, info.index, parent) {}

void CommunityChestTile::onLand(Player &player, Game &game, int step) {
    cout << player.getName() << " landed on " << name << " (Community Chest)." << endl;
    game.drawCommunityChestCard(game.getCurrentPlayerIndex());
}
