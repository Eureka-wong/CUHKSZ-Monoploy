#ifndef GAME_H
#define GAME_H

#include <vector>
#include <functional>
#include <QObject>
#include "Board.h"
#include "Player.h"
#include "CardDeck.h"

enum class GameState {
    WaitingForStart,
    PlayerTurn,
    Moving,
    ProcessingEvent,
    GameOver
};

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

    void addPlayer(std::string name);
    void clearPlayers();


    void startGame();
    void startPlayerTurn();
    void endTurn();
    void rollDiceAndMoveAndProcessEvent();
    void processJailAction(JailAction action);
    void triggerCurrentTileEvent();
    void endGame();

    int getPlayerCount() const { return players.size(); }

    void purchaseCurrentProperty();
    void payRent(int fromPlayer, int toPlayer, int rent);

    int getPlayerIndex(const Player& player) const;
    Player& getPlayer(int index) { return players[index]; }
    const Player& getPlayer(int index) const { return players[index]; }
    int getCurrentPlayerIndex() const { return currentPlayerIndex; }
    void setCurrentPlayerIndex(int index) { currentPlayerIndex = index; }

    int getCurrentRound() const { return round; }
    GameState getCurrentState() const { return currentState; }
    Board& getBoard() { return board; }
    const Board& getBoard() const { return board; }
    std::vector<PropertyTile*> getPropertiesByGroup(const std::string& group);
    const std::vector<Player>& getplayerlist() const { return players; }

    // checkers
    bool canRollDice() const;
    bool canEndTurn() const;
    bool isPlayerInJail(int playerIndex) const;
    bool isGameOver() const;

    // helpers
    int rollDice();
    void movePlayer(int playerIndex, int steps);
    int getAvailableBuildings(bool houses) const;
    void modifyAvailableBuildings(bool house, int count);

    bool playerCanPay(int payerIndex, int amount, int receiverIndex = -1);
    void payToPlayers(int payerIndex, int amount);
    void receiveFromPlayers(int receiverIndex, int amount);

    // card

    void drawChanceCard(int playerIndex);
    void drawCommunityChestCard(int playerIndex);
    // void useGetOutOfJailCard(int playerIndex);

    /*
    void proposeTrade(int fromPlayer, int toPlayer, int propertyIndex, int amount);
    void acceptTrade(int tradeId);
    void rejectTrade(int tradeId);*/


    /*
    void setPurchaseCallback(PurchaseCallback callback) { m_purchaseCallback = callback; }
    bool hasPurchaseCallback() const { return static_cast<bool>(m_purchaseCallback); }
    bool executePurchaseCallback(const std::string& propertyName, int price, const std::string& playerName) {
        if (m_purchaseCallback) {
            return m_purchaseCallback(propertyName, price, playerName);
        }
        return false;
    }*/

signals:
    // game events
    void gameStarted();
    void gameStateChanged(GameState newState);
    void gameOver(int winnerPlayerIndex);

    // turn events
    void playerTurnStarted(int playerIndex);
    void playerTurnEnded(int playerIndex);

    // action events
    void diceRolled(int playerIndex, int diceValue);
    void playerMoved(int playerIndex, int oldPosition, int newPosition);
    void playerSentToJail(int playerIndex);
    void playerReleasedFromJail(int playerIndex);

    // tile events
    void tileEventTriggered(int tileIndex, const QString& eventMessage);
    void purchaseOpportunity(const QString& propertyName, int price, int playerIndex);
    void rentPaymentRequired(const QString& propertyName, int rent, int fromPlayer, int toPlayer);
    //void cardDrawn(const QString& cardMessage);

    // deal
    void rentPaid(const QString& fromPlayer, int rentAmount, const QString& toPlayer);

    // game log
    void gameLogMessage(const QString& message);

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
