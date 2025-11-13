#include <iostream>
#include <string>
#include <random>

#include "Game.h"
#include "Player.h"
#include "Tile.h"

using namespace std;

Game::Game() : numPlayers(getNumPlayers()), board() {
    // -n if test player is used
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
        if (cin >> num && num >= 0 && num <= 4) {
            cin.ignore(10000, '\n');
            return num; // +n if test player is used
        }
        cout << "Invalid! Please enter 2, 3, or 4.\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    cout << endl;
}

void Game::addPlayer(string name) {
    players.emplace_back(Player(name, 0, 1500));
}

// For testing purposes, add a player with preset properties
/**
void Game::testAddPlayer(string name) {
    if (name == "Thanos") {
        players.emplace_back(Player(name, 0, 1500));
        Player& currentPlayer = players.back();
        int positions[] = {6, 8, 9};
        for (int pos : positions) {
            Tile& tile = board.getTile(pos);
            if (auto* property = dynamic_cast<PropertyTile*>(&tile)) {
                property->buyProperty(currentPlayer);
            }
        }
        for (int i = 0; i < 4; i++) {
            dynamic_cast<PropertyTile*>(&board.getTile(6))->buyBuilding(currentPlayer, *this);
            dynamic_cast<PropertyTile*>(&board.getTile(8))->buyBuilding(currentPlayer, *this);
            dynamic_cast<PropertyTile*>(&board.getTile(9))->buyBuilding(currentPlayer, *this);
        }
    }
    else if (name == "Kratos") {
        players.emplace_back(Player(name, 0, 120));
        Player& currentPlayer = players.back();
        int positions[] = {1, 3};    
        for (int pos : positions) {
            Tile& tile = board.getTile(pos);
            if (auto* property = dynamic_cast<PropertyTile*>(&tile)) {
                property->buyProperty(currentPlayer);
            }
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
**/

void Game::startGame() {
    this->showPlayers();
    this->showBoard();
    while (round < 5) {
        for (int i = 0; i < numPlayers; i++) {
            if (players[i].isBankrupt()) {
                currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers;
                continue;
            }
            if (bankruptcyCount == numPlayers - 1) {
                endGame(&players[i]);
                return;
            }
            cout << "---------------------------------------------------------------" << endl;
            cout << endl;

            // Ask player to roll the dice
            Player& currentPlayer = players[currentPlayerIndex];
            cout << "Round " << (round + 1) << ", " << currentPlayer.getName() << "'s turn." << endl << endl;
            showPlayers();
            showBoard();

            gameTurn(currentPlayer);
            currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers;
        }
        round++;
    }
    endGame();
}

void Game::gameTurn(Player& currentPlayer) {
    int jailStatus = currentPlayer.getJailStatus();
    if (jailStatus == -1) {
        normalTurn(currentPlayer, 0);
    }
    else {
        jailTurn(currentPlayer, jailStatus);
    }
}

void Game::normalTurn(Player& currentPlayer, int rolled_twelve) {
    int step = 0;
    char choice = 'Z';
    while (choice != 'R') {
        cout << "Type R to roll the dice." << endl;
        cout << "Type M if you would like to manage your properties." << endl << endl;
        choice = Game::getChoice('R', 'M');
        if (choice == 'R') {
            // Dice roll
            step = rollDice();
            cout << currentPlayer.getName() << " rolled a " << step << "!" << endl << endl;

            // Move the player
            int newPosition = (currentPlayer.getPosition() + step) % board.getSize();
            currentPlayer.setPosition(newPosition);

            // Send player to jail if they roll a 12 for the third time
            if (step == 12 && rolled_twelve == 2) {
                cout << currentPlayer.getName() << " rolled a 12 for the third time." << endl;
                cout << currentPlayer.getName() << " goes to jail." << endl << endl;
                currentPlayer.setJailStatus(0);
                currentPlayer.setPosition(10);
            }
            else {
                // Trigger the onLand event for the tile the player landed on
                Tile& currentTile = board.getTile(currentPlayer.getPosition());
                currentTile.onLand(currentPlayer, *this, step);
            }
        }
        else if (choice == 'M') {
            // Player manages their properties.
            currentPlayer.manageProperties(*this);
            cout << endl;
        }
    }

    if (currentPlayer.isBankrupt()) {
        return;
    }

    endTurn(currentPlayer);

    // Player rolls the dice again if they rolled a twelve.
    if (step == 12 && rolled_twelve < 2) {
        cout << "As " << currentPlayer.getName() << " rolled a 12, " << currentPlayer.getName() << " rolls the dice again." << endl << endl;
        normalTurn(currentPlayer, rolled_twelve + 1);
    }
}

void Game::jailTurn(Player& currentPlayer, int jailStatus) {
    /*
    Jail Turn Logic Explanation:
    ----------------------------
    When a player is in jail, they have up to 3 turns to try to get out.

    • On the 1st and 2nd turns (jailStatus < 2):
        - The player chooses between:
            'P' → Pay $50 immediately to get out and get to roll the dicee.
            'T' → Try to roll a 12 (double six). If successful, they are freed and move 12 on that turn, without additional dice rolls.
                  If not, they stay in jail until their next turn.
            'S' → Stay in jail.
            'M' → Allow player to raise money to get out of jail.

    • On the 3rd turn (jailStatus == 2):
        - The player is required to try and roll a 12.
        - If they succeed, they are freed and move a 12.
        - If they fail, they must pay $50 to get out, and continue with a normal turn.
        - If they don't have enough money, they'll be forced to raise money by selling or mortgaging property.
        - If they cannot do so, they are considered bankrupt.

    After paying or successfully rolling, the player’s jail status resets (set to -1)
    and they can proceed to roll/move normally.
    */
    cout << currentPlayer.getName() << " is in jail (" << jailStatus + 1 << ")." << endl << endl;
    if (jailStatus < 2) {
        bool chose = false;
        char choice = 'Z';
        while (!chose) {
            if (currentPlayer.getCash() >= 50) {
                cout << "Type P if you would like to pay $50 to get out of jail." << endl;
            }
            cout << "Type T if you would like to try to roll a 12 to get out jail" << endl;
            cout << "Type S if you would like to stay in jail" << endl;
            cout << "Type M if you would like to manage your properties." << endl << endl;

            // If a player does not have $50, they cannot choose to pay.
            if (currentPlayer.getCash() >= 50) {
                choice = Game::getChoice('P', 'T', 'S', 'M');
            }
            else {
                choice = Game::getChoice('T', 'S', 'M');
            }
            cout << endl;

            if (choice == 'P' || choice == 'T' || choice == 'S') {
                chose = true;
            }

            if (choice == 'P') {
                payToGetOutofJail(currentPlayer);
            } else if (choice == 'T') {
                rollToGetOutofJail(currentPlayer);
                endTurn(currentPlayer);
            } else if (choice == 'S') {
                currentPlayer.setJailStatus(currentPlayer.getJailStatus() + 1);
                endTurn(currentPlayer);
            } else {
                currentPlayer.manageProperties(*this);
                cout << endl;
            }
        }
    }
    else if (jailStatus == 2) {
        cout << "Press Enter to roll the dice.";
        cin.get();
        cout << endl;

        rollToGetOutofJail(currentPlayer);
        if (currentPlayer.getJailStatus() != -1) {
            if (currentPlayer.getCash() < 50) {
                cout << currentPlayer.getName() << " does not have enough money to pay $50." << endl;
                if (currentPlayer.calculateWealth() < 50) {
                    currentPlayer.declareBankruptcy(*this);
                    return;
                }
                cout << currentPlayer.getName() << " must raise money by selling or mortgaging properties." << endl << endl;
                currentPlayer.forceRaiseMoney(*this, 50);
            }
            cout << "Press Enter to pay $50 to get out of jail.";
            cin.get();
            cout << endl;
            payToGetOutofJail(currentPlayer);
        }
    }
}

void Game::rollToGetOutofJail(Player& currentPlayer) {
    // Dice roll
    int roll = rollDice();
    cout << currentPlayer.getName() << " rolled a " << roll << "!" << endl;
    if (roll == 12) {
        // Get player out of jail
        currentPlayer.setJailStatus(-1);
        cout << currentPlayer.getName() << " gets out from jail." << endl << endl;

        // Move the player
        int newPosition = (currentPlayer.getPosition() + roll) % board.getSize();
        currentPlayer.setPosition(newPosition);

        // Trigger the onLand event for the tile the player landed on
        Tile& currentTile = board.getTile(currentPlayer.getPosition());
        currentTile.onLand(currentPlayer, *this, roll);
        endTurn(currentPlayer);
    } else {
        currentPlayer.setJailStatus(currentPlayer.getJailStatus() + 1);
        cout << currentPlayer.getName() << " stays in jail." << endl << endl;
    }
}

void Game::payToGetOutofJail(Player& currentPlayer) {
    currentPlayer.deductMoney(50);
    currentPlayer.setJailStatus(-1);
    cout << currentPlayer.getName() << " paid $50 and got out from jail." << endl << endl;
    normalTurn(currentPlayer, 0);
}

void Game::endTurn(Player& currentPlayer) {
    char choice = 'Z';
    while (choice != 'C') {
        cout << "Type C to continue." << endl;
        cout << "Type M if you would like to manage your properties." << endl << endl;
        choice = Game::getChoice('C', 'M');
        if (choice == 'C') {
            cout << endl;
            return;
        }
        else if (choice == 'M') {
            // Player manages their properties.
            currentPlayer.manageProperties(*this);
            cout << endl;
        }
    }
}

void Game::endGame(Player* currentPlayer) {
    cout << "---------------------------------------------------------------" << endl;
    cout << endl;
    cout << "Game Over!" << endl << endl;
    if (currentPlayer != nullptr) {
        cout << currentPlayer->getName() << " is the winner!" << endl << endl;
    }
    else {
        Player* winner = nullptr;
        int highestWealth = -1;
        for (int i = 0; i < numPlayers; i++) {
            int wealth = players[i].calculateWealth();
            if (wealth > highestWealth && !players[i].isBankrupt()) {
                winner = &players[i];
                highestWealth = wealth;
            }
        }
        cout << winner->getName() << " is the winner!" << endl << endl;
    }
    showPlayers();
    showBoard();
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

char Game::getChoice(char a, char b, char c, char d) {
    char choice;
    while (true) {
        if (c == '\0' && d == '\0') {
            cout << "Enter " << a << " or " << b << ": " ;
        }
        else if (d == '\0') {
            cout << "Enter " << a << " or " << b << " or " << c << ": " ;
        }
        else {
            cout << "Enter " << a << " or " << b << " or " << c << " or " << d << ": " ;
        }
        cin >> choice;
        cin.ignore(10000, '\n');
        if (choice == a || choice == b || (c != '\0'  && choice == c) || (d != '\0'  && choice == d)) {
            break;
        }
        cout << "Invalid input. Please try again." << endl << endl;
        cin.clear();
    }
    return choice;
}

void Game::addBankruptcyCount() {
    bankruptcyCount++;
}