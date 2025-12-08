#include <string>
#include <random>
#include <QDebug>

#include "Game.h"
#include "Player.h"
#include "Tile.h"

using namespace std;

Game::Game(QObject* parent)
    : QObject(parent)
    , numPlayers(0)
    , board()
    , communityChestDeck("Community Chest")
    , chanceDeck("Chance")
{
    // 初始化玩家
    addPlayer("1");
    addPlayer("2");
    addPlayer("3");
    int positions_2[] = {6, 8, 9};
    for (int pos : positions_2) {
        Tile& tile = board.getTile(pos);
        if (auto* property = dynamic_cast<PropertyTile*>(&tile)) {
            property->buyProperty(players[2]);
        }
    }
    for (int pos : positions_2) {
        Tile& tile = board.getTile(pos);
        if (auto* property = dynamic_cast<PropertyTile*>(&tile)) {
            property->buyBuilding(players[2], *this);
        }
    }
    for (int pos : positions_2) {
        Tile& tile = board.getTile(pos);
        if (auto* property = dynamic_cast<PropertyTile*>(&tile)) {
            property->buyBuilding(players[2], *this);
        }
    }
    Card additionalOutOfJailCard = Card("Get Out of Jail Free. This card may be kept until needed or sold.", CardType::GET_OUT_OF_JAIL, 0, -1, 0, "", 0, 0, "Community Chest");
    players[2].addGetOutOfJailCard(&additionalOutOfJailCard);
    //testing on jailturn functionality
    //players[2].setJailStatus(0);
    int positions_0[] = {1};
    for (int pos : positions_0) {
        Tile& tile = board.getTile(pos);
        if (auto* property = dynamic_cast<PropertyTile*>(&tile)) {
            property->buyProperty(players[0]);
        }
    }
    numPlayers = players.size();
}

void Game::addPlayer(string name) {
    players.emplace_back(Player(name, 0, 1500));
    emit gameLogMessage(QString("Player %1 joined the game").arg(QString::fromStdString(name)));
}

void Game::startGame() {
    round = 1;
    currentPlayerIndex = 0;
    // update gamestate: waiting for start --> player turn
    currentState = GameState::PlayerTurn;
    emit gameLogMessage("Game started!");
    // start first player's turn
    startPlayerTurn();
    emit roundLabelChanged();
}

void Game::startPlayerTurn() {
    if (currentState != GameState::PlayerTurn) return;

    Player& currentPlayer = players[currentPlayerIndex];

    // 检查玩家是否破产
    if (currentPlayer.isBankrupt()) {
        emit gameLogMessage(QString("%1 is bankrupt, skipping turn")
                                .arg(QString::fromStdString(currentPlayer.getName())));
        endTurn();
        return;
    }

    // 检查游戏是否结束
    if (bankruptcyCount >= numPlayers - 1) {
        endGame();
        return;
    }

    emit playerTurnStarted(currentPlayerIndex);
    // go the mainwindow.cpp and wait for player's input
}

void Game::rollDiceAndMoveAndProcessEvent() {
    if (currentState != GameState::PlayerTurn){};
    Player& currentPlayer = players[currentPlayerIndex];

    // First of all, roll the dice
    int diceValue = 8;

    if(diceValue == 12 && currentPlayer.getRolledTwelve()==2){
        int fromIndex = currentPlayer.getPosition();
        currentPlayer.setJailStatus(0);
        currentPlayer.setPosition(10);
        currentPlayer.resetRolledTwelve();
        emit playerMoved(currentPlayerIndex,fromIndex,10);
        emit enableEndTurnAndDisableRoll();
        emit playerTurnEnded(currentPlayerIndex);
        //
    }else{
        // update the gamelog
        emit gameLogMessage(QString("%1 rolled %2")
                                .arg(QString::fromStdString(currentPlayer.getName()))
                                .arg(diceValue));

        // Secondly, move the player
        movePlayer(currentPlayerIndex, diceValue);

        // Thirdly, trigger the onLand event for the tile the player landed on
        Tile& currentTile = board.getTile(currentPlayer.getPosition());
        currentTile.onLand(currentPlayer, *this, diceValue);
        // from onLand in tile.cpp, we connect to the slots in ui part
        currentState = GameState::PlayerTurn;

        if (diceValue == 12){
            currentPlayer.increaseRolledTwelve();
            //下一轮还是这个玩家
            emit playerTurnStarted(currentPlayerIndex);
        }else if (diceValue != 12){
            //清空这个玩家的12值
            currentPlayer.resetRolledTwelve();
            // next player's turn
            emit playerTurnEnded(currentPlayerIndex);
        }
    }
}

/*void Game::movePlayer(Player& currentPlayer, int step) {
    // Move the player
    int newPosition = (currentPlayer.getPosition() + step) % board.getSize();
    currentPlayer.setPosition(newPosition);

    // Trigger the onLand event for the tile the player landed on
    //Tile& currentTile = board.getTile(currentPlayer.getPosition());
    //currentTile.onLand(currentPlayer, *this, step);
}*/

void Game::movePlayer(int playerIndex, int steps) {
    Player& player = players[playerIndex];
    int oldPosition = player.getPosition();
    int newPosition = (oldPosition + steps) % board.getSize();

    player.setPosition(newPosition);

    // update player's location on ui's board
    emit playerMoved(playerIndex, oldPosition, newPosition);

    // update the game log
    emit gameLogMessage(QString("Player %1 moved from position %2 to %3")
                            .arg(QString::fromStdString(player.getName()))
                            .arg(oldPosition)
                            .arg(newPosition));
}

void Game::purchaseCurrentProperty() {
    Player& currentPlayer = players[currentPlayerIndex];
    int position = currentPlayer.getPosition();
    Tile& tile = board.getTile(position);

    if (auto* property = dynamic_cast<PropertyTile*>(&tile)) {
        // reuse the logic part in tile.cpp
        property->buyProperty(currentPlayer);
    }
}

void Game::payRent(int fromPlayer, int toPlayer, int rent){
    Player& from = players[fromPlayer];
    Player& to = players[toPlayer];
    from.deductMoney(rent);
    emit moneyChanged();
    to.addMoney(rent);
    emit gameLogMessage(QString::fromStdString("Player %1 pays %2 to player %3").arg(fromPlayer+1).arg(rent).arg(toPlayer+1));
}

void Game::endTurn() {
    // 切换到下一个玩家
    currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers;

    // 如果一轮结束
    if (currentPlayerIndex == 0) {
        round++;
        emit roundLabelChanged();
        // 检查游戏是否结束
        // 后续要检查这里检查的逻辑
        if (round > 50 || bankruptcyCount >= numPlayers - 1) {
            endGame();
            return;
        }
    }
    // 开始下一个玩家的回合
    startPlayerTurn();
}

void Game::endGame() {
    currentState = GameState::GameOver;

    // 找出赢家
    int winnerIndex = 0;
    int highestWealth = -1;
    for (int i = 0; i < numPlayers; i++) {
        if (!players[i].isBankrupt()) {
            int wealth = players[i].calculateWealth();
            if (wealth > highestWealth) {
                highestWealth = wealth;
                winnerIndex = i;
            }
        }
    }

    emit gameOver(winnerIndex);
    emit gameLogMessage(QString("Game Over! Player %1 wins!")
                            .arg(QString::fromStdString(players[winnerIndex].getName())));
}

void Game::processJailAction(JailAction action) {
    Player& currentPlayer = players[currentPlayerIndex];
    int jailStatus = currentPlayer.getJailStatus();
    switch (action) {
    case JailAction::UseCard:
        if (currentPlayer.hasGetOutOfJailCard()) {
            useGetOutOfJailCard(currentPlayerIndex);
        }
        break;

    case JailAction::PayBail:
        currentPlayer.deductMoney(50);
        emit moneyChanged();
        currentPlayer.setJailStatus(-1);
        emit gameLogMessage(QString("Player %1 paid $50 to get out of jail").arg(currentPlayerIndex+1));
        //玩家可以继续正常掷色子
        emit playerTurnStarted(currentPlayerIndex);
        break;

    case JailAction::RollForFreedom: {
        int roll = rollDice();

        if (roll == 12) {
            currentPlayer.setJailStatus(-1);
            emit gameLogMessage(QString("Player %1 rolled 12 and got out of jail!").arg(currentPlayerIndex+1));
            // 移动玩家并触发格子功能
            movePlayer(currentPlayerIndex, 12);
            Tile& currentTile = board.getTile(currentPlayer.getPosition());
            currentTile.onLand(currentPlayer,*this,12);
            //不需要发出信号，因为一旦发出playerTurnStarted信号，玩家就可以再次掷色子了
        } else {
            currentPlayer.setJailStatus(jailStatus + 1);
            emit gameLogMessage(QString("Player %1 rolled %2 and stays in jail")
                                    .arg(currentPlayerIndex+1)
                                    .arg(roll));
            //玩家自己点击end turn button结束自己的轮次
        }
        break;
    }

    case JailAction::StayInJail:
        currentPlayer.setJailStatus(jailStatus + 1);
        emit gameLogMessage(QString("Player %1 stays in jail")
                                .arg(currentPlayerIndex+1));
        //玩家自己点击endTurn button
        break;
    }
}

bool Game::canRollDice() const {
    if (currentState != GameState::PlayerTurn) {
        return false;
    }
    const Player& currentPlayer = players[currentPlayerIndex];
    return !currentPlayer.isBankrupt() && currentPlayer.getJailStatus() == -1;
}

bool Game::canEndTurn() const {
    return currentState == GameState::PlayerTurn;
}

/*bool Game::isPlayerInJail(int playerIndex) const {
    return players[playerIndex].getJailStatus() != -1;
}*/

bool Game::isGameOver() const {
    return currentState == GameState::GameOver;
}

// 辅助函数：根据玩家引用获取索引
int Game::getPlayerIndex(const Player& player) const {
    for (int i = 0; i < players.size(); ++i) {
        if (&players[i] == &player) {
            return i;
        }
    }
    return -1;
}


std::vector<PropertyTile*> Game::getPropertiesByGroup(const std::string& group) {
    std::vector<PropertyTile*> result;

    Board& board = getBoard();
    int totalTiles = board.getSize();   // 40

    for (int i = 0; i < totalTiles; i++) {

        Tile& tile = board.getTile(i);

        // 检查 tile 是否为 PropertyTile
        PropertyTile* prop = dynamic_cast<PropertyTile*>(&tile);
        if (!prop) continue;

        // 匹配颜色组
        if (prop->getGroup() == group) {
            result.push_back(prop);
        }
    }

    return result;
}




void Game::useGetOutOfJailCard(int playerIndex) {
    Player& player = players[playerIndex];
    Card* card = player.takeOutGetOutOfJailCard();
    if (card != nullptr) {
        player.setJailStatus(-1);
        emit gameLogMessage(QString("Player %1 used a Get Out of Jail Free card").arg(playerIndex+1));
        if (card->sourceDeck == "Chance") {
            chanceDeck.returnCard(*card);
        } else if (card->sourceDeck == "Community Chest") {
            communityChestDeck.returnCard(*card);
        }
        //玩家之后可以正常掷色子
        emit playerTurnStarted(playerIndex);
    } else {
        emit gameLogMessage(QString("Player %1 does not have a Get Out of Jail Free card").arg(playerIndex+1));
    }
}


/*void Game::endTurn(Player& currentPlayer) {
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
}*/

/*void Game::gameTurn(Player& currentPlayer) {
    int jailStatus = currentPlayer.getJailStatus();
    if (jailStatus == -1) {
        normalTurn(currentPlayer, 0);
    }
    else {
        jailTurn(currentPlayer, jailStatus);
    }
}*/

/*void Game::normalTurn(Player& currentPlayer, int rolled_twelve) {
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
}*/

/*void Game::jailTurn(Player& currentPlayer, int jailStatus) {

    // Jail Turn Logic Explanation:
    // ----------------------------
    // When a player is in jail, they have up to 3 turns to try to get out.

    // • On the 1st and 2nd turns (jailStatus < 2):
    //     - The player chooses between:
    //         'C' → Use a "Get out of Jail for Free" card if they have one.
    //         'P' → Pay $50 immediately to get out and get to roll the dice.
    //         'T' → Try to roll a 12 (double six). If successful, they are freed and move 12 on that turn, without additional dice rolls.
    //               If not, they stay in jail until their next turn.
    //         'S' → Stay in jail.
    //         'M' → Allow player to raise money to get out of jail.

    // • On the 3rd turn (jailStatus == 2):
    //     - The player is required to try and roll a 12.
    //     - If they succeed, they are freed and move a 12.
    //     - If they fail, they must pay $50 or use a "Get out of Jail for Free" card to get out, and continue with a normal turn.
    //     - If they don't have enough money, they'll be forced to raise money by selling or mortgaging property.
    //     - If they cannot do so, they are considered bankrupt.

    // After paying or successfully rolling, the player’s jail status resets (set to -1)
    // and they can proceed to roll/move normally.

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
*/

/*void Game::rollToGetOutofJail(Player& currentPlayer) {
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
}*/

/*void Game::useGetOutOfJailCard(Player& player) {
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
}*/

/*void Game::payToGetOutofJail(Player& currentPlayer) {
    currentPlayer.deductMoney(50);
    currentPlayer.setJailStatus(-1);
    cout << currentPlayer.getName() << " paid $50 and got out from jail." << endl << endl;
    normalTurn(currentPlayer, 0);
}*/

/*void Game::endGame(Player* currentPlayer) {
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
}*/

/*bool Game::playerCanPay(Player& payer, int amount, Player* receiver) {
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
}*/

bool Game::playerCanPay(int payerIndex, int amount, int receiverIndex) {
    Player& payer = players[payerIndex];

    if (payer.getCash() < amount) {
        QString message = QString("Player %1 does not have enough money to pay $%2")
        .arg(QString::fromStdString(payer.getName()))
            .arg(amount);
        emit gameLogMessage(message);

        if (payer.calculateWealth() < amount) {
            Player* receiver = (receiverIndex >= 0) ? &players[receiverIndex] : nullptr;
            payer.declareBankruptcy(*this, receiver);
            bankruptcyCount++;
            emit playerBankrupt(payerIndex);
            return false;
        }

        emit forceRaiseMoney(payerIndex, amount);
        return true;
    }
    return true;
}


/*void Game::payToPlayers(Player& payer, int amount) {
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
}*/

void Game::payToPlayers(int payerIndex, int amount) {
    if (payerIndex < 0 || payerIndex >= players.size()) return;

    Player& payer = players[payerIndex];

    // 先检查是否能支付
    if (!playerCanPay(payerIndex, amount)) {
        return;
    }

    for (int i = 0; i < players.size(); ++i) {
        if (i != payerIndex && !players[i].isBankrupt()) {
            if (playerCanPay(payerIndex, amount, i)) {
                payer.deductMoney(amount);
                emit moneyChanged();
                players[i].addMoney(amount);
                emit moneyChanged();
                QString message = QString("%1 pays $%2 to %3")
                                      .arg(QString::fromStdString(payer.getName()))
                                      .arg(amount)
                                      .arg(QString::fromStdString(players[i].getName()));
                emit gameLogMessage(message);
            }
        }
    }
}

void Game::receiveFromPlayers(int receiverIndex, int amount) {
    if (receiverIndex < 0 || receiverIndex >= players.size()) return;

    Player& receiver = players[receiverIndex];

    for (int i = 0; i < players.size(); ++i) {
        if (i != receiverIndex && !players[i].isBankrupt()) {
            if (playerCanPay(i, amount, receiverIndex)) {
                players[i].deductMoney(amount);
                receiver.addMoney(amount);
                emit moneyChanged();
                QString message = QString("Player %1 receives $%2 from %3")
                                      .arg(QString::fromStdString(receiver.getName()))
                                      .arg(amount)
                                      .arg(QString::fromStdString(players[i].getName()));
                emit gameLogMessage(message);
            }
        }
    }
}

/*void Game::receiveFromPlayers(Player& receiver, int amount) {
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
}*/

int Game::getAvailableBuildings(bool house) const {
    return house ? availableHouses : availableHotels;
}

void Game::modifyAvailableBuildings(bool house, int count) {
    if (house) {
        availableHouses += count;

        // 这里需要传递信号嘛？

        emit gameLogMessage(QString("Available Houses: %1").arg(availableHouses));
    } else {
        availableHotels += count;
        emit gameLogMessage(QString("Available Hotels: %1").arg(availableHotels));
    }
}

/*void Game::modifyAvailableBuildings(bool house, int count) {
    if (house) {
        availableHouses += count;
        cout << "Available Houses: " << availableHouses << endl;
    } else {
        availableHotels += count;
        cout << "Available Hotels: " << availableHotels << endl;
    }
}*/

void Game::drawChanceCard(int playerIndex) {
    Player& player = players[playerIndex];
    Card card = chanceDeck.draw();

    QString message = QString("%1 draws Chance card: %2")
                          .arg(QString::fromStdString(player.getName()), QString::fromStdString(card.description));
    emit gameLogMessage(message);
    emit openChanceCard(playerIndex,QString::fromStdString(card.description));

    //execute the logic part
    card.execute(player, *this);
    if (card.type != CardType::GET_OUT_OF_JAIL) {
        chanceDeck.returnCard(card);
    }
}

void Game::drawCommunityChestCard(int playerIndex) {
    Player& player = players[playerIndex];
    Card card = communityChestDeck.draw();

    QString message = QString("%1 draws Community Chest card: %2").arg(player.getName(), card.description);
    //emit cardDrawn(message);
    emit gameLogMessage(message);
    emit openCommunityCard(playerIndex,QString::fromStdString(card.description));

    card.execute(player, *this);
    if (card.type != CardType::GET_OUT_OF_JAIL) {
        communityChestDeck.returnCard(card);
    }
}


// 交易相关函数需要大幅修改
// void Game::proposeTrade(int fromPlayer, int toPlayer, int propertyIndex, int amount) {
//     // 发出交易提议信号，让UI处理
//     emit tradeProposed(fromPlayer, toPlayer, propertyIndex, amount);
// }

// void Game::acceptTrade(int tradeId) {
//     // 执行交易逻辑
//     emit gameLogMessage("Trade accepted");
// }

// void Game::rejectTrade(int tradeId) {
//     emit gameLogMessage("Trade rejected");
// }

/*void Game::manageSellTrade(Player& offeringPlayer, int propertyIndex) {
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
}*/

/*void Game::manageBuyTrade(Player& offeringPlayer, int propertyIndex) {
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
}*/

/*bool Game::validateTradePlayers(Player* offeringPlayer, Player* targetPlayer, int propertyIndex, bool offerBuy) {
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
}*/

/*void Game::askTradeDecision(Player* offeringPlayer, Player* targetPlayer, int amount, int propertyIndex, PropertyTile* property, bool offerBuy) {
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
}*/

/*void Game::executeTrade(Player* buyingPlayer, Player* sellingPlayer, int amount, int propertyIndex, PropertyTile* property) {
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
*/

void Game::executeTrade(Player* buyingPlayer, Player* sellingPlayer, int amount, bool card, PropertyTile* property) {
    buyingPlayer->deductMoney(amount);
    sellingPlayer->addMoney(amount);
    if (card) {
        Card* card = sellingPlayer->takeOutGetOutOfJailCard();
        buyingPlayer->addGetOutOfJailCard(card);
        QString message = QString("Trade completed! Player %1 acquired a <b>Get Out of Jail for Free</b> card from %2 for $%3.")
                              .arg(buyingPlayer->getName()).arg(sellingPlayer->getName()).arg(amount);

        emit gameLogMessage(message);
    }
    else {
        property->transferOwnership(*sellingPlayer, buyingPlayer, *this);
        QString message = QString("Trade completed! Player %1 acquired a %2 from %2 for $%3.")
                              .arg(buyingPlayer->getName()).arg(property->getName()).arg(sellingPlayer->getName()).arg(amount);

        emit gameLogMessage(message);
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
