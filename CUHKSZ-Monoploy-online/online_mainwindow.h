#ifndef ONLINE_MAINWINDOW_H
#define ONLINE_MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QDialog>
#include <QListWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QRadioButton>
#include <QComboBox>
#include <QButtonGroup>
#include <QScrollBar>
#include <QInputDialog>
#include <QMap>
#include <memory>
#include <map>

#include "monopoly/online_client.h"
#include "Game.h"
#include "BoardWidget.h"

class OnlineMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit OnlineMainWindow(QWidget* parent = nullptr);
    ~OnlineMainWindow();

private slots:
    // room management
    void onConnectToServer();
    void onCreateRoom();
    void onJoinRoom();
    void onRefreshRoomList();
    void onRoomSelected();
    
    // game operations (online mode)
    void onRollDiceClicked();
    void onBuyPropertyClicked();
    void onSellPropertyClicked();
    void onPayBailClicked();  
    void onEndTurnClicked();
    void onHintClicked();
    void onStartGameClicked();
    void onViewPropertyClicked();
    
    // chatroom
    void onChatroomClicked();
    void onSendChatMessage();
    
    // network callbacks
    void onConnected();
    void onDisconnected();
    void onError(const QString& error);
    void onJoinedRoom(const QString& roomId, int playerId, const monopoly::StateSnapshot& snapshot);
    void onRoomListReceived(const std::vector<monopoly::RoomSummary>& rooms);
    void onStateUpdate(const monopoly::StateSnapshot& snapshot);
    void onGameEvent(const monopoly::GameEvent& event);
    void onChatReceived(int playerId, const QString& playerName, const QString& message);
    
    // game signal handling
    void onPlayerTurnStarted(int playerIndex);
    void onPlayerTurnEnded(int playerIndex);
    void onDiceRolled(int playerIndex, int diceValue);
    void onPlayerMoved(int playerIndex, int oldPosition, int newPosition);
    void onPurchaseOpportunity(const QString& propertyName, int price, int playerIndex);
    void onGameLogMessage(const QString& message);
    void onPlayerBankrupt(int playerIndex);
    void onRentPaymentRequired(const QString& propertyName, int rent, int fromPlayer, int toPlayer);
    void onForceRaiseMoney(int payerIndex, int amountDue);

private:
    // UI components
    void setupUI();
    void setupRoomDialog();
    void setupChatDialog();
    void setupPropertyDialog();
    void setupNetworkConnections();
    void setupGameConnections();
    void showRoomDialog();
    void updatePlayerInfo();
    void updateGameDisplay();
    void addGameLog(const QString& message);
    
    // game state synchronization
    void syncGameStateFromNetwork(const monopoly::StateSnapshot& snapshot);
    void syncPlayersFromSnapshot(const monopoly::StateSnapshot& snapshot);
    void syncPlayerState(int localIndex, const monopoly::PlayerState& serverState);
    
    // helper functions
    int getMyLocalPlayerIndex() const;
    bool isMyTurn() const;
    int getServerPlayerId(int localIndex) const;
    int getLocalPlayerIndex(int serverPlayerId) const;
    void updateChatPlayerList(); 
    
    // game objects
    Game* m_game;
    BoardWidget* m_boardWidget;
    
    // network client
    monopoly::OnlineGameClient* m_client;
    QString m_currentRoomId;
    int m_myPlayerId{-1};
    std::map<int, int> m_localToServerPlayerId;  
    monopoly::StateSnapshot m_lastSnapshot;
    
    // turn limit flags
    bool m_hasRolledDiceThisTurn{false};
    bool m_hasUsedHintThisTurn{false};
    int m_lastTurnPlayerId{-1}; 
    
    // UI components
    QPushButton* m_rollButton;
    QPushButton* m_buyPropertyButton;
    QPushButton* m_sellPropertyButton;
    QPushButton* m_endTurnButton;
    QPushButton* m_hintButton;
    QPushButton* m_startGameButton;
    QPushButton* m_chatroomButton;
    QPushButton* m_payBailButton;  
    QPushButton* m_viewPropertyButton;
    
    QLabel* m_playerInfoLabel;
    QLabel* m_statusLabel;
    QLabel* m_cashLabel;
    QTextEdit* m_gameLog;
    
    // dialogs
    QDialog* m_roomDialog;
    QDialog* m_propertyDialog;
    QLineEdit* m_serverUrlEdit;
    QLineEdit* m_playerNameEdit;
    QLineEdit* m_roomIdEdit;
    QListWidget* m_roomListWidget;
    QPushButton* m_connectButton;
    QPushButton* m_createRoomButton;
    QPushButton* m_joinRoomButton;
    QPushButton* m_refreshButton;
    QLabel* m_connectionStatusLabel;
    
    // chatroom dialog
    QDialog* m_chatDialog;
    QTextEdit* m_chatMessages;
    QLineEdit* m_chatInput;
    QPushButton* m_sendChatButton;
    QRadioButton* m_chatModeGroup; 
    QRadioButton* m_chatModePrivate;  
    QComboBox* m_chatTargetCombo; 
};

#endif 

