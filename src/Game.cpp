#include <iostream>
#include <string>
#include <random>

#include "Game.h"
#include "CardDeck.h"
#include "Player.h"
#include "Tile.h"

using namespace std;

Game::Game() : numPlayers(getNumPlayers()), board(), communityChestDeck("Community Chest"), chanceDeck("Chance") {
    players.reserve(numPlayers);
    // -n if test player is used
    for (int i = 0; i < numPlayers; i++) {
        string name;
        cout << "Enter name for player " << (i + 1) << ": ";
        getline(cin, name);
        addPlayer(name);
    }
    cout << endl;
}

//use this constructor for m_game if testing ui
// Game::Game(): numPlayers(0),board(),communityChestDeck("Community Chest"),chanceDeck("Chance") {
//     addPlayer("Player 1");
//     addPlayer("Player 2");
// }

int Game::getNumPlayers() {
    int num;
    while (true) {
        cout << "Enter number of players: ";
        if (cin >> num && num >= 2 && num <= 4) {
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
    players.emplace_back(Player(name, 0, 500));
}

// For testing purposes, add a player with preset properties
/**
void Game::testAddPlayer(string name) {
    if (name == "A") {
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
    else if (name == "B") {
        players.emplace_back(Player(name, 0, 220));
        Player& currentPlayer = players.back();
        int positions[] = {1, 3};    
        for (int pos : positions) {
            Tile& tile = board.getTile(pos);
            if (auto* property = dynamic_cast<PropertyTile*>(&tile)) {
                property->buyProperty(currentPlayer);
            }
        }
        currentPlayer.addGetOutOfJailCard(new Card("Get Out of Jail Free. This card may be kept until needed or sold.", CardType::GET_OUT_OF_JAIL, 0, -1, 0, "", 0, 0, "Chance"));
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
            int step = rollDice();
            cout << currentPlayer.getName() << " rolled a " << step << "!" << endl << endl;

            // Send player to jail if they roll a 12 for the third time
            if (step == 12 && rolled_twelve == 2) {
                cout << currentPlayer.getName() << " rolled a 12 for the third time." << endl;
                cout << currentPlayer.getName() << " goes to jail." << endl << endl;
                currentPlayer.setJailStatus(0);
                currentPlayer.setPosition(10);
            }
            else {
                movePlayer(currentPlayer, step);
            }
        }
        else if (choice == 'M') {
            // Player manages their properties.
            currentPlayer.manageProperties(*this);
            cout << endl;
        }
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
            'C' → Use a "Get out of Jail for Free" card if they have one.
            'P' → Pay $50 immediately to get out and get to roll the dice.
            'T' → Try to roll a 12 (double six). If successful, they are freed and move 12 on that turn, without additional dice rolls.
                  If not, they stay in jail until their next turn.
            'S' → Stay in jail.
            'M' → Allow player to raise money to get out of jail.

    • On the 3rd turn (jailStatus == 2):
        - The player is required to try and roll a 12.
        - If they succeed, they are freed and move a 12.
        - If they fail, they must pay $50 or use a "Get out of Jail for Free" card to get out, and continue with a normal turn.
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
            // Player's action options while in jail
            if (currentPlayer.hasGetOutOfJailCard()) {
                cout << "Type C if you would like to use a Get Out of Jail Free card." << endl;
            }
            if (currentPlayer.getCash() >= 50) {
                cout << "Type P if you would like to pay $50 to get out of jail." << endl;
            }
            cout << "Type T if you would like to try to roll a 12 to get out jail" << endl;
            cout << "Type S if you would like to stay in jail" << endl;
            cout << "Type M if you would like to manage your properties." << endl << endl;

            // Get player's choice
            if (currentPlayer.hasGetOutOfJailCard() && currentPlayer.getCash() >= 50) {
                choice = Game::getChoice('C', 'P', 'T', 'S', 'M');
            }
            else if (currentPlayer.hasGetOutOfJailCard()) {
                choice = Game::getChoice('C', 'T', 'S', 'M');
            }
            else if (currentPlayer.getCash() >= 50) {
                choice = Game::getChoice('P', 'T', 'S', 'M');
            }
            else {
                choice = Game::getChoice('T', 'S', 'M');
            }
            cout << endl;

            if (choice == 'C' || choice == 'P' || choice == 'T' || choice == 'S') {
                chose = true;
            }

            // Execute player's choice
            if (choice == 'C') {
                useGetOutOfJailCard(currentPlayer);
            }
            else if (choice == 'P') {
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

        // Player must first attempt to roll a 12.
        rollToGetOutofJail(currentPlayer);
        if (currentPlayer.getJailStatus() != -1) {
            if (currentPlayer.hasGetOutOfJailCard()) {
                bool chose = false;
                char choice = 'Z';
                while (!chose) {
                    // Player's action options while in third turn in jail
                    cout << "Type C if you would like to use a Get Out of Jail Free card." << endl;
                    if (currentPlayer.getCash() >= 50) {
                        cout << "Type P if you would like to pay $50 to get out of jail." << endl;
                    }
                    cout << "Type M if you would like to manage your properties." << endl << endl;

                    // Get player's choice
                    if (currentPlayer.getCash() >= 50) {
                        choice = Game::getChoice('C', 'P', 'M');
                    }
                    else {
                        choice = Game::getChoice('C', 'M');
                    }
                    cout << endl;

                    if (choice == 'C' || choice == 'P') {
                        chose = true;
                    }

                    // Execute player's choice
                    if (choice == 'C') {
                        useGetOutOfJailCard(currentPlayer);
                    }
                    else if (choice == 'P') {
                        payToGetOutofJail(currentPlayer);
                    }
                    else {
                        currentPlayer.manageProperties(*this);
                        cout << endl;
                    }
                }
            }
            else {
                if (playerCanPay(currentPlayer, 50)) {
                    cout << "Press Enter to pay $50 to get out of jail.";
                    cin.get();
                    cout << endl;
                    payToGetOutofJail(currentPlayer);
                }
            }
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

        movePlayer(currentPlayer, 12);
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
    // In the end of turn, player can choose to finish their turn or manage their properties.
    if (currentPlayer.isBankrupt()) {
        return;
    }
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

void Game::movePlayer(Player& currentPlayer, int step) {
    // Move the player
    int newPosition = (currentPlayer.getPosition() + step) % board.getSize();
    currentPlayer.setPosition(newPosition);

    // Trigger the onLand event for the tile the player landed on
    Tile& currentTile = board.getTile(currentPlayer.getPosition());
    currentTile.onLand(currentPlayer, *this, step);
}

bool Game::playerCanPay(Player& payer, int amount, Player* receiver) {
    if (payer.getCash() < amount) {
        cout << payer.getName() << " does not have enough money to pay $" << amount << "." << endl;
        if (payer.calculateWealth() < amount) {
            payer.declareBankruptcy(*this, receiver);
            bankruptcyCount++;
            return false;
        }
        payer.forceRaiseMoney(*this, amount);
    }
    return true;
}

void Game::payToPlayers(Player& payer, int amount) {
    for (auto& player : players) {
        if (&player != &payer && !player.isBankrupt()) {
            if (playerCanPay(payer, amount, &player)) {
                payer.deductMoney(amount);
                player.addMoney(amount);
                cout << payer.getName() << " pays $" << amount << " to " << player.getName() << "." << endl;
            }
        }
    }
    cout << endl;
}

void Game::receiveFromPlayers(Player& receiver, int amount) {
    for (auto& player : players) {
        if (&player != &receiver && !player.isBankrupt()) {
            if (playerCanPay(player, amount, &receiver)) {
                player.deductMoney(amount);
                receiver.addMoney(amount);
                cout << receiver.getName() << " receives $" << amount << " from " << player.getName() << "." << endl;
            }
        }
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

void Game::drawChanceCard(Player& player) {
    cout << "Press Enter to draw a Chance card...";
    cin.get();
    cout << endl;
    Card card = chanceDeck.draw();
    card.execute(player, *this);
    if (card.type != CardType::GET_OUT_OF_JAIL) {
        chanceDeck.returnCard(card);
    }
}

void Game::drawCommunityChestCard(Player& player) {
    cout << "Press Enter to draw a Community Chest card...";
    cin.get();
    cout << endl;
    Card card = communityChestDeck.draw();
    card.execute(player, *this);
    if (card.type != CardType::GET_OUT_OF_JAIL) {
        communityChestDeck.returnCard(card);
    }
}

void Game::useGetOutOfJailCard(Player& player) {
    Card* card = player.takeOutGetOutOfJailCard();
    if (card != nullptr) {
        player.setJailStatus(-1);
        cout << player.getName() << " used a Get Out of Jail Free card and got out from jail." << endl << endl;

        if (card->sourceDeck == "Chance") {
            chanceDeck.returnCard(*card);
        }
        else if (card->sourceDeck == "Community Chest") {
            communityChestDeck.returnCard(*card);
        }
        normalTurn(player, 0);
    } else {
        cout << player.getName() << " does not have a Get Out of Jail Free card." << endl << endl;
    }
}

void Game::manageSellTrade(Player& offeringPlayer, int propertyIndex) {
    // Check if offering player can sell their property.
    PropertyTile* property = nullptr;
    if (propertyIndex < 40) {
        Tile& tile =  board.getTile(propertyIndex);
        property = dynamic_cast<PropertyTile*>(&tile);
        if (property) {
            if (property->getOwner() != &offeringPlayer) {
                cout << "You do not own " << property->getName() << "." << endl;
                return;
            }
            if (property->getHouses() > 0) {
                cout << "You cannot trade a property that has buildings." << endl;
                return;
            }
        }
        else {
            cout << "Index " << propertyIndex << " is not a property." << endl;
            return;
        }
    }

    Player* targetPlayer = nullptr;
    int targetIndex, amount;

    cout << endl;
    showPlayers();

    offeringPlayer.getTradeDetails(targetIndex, amount);

    // Determine target player
    if (targetIndex < 0 || targetIndex >= numPlayers) {
        cout << "Invalid player index. Please try again." << endl;
        return;
    }
    targetPlayer = &players[targetIndex];

    if (!validateTradePlayers(&offeringPlayer, targetPlayer, propertyIndex, false)) {
        return;
    }

    // Validate trade amount
    if (amount > targetPlayer->getCash()) {
        cout << targetPlayer->getName() << " does not own $" << amount << " to trade." << endl;
        return;
    }
    if (amount < 0) {
        cout << "Amount cannot be negative. Please try again." << endl;
        return; 
    }

    askTradeDecision(&offeringPlayer, targetPlayer, amount, propertyIndex, property, false);
}

void Game::manageBuyTrade(Player& offeringPlayer, int propertyIndex) {
    // Check if the property can be bought from other player.
    PropertyTile* property = nullptr;
    if (propertyIndex < 40) {
        Tile& tile =  board.getTile(propertyIndex);
        property = dynamic_cast<PropertyTile*>(&tile);
        if (property) {
            if (property->getOwner() == nullptr) {
                cout << property->getName() << " is not owned by anyone." << endl;
                return;
            }
            if (property->getOwner() == &offeringPlayer) {
                cout << "You already own the property." << endl;
                return;
            }
            if (property->getHouses() > 0) {
                cout << "You cannot trade a property that has buildings." << endl;
                return;
            }
        }
        else {
            cout << "Index " << propertyIndex << " is not a property." << endl;
            return;
        }
    }

    // Determine target player
    Player* targetPlayer = nullptr;
    int amount, targetIndex;

    cout << endl;
    showPlayers();

    offeringPlayer.getTradeDetails(targetIndex, amount, (propertyIndex < 40));

    // Determine target player
    if (propertyIndex == 40) {
        if (targetIndex < 0 || targetIndex >= numPlayers) {
            cout << "Invalid player index. Please try again." << endl;
            return;
        }
        targetPlayer = &players[targetIndex];
    }
    else {
        targetPlayer = property->getOwner();
    }

    if (!validateTradePlayers(&offeringPlayer, targetPlayer, propertyIndex, true)) {
        return;
    }

    // Validate trade amount
    if (amount > offeringPlayer.getCash()) {
        cout << "You do not own $" << amount << " to trade." << endl;
        return;
    }
    if (amount < 0) {
        cout << "Amount cannot be negative. Please try again." << endl;
        return; 
    }

    // Propose trade to target player
    askTradeDecision(&offeringPlayer, targetPlayer, amount, propertyIndex, property, true);
}

bool Game::validateTradePlayers(Player* offeringPlayer, Player* targetPlayer, int propertyIndex, bool offerBuy) {
    if (targetPlayer == offeringPlayer) {
        cout << "You cannot trade with yourself. Please try again." << endl;
        return false;
    }
    if (targetPlayer->isBankrupt()) {
        cout << targetPlayer->getName() << " is bankrupt. Cannot trade with them." << endl;
        return false;
    }
    if (propertyIndex == 40) {
        if (offerBuy && !targetPlayer->hasGetOutOfJailCard()) {
            cout << targetPlayer->getName() << " does not own a 'Get out of Jail for Free' card." << endl;
            return false;
        }
        else if (!offerBuy && !offeringPlayer->hasGetOutOfJailCard()) {
            cout << "You do not own a 'Get out of Jail for Free' card." << endl;
            return false;
        }
    }
    return true;
}

void Game::askTradeDecision(Player* offeringPlayer, Player* targetPlayer, int amount, int propertyIndex, PropertyTile* property, bool offerBuy) {
    cout << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << endl;

    cout << targetPlayer->getName() << "'s Screen" << endl << endl;

    if (propertyIndex == 40) {
        cout << offeringPlayer->getName() << " is proposing to trade a Get Out of Jail Free card for $" << amount << "." << endl;
    } else {
        if (property->isMortgaged()) {
            cout << "Note: " << property->getName() << " is mortgaged." << endl; 
            amount += static_cast<int>((property->getPrice() / 2) * 1.1);
        }
        cout << offeringPlayer->getName() << " is proposing to trade " << property->getName() << " for $" << amount << "." << endl;
    }
    cout << "Type Y to accept the trade or N to decline: " << endl;;
    char tradeChoice = Game::getChoice('Y', 'N');

    cout << endl;
    if (tradeChoice == 'Y') {
        if (offerBuy) {
            executeTrade(offeringPlayer, targetPlayer, amount, propertyIndex, property);
        }
        else {
            executeTrade(targetPlayer, offeringPlayer, amount, propertyIndex, property);
        }
    } else {
        cout << targetPlayer->getName() << " declined the trade." << endl;
    }

    cout << endl;
    cout << "---------------------------------------------------------------" << endl;
}

void Game::executeTrade(Player* buyingPlayer, Player* sellingPlayer, int amount, int propertyIndex, PropertyTile* property) {
    if (buyingPlayer->getCash() < amount) {
        cout << buyingPlayer->getName() << " does not have $" << amount << " to trade." << endl;
        return;
    }
    buyingPlayer->deductMoney(amount);
    sellingPlayer->addMoney(amount);
    if (propertyIndex == 40) {
        Card* card = sellingPlayer->takeOutGetOutOfJailCard();
        buyingPlayer->addGetOutOfJailCard(card);
        cout << "Trade completed! " << buyingPlayer->getName() << " acquired a Get Out of Jail Free card from " << sellingPlayer->getName() << " for $" << amount << "." << endl;
    }
    else {
        property->transferOwnership(*sellingPlayer, buyingPlayer);
        cout << "Trade completed! " << buyingPlayer->getName() << " acquired " << property->getName() << " from " << sellingPlayer->getName() << " for $" << amount << "." << endl;
    }
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

char Game::getChoice(char a, char b, char c, char d, char e) {
    char choice;
    while (true) {
        if (c == '\0' && d == '\0' && e == '\0') {
            cout << "Enter " << a << " or " << b << ": " ;
        }
        else if (d == '\0' && e == '\0') {
            cout << "Enter " << a << " or " << b << " or " << c << ": " ;
        }
        else if (e == '\0') {
            cout << "Enter " << a << " or " << b << " or " << c << " or " << d << ": " ;
        }
        else {
            cout << "Enter " << a << " or " << b << " or " << c << " or " << d << " or " << e << ": " ;
        }
        cin >> choice;
        cin.ignore(10000, '\n');
        if (choice == a || choice == b || (c != '\0'  && choice == c) || (d != '\0'  && choice == d) || (e != '\0'  && choice == e)) {
            break;
        }
        cout << "Invalid input. Please try again." << endl << endl;
        cin.clear();
    }
    return choice;
}
