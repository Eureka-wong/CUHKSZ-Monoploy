#include <iostream>
#include <string>
#include <random>

#include "Game.h"
#include "Player.h"
#include "Tile.h"

using namespace std;

Game::Game() : numPlayers(getNumPlayers()), board() {
    // -1 if test player is used
    for (int i = 0; i < numPlayers; i++) {
        string name;
        cout << "Enter name for player " << (i + 1) << ": ";
        getline(cin, name);
        addPlayer(name);
    }
    cout << endl;
}

int Game::getNumPlayers() {
    int num;
    while (true) {
        cout << "Enter number of players: ";
        if (cin >> num && num >= 2 && num <= 4) {
            cin.ignore(10000, '\n');
            return num; // +1 if test player is used
        }
        cout << "Invalid! Please enter 2, 3, or 4.\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }
}

void Game::addPlayer(string name) {
    players.emplace_back(Player(name, 0, 1500));
}

// For testing purposes, add a player with preset properties
void Game::testAddPlayer(string name) {
    players.emplace_back(Player(name, 0, 2000));
    Player& currentPlayer = players.back();
    int positions[] = {1, 3, 6, 8, 9};
    for (int pos : positions) {
        Tile& tile = board.getTile(pos);
        if (auto* property = dynamic_cast<PropertyTile*>(&tile)) {
            property->buyProperty(currentPlayer);
        }
    }
    for (int i = 0; i < 5; i++) {
        dynamic_cast<PropertyTile*>(&board.getTile(1))->buyBuilding(currentPlayer, *this);
        dynamic_cast<PropertyTile*>(&board.getTile(3))->buyBuilding(currentPlayer, *this);
    }
    for (int i = 0; i < 4; i++) {
        dynamic_cast<PropertyTile*>(&board.getTile(6))->buyBuilding(currentPlayer, *this);
        dynamic_cast<PropertyTile*>(&board.getTile(8))->buyBuilding(currentPlayer, *this);
        dynamic_cast<PropertyTile*>(&board.getTile(9))->buyBuilding(currentPlayer, *this);
    }
}

void Game::startGame() {
    this->showPlayers();
    this->showBoard();
    while (round < 5) {
        for (int i = 0; i < numPlayers; i++) {
            cout << "---------------------------------------------------------------" << endl;
            cout << endl;
            // Ask player to roll the dice
            Player& currentPlayer = players[currentPlayerIndex];
            cout << "Round " << (round + 1) << ", " << currentPlayer.getName() << "'s turn." << endl << endl;
            cout << "Press Enter to roll the dice...";
            cin.get();

            // Announce the dice roll and move the player
            int step = rollDice();
            int newPosition = (currentPlayer.getPosition() + step) % board.getSize();
            currentPlayer.setPosition(newPosition);
            cout << currentPlayer.getName() << " rolled a " << step << "!" << endl << endl;

            // Trigger the onLand event for the tile the player landed on
            Tile& currentTile = board.getTile(currentPlayer.getPosition());
            currentTile.onLand(currentPlayer, *this, step);
            currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers;

            // Allow player to manage their properties in their turn
            currentPlayer.manageProperties(*this);

            cout << endl;
            cout << "Press Enter to continue...";
            cin.get();
            cout << endl;
        }
        round++;
    }
}


int Game::rollDice() {
    // For randomness in dice rolls
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dist(1, 6);

    int step = dist(gen) + dist(gen);

    return step;
}

void Game::showPlayers() const {
    cout << "Players:" << endl;
    for (const auto& player : players) {
        player.showPlayer();
    }
    cout << endl;
}

void Game::showBoard() const {
    // Formatting
    cout << "Board:" << endl;
    for (int i = 0; i < board.getSize() / 10; i++) {
        for (int j = 0; j < 10; j++) {
            int idx = i * 10 + j;
            cout << "[" << board.getTile(idx).getName();
            for (const auto& player : players) {
                if (player.getPosition() == idx) {
                    cout << " <" << player.getName() << ">";
                }
            }
            cout << "]";
            if (j != 9) {
                cout << "   ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

int Game::getAvailableBuildings(bool house) const {
    return house ? availableHouses : availableHotels;
}

void Game::modifyAvailableBuildings(bool house, int count) {
    if (house) {
        availableHouses += count;
        cout << "Available Houses: " << availableHouses << endl;
    } else {
        availableHotels += count;
        cout << "Available Hotels: " << availableHotels << endl;
    }
}