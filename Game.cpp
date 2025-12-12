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
    addPlayer("4");
    numPlayers = players.size();
}

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
                players[i].addMoney(amount);
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
                QString message = QString("Player %1 receives $%2 from %3")
                                      .arg(QString::fromStdString(receiver.getName()))
                                      .arg(amount)
                                      .arg(QString::fromStdString(players[i].getName()));
                emit gameLogMessage(message);
            }
        }
    }
}


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
