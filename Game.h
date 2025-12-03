#ifndef GAME_H
#define GAME_H

#include <vector>
#include <functional>
#include <QObject>

#include "Board.h"
#include "Player.h"
#include "CardDeck.h"

// 定义游戏状态枚举
enum class GameState {
    WaitingForStart,
    PlayerTurn,
    Moving,
    ProcessingEvent,
    GameOver
};

// 定义监狱操作枚举
enum class JailAction {
    UseCard,
    PayBail,
    RollForFreedom,
    StayInJail
};

class Game : public QObject{

    Q_OBJECT

private:
    Board board;
    int numPlayers;
    std::vector<Player> players;
    int round = 0;
    int currentPlayerIndex = 0;
    int availableHouses = 32;
    int availableHotels = 12;
    int bankruptcyCount = 0;
    CardDeck communityChestDeck;
    CardDeck chanceDeck;

    GameState currentState = GameState::WaitingForStart;

    using PurchaseCallback = std::function<bool(const std::string&, int, const std::string&)>;
    PurchaseCallback m_purchaseCallback;


public:
    explicit Game(QObject* parent = nullptr);

    // 玩家管理
    void addPlayer(std::string name);

    // 游戏流程控制 - 完全重写
    void startGame();
    void startPlayerTurn();
    void endTurn();
    void rollDiceAndMoveAndProcessEvent();
    void processJailAction(JailAction action);
    void triggerCurrentTileEvent();
    void endGame();

    // 查询方法
    int getPlayerCount() const { return players.size(); }

    //购买房产
    void purchaseCurrentProperty();
    //支付租金
    void payRent(int fromPlayer, int toPlayer, int rent);

    int getPlayerIndex(const Player& player) const;
    Player& getPlayer(int index) { return players[index]; }
    const Player& getPlayer(int index) const { return players[index]; }
    int getCurrentPlayerIndex() const { return currentPlayerIndex; }
    int getCurrentRound() const { return round; }
    GameState getCurrentState() const { return currentState; }
    Board& getBoard() { return board; }
    const Board& getBoard() const { return board; }


    // 状态检查
    bool canRollDice() const;
    bool canEndTurn() const;
    bool isPlayerInJail(int playerIndex) const;
    bool isGameOver() const;

    // 原有的辅助方法（需要修改）
    int rollDice();
    void movePlayer(int playerIndex, int steps);
    int getAvailableBuildings(bool houses) const;
    void modifyAvailableBuildings(bool house, int count);

    bool playerCanPay(int payerIndex, int amount, int receiverIndex = -1);
    void payToPlayers(int payerIndex, int amount);
    void receiveFromPlayers(int receiverIndex, int amount);

    // 卡片相关
    void drawChanceCard(int playerIndex);
    void drawCommunityChestCard(int playerIndex);
    void useGetOutOfJailCard(int playerIndex);

    // 交易相关（需要大幅修改）
    /*
    void proposeTrade(int fromPlayer, int toPlayer, int propertyIndex, int amount);
    void acceptTrade(int tradeId);
    void rejectTrade(int tradeId);*/

signals:
    // 游戏状态信号
    void gameStarted();
    void gameStateChanged(GameState newState);
    void gameOver(int winnerPlayerIndex);

    // 玩家回合信号
    void playerTurnStarted(int playerIndex);
    void playerTurnEnded(int playerIndex);

    // 玩家行动信号
    void diceRolled(int playerIndex, int diceValue);
    void playerMoved(int playerIndex, int oldPosition, int newPosition);
    void playerSentToJail(int playerIndex);
    //void playerReleasedFromJail(int playerIndex);

    // 格子事件信号
    void tileEventTriggered(int tileIndex, const QString& eventMessage);
    void purchaseOpportunity(const QString& propertyName, int price, int playerIndex);
    void rentPaymentRequired(const QString& propertyName, int rent, int fromPlayer, int toPlayer);
    void taxPaymentRequired(const QString& taxName, int tax,int fromPlayer);
    void landOnSelfProperty(int playerIndex, const QString& propertyName);
    void openChanceCard(int playerIndex,const QString& cardDiscription);
    void openCommunityCard(int playerIndex,const QString& cardDiscription);
    void landOnFreeParking(int playerIndex, int type);

    // 经济信号
    void rentPaid(const QString& fromPlayer, int rentAmount, const QString& toPlayer);

    // 游戏日志信号
    void gameLogMessage(const QString& message);

    // 需要读懂这些信号是干嘛的
    void forceRaiseMoney(int playerIndex, int amount);
    void playerBankrupt(int playerIndex);
    void tradeProposed(int fromPlayer, int toPlayer, int propertyIndex, int amount);

    void warningSignal(const QString& message);

    /*Game();

    // Constructor tools
    static int getNumPlayers();
    void addPlayer(std::string name);
    void testAddPlayer(std::string name);

    // Game loop
    void startGame();
    void gameTurn(Player& currentPlayer);
    void normalTurn(Player& currentPlayers, int rolled_twelve);
    void jailTurn(Player& currentPlayers, int jailStatus);
    void rollToGetOutofJail(Player& currentPlayer);
    void payToGetOutofJail(Player& currentPlayer);
    void endTurn(Player& currentPlayer);
    void endGame(Player* currentPlayer = nullptr);

    // Helpers
    int rollDice();

    void showPlayers() const;
    void showBoard() const;
    int getAvailableBuildings(bool houses) const;
    void modifyAvailableBuildings(bool house, int count);
    void movePlayer(Player& currentPlayer, int step);

    bool playerCanPay(Player& payer, int amount, Player* receiver = nullptr);
    void payToPlayers(Player& payer, int amount);
    void receiveFromPlayers(Player& receiver, int amount);

    void drawChanceCard(Player& player);
    void drawCommunityChestCard(Player& player);
    void useGetOutOfJailCard(Player& player);

    void manageSellTrade(Player& offeringPlayer, int propertyIndex);
    void manageBuyTrade(Player& offeringPlayer, int propertyIndex);
    bool validateTradePlayers(Player* offeringPlayer, Player* targetPlayer, int propertyIndex, bool offerBuy);
    void askTradeDecision(Player* offeringPlayer, Player* targetPlayer, int amount, int propertyIndex, PropertyTile* property, bool offerBuy);
    void executeTrade(Player* buyingPlayer, Player* sellingPlayer, int amount, int propertyIndex, PropertyTile* property);

    static char getChoice(char a, char b, char c = '\0', char d = '\0', char e = '\0');

    // newly added method
    int getPlayerCount() const { return players.size(); }
    Player& getPlayer(int index) { return players[index];}
    int getCurrentPlayerIndex() const { return currentPlayerIndex; }

*/

};

#endif
