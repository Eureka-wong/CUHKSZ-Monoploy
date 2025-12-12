#include "online_mainwindow.h"
#include <QMessageBox>
#include <QScrollArea>
#include <QDialogButtonBox>
#include <QTimer>
#include <QObject>
#include <QApplication>
#include <iostream>
#include "Tile.h"
#include "StrategyEngine.h"

OnlineMainWindow::OnlineMainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_game(new Game(this))
    , m_boardWidget(nullptr)
    , m_client(new monopoly::OnlineGameClient(this))
    , m_myPlayerId(-1)
    , m_rollButton(nullptr)
    , m_buyPropertyButton(nullptr)
    , m_sellPropertyButton(nullptr)
    , m_endTurnButton(nullptr)
    , m_hintButton(nullptr)
    , m_startGameButton(nullptr)
    , m_chatroomButton(nullptr)
    , m_payBailButton(nullptr)
    , m_viewPropertyButton(nullptr)
    , m_propertyDialog(nullptr)
    , m_playerInfoLabel(nullptr)
    , m_statusLabel(nullptr)
    , m_cashLabel(nullptr)
    , m_gameLog(nullptr)
    , m_roomDialog(nullptr)
    , m_chatDialog(nullptr)
    , m_chatModeGroup(nullptr)
    , m_chatModePrivate(nullptr)
    , m_chatTargetCombo(nullptr)
{
    setupUI();
    setupRoomDialog();
    setupChatDialog();
    setupPropertyDialog();
    setupNetworkConnections();
    setupGameConnections();
    showRoomDialog();
}

OnlineMainWindow::~OnlineMainWindow() {
}

void OnlineMainWindow::setupUI() {
    QWidget* centralWidget = new QWidget();
    setCentralWidget(centralWidget);
    
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    
    m_boardWidget = new BoardWidget(m_game, this);
    mainLayout->addWidget(m_boardWidget);
    
    QWidget* controlPanel = new QWidget();
    QVBoxLayout* controlLayout = new QVBoxLayout(controlPanel);
    
    m_playerInfoLabel = new QLabel("Player: Not connected");
    m_playerInfoLabel->setStyleSheet("font-weight: bold; margin: 10px;");
    controlLayout->addWidget(m_playerInfoLabel);
    
    m_cashLabel = new QLabel("Cash: $0");
    m_cashLabel->setStyleSheet("margin-left: 10px;");
    controlLayout->addWidget(m_cashLabel);
    
    m_statusLabel = new QLabel("Status: Disconnected");
    m_statusLabel->setStyleSheet("margin-left: 10px; color: #e74c3c;");
    controlLayout->addWidget(m_statusLabel);
    
    QLabel* logTitle = new QLabel("Game Events:");
    logTitle->setStyleSheet("font-weight: bold; margin-top: 10px;");
    controlLayout->addWidget(logTitle);
    
    m_gameLog = new QTextEdit();
    m_gameLog->setMaximumHeight(150);
    m_gameLog->setReadOnly(true);
    m_gameLog->setStyleSheet("QTextEdit { background-color: black; color: white; }");
    controlLayout->addWidget(m_gameLog);
    
    m_startGameButton = new QPushButton("Start Game");
    m_startGameButton->setStyleSheet("QPushButton { background-color: #27ae60; color: white; padding: 10px; border-radius: 4px; margin: 5px 10px; font-weight: bold; }");
    m_startGameButton->setVisible(false);
    connect(m_startGameButton, &QPushButton::clicked, this, &OnlineMainWindow::onStartGameClicked);
    controlLayout->addWidget(m_startGameButton);
    
    m_rollButton = new QPushButton("Roll Dice");
    m_rollButton->setStyleSheet("QPushButton { background-color: #E4A823; color: white; padding: 10px; border-radius: 4px; margin: 5px 10px; font-weight: bold; }");
    m_rollButton->setEnabled(false);
    connect(m_rollButton, &QPushButton::clicked, this, &OnlineMainWindow::onRollDiceClicked);
    controlLayout->addWidget(m_rollButton);
    
    m_buyPropertyButton = new QPushButton("Buy Property");
    m_buyPropertyButton->setStyleSheet("QPushButton { background-color: #E4A823; color: white; padding: 8px; border-radius: 4px; margin: 5px 10px; }");
    m_buyPropertyButton->setEnabled(false);
    connect(m_buyPropertyButton, &QPushButton::clicked, this, &OnlineMainWindow::onBuyPropertyClicked);
    controlLayout->addWidget(m_buyPropertyButton);
    
    m_sellPropertyButton = new QPushButton("Sell Property");
    m_sellPropertyButton->setStyleSheet("QPushButton { background-color: #E74C3C; color: white; padding: 8px; border-radius: 4px; margin: 5px 10px; }");
    m_sellPropertyButton->setEnabled(false);
    connect(m_sellPropertyButton, &QPushButton::clicked, this, &OnlineMainWindow::onSellPropertyClicked);
    controlLayout->addWidget(m_sellPropertyButton);
    
    m_payBailButton = new QPushButton("Pay Bail ($50)");
    m_payBailButton->setStyleSheet("QPushButton { background-color: #9B59B6; color: white; padding: 8px; border-radius: 4px; margin: 5px 10px; }");
    m_payBailButton->setEnabled(false);
    connect(m_payBailButton, &QPushButton::clicked, this, &OnlineMainWindow::onPayBailClicked);
    controlLayout->addWidget(m_payBailButton);
    
    m_endTurnButton = new QPushButton("End Turn");
    m_endTurnButton->setStyleSheet("QPushButton { background-color: #E4A823; color: white; padding: 8px; border-radius: 4px; margin: 5px 10px; }");
    m_endTurnButton->setEnabled(false);
    connect(m_endTurnButton, &QPushButton::clicked, this, &OnlineMainWindow::onEndTurnClicked);
    controlLayout->addWidget(m_endTurnButton);
    
    m_hintButton = new QPushButton("Get AI Hint ($50)");
    m_hintButton->setStyleSheet("QPushButton { background-color: #E4A823; color: white; padding: 8px; border-radius: 4px; margin: 5px 10px; }");
    m_hintButton->setEnabled(false);
    connect(m_hintButton, &QPushButton::clicked, this, &OnlineMainWindow::onHintClicked);
    controlLayout->addWidget(m_hintButton);
    
    m_chatroomButton = new QPushButton("Chatroom");
    m_chatroomButton->setStyleSheet("QPushButton { background-color: #E4A823; color: white; padding: 8px; border-radius: 4px; margin: 5px 10px; }");
    connect(m_chatroomButton, &QPushButton::clicked, this, &OnlineMainWindow::onChatroomClicked);
    controlLayout->addWidget(m_chatroomButton);
    
    m_viewPropertyButton = new QPushButton("View Property");
    m_viewPropertyButton->setStyleSheet("QPushButton { background-color: #E4A823; color: white; padding: 8px; border-radius: 4px; margin: 5px 10px; }");
    connect(m_viewPropertyButton, &QPushButton::clicked, this, &OnlineMainWindow::onViewPropertyClicked);
    controlLayout->addWidget(m_viewPropertyButton);
    
    controlLayout->addStretch();
    mainLayout->addWidget(controlPanel);
    
    setWindowTitle("CUHKSZ Monopoly Online");
    resize(1200, 850);
}

void OnlineMainWindow::setupRoomDialog() {
    m_roomDialog = new QDialog(this);
    m_roomDialog->setWindowTitle("Join Room");
    m_roomDialog->setMinimumSize(500, 400);
    
    QVBoxLayout* layout = new QVBoxLayout(m_roomDialog);
    
    QLabel* serverLabel = new QLabel("Server URL:");
    layout->addWidget(serverLabel);
    m_serverUrlEdit = new QLineEdit();
    m_serverUrlEdit->setText("ws://10.30.198.46:9002");
    m_serverUrlEdit->setPlaceholderText("ws://10.30.198.46:9002 or ws://localhost:9002");
    layout->addWidget(m_serverUrlEdit);
    
    QLabel* nameLabel = new QLabel("Player Name:");
    layout->addWidget(nameLabel);
    m_playerNameEdit = new QLineEdit();
    m_playerNameEdit->setText("Player1");
    layout->addWidget(m_playerNameEdit);
    
    m_connectionStatusLabel = new QLabel("Not connected");
    m_connectionStatusLabel->setStyleSheet("color: #e74c3c;");
    layout->addWidget(m_connectionStatusLabel);
    
    QLabel* listLabel = new QLabel("Available Rooms:");
    layout->addWidget(listLabel);
    m_roomListWidget = new QListWidget();
    layout->addWidget(m_roomListWidget);
    
    QLabel* roomLabel = new QLabel("Room ID:");
    layout->addWidget(roomLabel);
    m_roomIdEdit = new QLineEdit();
    m_roomIdEdit->setText("room1");
    layout->addWidget(m_roomIdEdit);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    m_connectButton = new QPushButton("Connect");
    m_createRoomButton = new QPushButton("Create Room");
    m_joinRoomButton = new QPushButton("Join Room");
    m_refreshButton = new QPushButton("Refresh");
    
    connect(m_connectButton, &QPushButton::clicked, this, &OnlineMainWindow::onConnectToServer);
    connect(m_createRoomButton, &QPushButton::clicked, this, &OnlineMainWindow::onCreateRoom);
    connect(m_joinRoomButton, &QPushButton::clicked, this, &OnlineMainWindow::onJoinRoom);
    connect(m_refreshButton, &QPushButton::clicked, this, &OnlineMainWindow::onRefreshRoomList);
    connect(m_roomListWidget, &QListWidget::itemDoubleClicked, this, &OnlineMainWindow::onRoomSelected);
    
    buttonLayout->addWidget(m_connectButton);
    buttonLayout->addWidget(m_createRoomButton);
    buttonLayout->addWidget(m_joinRoomButton);
    buttonLayout->addWidget(m_refreshButton);
    layout->addLayout(buttonLayout);
}

void OnlineMainWindow::setupChatDialog() {
    m_chatDialog = new QDialog(this);
    m_chatDialog->setWindowTitle("Chatroom");
    m_chatDialog->setMinimumSize(600, 500);
    m_chatDialog->setModal(false); 
    
    QVBoxLayout* layout = new QVBoxLayout(m_chatDialog);
    layout->setSpacing(10);
    layout->setContentsMargins(15, 15, 15, 15);
    
    QLabel* titleLabel = new QLabel("💬 Chatroom");
    titleLabel->setStyleSheet("QLabel { font-size: 18px; font-weight: bold; color: #2c3e50; margin-bottom: 10px; }");
    layout->addWidget(titleLabel);
    
    QHBoxLayout* modeLayout = new QHBoxLayout();
    QLabel* modeLabel = new QLabel("Mode:");
    modeLabel->setStyleSheet("QLabel { font-weight: bold; color: #34495e; }");
    modeLayout->addWidget(modeLabel);
    
    m_chatModeGroup = new QRadioButton("Group Chat");
    m_chatModeGroup->setChecked(true);  
    m_chatModeGroup->setStyleSheet("QRadioButton { color: #2c3e50; }");
    
    m_chatModePrivate = new QRadioButton("Private Chat");
    m_chatModePrivate->setStyleSheet("QRadioButton { color: #2c3e50; }");
    
    QButtonGroup* modeGroup = new QButtonGroup(this);
    modeGroup->addButton(m_chatModeGroup, 0);
    modeGroup->addButton(m_chatModePrivate, 1);
    
    modeLayout->addWidget(m_chatModeGroup);
    modeLayout->addWidget(m_chatModePrivate);
    modeLayout->addStretch();
    
    QLabel* targetLabel = new QLabel("To:");
    targetLabel->setStyleSheet("QLabel { font-weight: bold; color: #34495e; }");
    m_chatTargetCombo = new QComboBox();
    m_chatTargetCombo->setStyleSheet(
        "QComboBox { "
        "border: 2px solid #95a5a6; "
        "border-radius: 4px; "
        "padding: 5px; "
        "background-color: #ffffff; "
        "min-width: 150px; "
        "}"
    );
    m_chatTargetCombo->setEnabled(false); 
    m_chatTargetCombo->addItem("Select player...");
    
    modeLayout->addWidget(targetLabel);
    modeLayout->addWidget(m_chatTargetCombo);
    layout->addLayout(modeLayout);
    
    connect(m_chatModeGroup, &QRadioButton::toggled, [this](bool checked) {
        if (m_chatTargetCombo) {
            m_chatTargetCombo->setEnabled(!checked);
        }
    });
    
    m_chatMessages = new QTextEdit();
    m_chatMessages->setReadOnly(true);
    m_chatMessages->setStyleSheet(
        "QTextEdit { "
        "background-color: #ffffff; "
        "border: 2px solid #e0e0e0; "
        "border-radius: 8px; "
        "padding: 10px; "
        "font-size: 13px; "
        "color: #333333; "
        "}"
    );
    m_chatMessages->setPlaceholderText("No messages yet. Start chatting!");
    layout->addWidget(m_chatMessages, 1); 
    
    QHBoxLayout* inputLayout = new QHBoxLayout();
    inputLayout->setSpacing(10);
    
    m_chatInput = new QLineEdit();
    m_chatInput->setPlaceholderText("Type your message here... (Press Enter to send)");
    m_chatInput->setStyleSheet(
        "QLineEdit { "
        "border: 2px solid #3498db; "
        "border-radius: 6px; "
        "padding: 8px; "
        "font-size: 13px; "
        "background-color: #ffffff; "
        "}"
        "QLineEdit:focus { "
        "border-color: #2980b9; "
        "}"
    );
    
    m_sendChatButton = new QPushButton("Send");
    m_sendChatButton->setStyleSheet(
        "QPushButton { "
        "background-color: #3498db; "
        "color: white; "
        "border: none; "
        "border-radius: 6px; "
        "padding: 8px 20px; "
        "font-size: 13px; "
        "font-weight: bold; "
        "min-width: 80px; "
        "}"
        "QPushButton:hover { "
        "background-color: #2980b9; "
        "}"
        "QPushButton:pressed { "
        "background-color: #21618c; "
        "}"
    );
    
    connect(m_chatInput, &QLineEdit::returnPressed, this, &OnlineMainWindow::onSendChatMessage);
    connect(m_sendChatButton, &QPushButton::clicked, this, &OnlineMainWindow::onSendChatMessage);
    
    inputLayout->addWidget(m_chatInput, 1);
    inputLayout->addWidget(m_sendChatButton);
    layout->addLayout(inputLayout);
    
    if (m_chatMessages) {
        m_chatMessages->append("<div style='color: #7f8c8d; font-style: italic;'>Welcome to the chatroom! Start chatting with other players.</div>");
    }
}

void OnlineMainWindow::setupPropertyDialog() {
    m_propertyDialog = new QDialog(this);
    m_propertyDialog->setWindowTitle("View Properties");
    m_propertyDialog->setMinimumSize(800, 600);
    m_propertyDialog->setModal(false);
    
    QVBoxLayout* layout = new QVBoxLayout(m_propertyDialog);
    layout->setSpacing(10);
    layout->setContentsMargins(15, 15, 15, 15);
    
    QLabel* titleLabel = new QLabel("🏠 Property Overview");
    titleLabel->setStyleSheet("QLabel { font-size: 18px; font-weight: bold; color: #2c3e50; margin-bottom: 10px; }");
    layout->addWidget(titleLabel);
    
    QTextEdit* propertyText = new QTextEdit();
    propertyText->setReadOnly(true);
    propertyText->setStyleSheet(
        "QTextEdit { "
        "border: 2px solid #bdc3c7; "
        "border-radius: 4px; "
        "padding: 10px; "
        "background-color: #ffffff; "
        "font-family: 'Consolas', 'Courier New', monospace; "
        "font-size: 11px; "
        "}"
    );
    layout->addWidget(propertyText);
    
    m_propertyDialog->setProperty("propertyText", QVariant::fromValue(propertyText));
    
    QPushButton* closeButton = new QPushButton("Close");
    closeButton->setStyleSheet(
        "QPushButton { "
        "background-color: #e74c3c; "
        "color: white; "
        "padding: 8px 20px; "
        "border-radius: 4px; "
        "font-weight: bold; "
        "}"
        "QPushButton:hover { "
        "background-color: #c0392b; "
        "}"
    );
    connect(closeButton, &QPushButton::clicked, m_propertyDialog, &QDialog::close);
    layout->addWidget(closeButton);
}

void OnlineMainWindow::setupNetworkConnections() {
    m_client->setOnConnected([this]() {
        addGameLog("Connected to server!");
        if (m_connectionStatusLabel) {
            m_connectionStatusLabel->setText("Connected");
            m_connectionStatusLabel->setStyleSheet("color: #27ae60;");
        }
        onRefreshRoomList();
    });
    
    m_client->setOnDisconnected([this]() {
        addGameLog("Disconnected from server");
        if (m_connectionStatusLabel) {
            m_connectionStatusLabel->setText("Disconnected");
            m_connectionStatusLabel->setStyleSheet("color: #e74c3c;");
        }
    });
    
    m_client->setOnError([this](const QString& error) {
        addGameLog("Error: " + error);
        QMessageBox::warning(this, "Connection Error", error);
    });
    
    m_client->setOnJoinedRoom([this](const QString& roomId, int playerId, const monopoly::StateSnapshot& snapshot) {
        std::cout << "[MainWindow] onJoinedRoom: roomId=" << roomId.toStdString() 
                  << ", playerId=" << playerId << std::endl;
        m_currentRoomId = roomId;
        m_myPlayerId = playerId;
        m_lastSnapshot = snapshot;
        
        addGameLog(QString("Joined room: %1, Player ID: %2").arg(roomId).arg(playerId));
        addGameLog(QString("Room has %1 players").arg(snapshot.players.size()));
        
        syncGameStateFromNetwork(snapshot);
        
        if (m_roomDialog) {
            m_roomDialog->accept();
        }
        
        if (m_statusLabel) {
            if (snapshot.phase == monopoly::RoomPhase::InGame) {
                m_statusLabel->setText("Game already in progress. Waiting for your turn...");
            } else {
                m_statusLabel->setText("Connected to room. Waiting for game to start...");
            }
        }
        
        if (m_startGameButton) {
            if (snapshot.phase != monopoly::RoomPhase::InGame) {
                m_startGameButton->setVisible(true);
                m_startGameButton->setEnabled(true);
                m_startGameButton->setText("Start Game");
            } else {
                m_startGameButton->setVisible(false);
            }
        }
    });
    
    m_client->setOnRoomList([this](const std::vector<monopoly::RoomSummary>& rooms) {
        if (m_roomListWidget) {
            m_roomListWidget->clear();
            for (const auto& room : rooms) {
                QString text = QString("%1 (%2/%3) - %4")
                    .arg(QString::fromStdString(room.roomId))
                    .arg(room.currentPlayers)
                    .arg(room.maxPlayers)
                    .arg(QString::fromStdString(room.ruleVariant));
                m_roomListWidget->addItem(text);
            }
        }
    });
    
    m_client->setOnStateUpdate([this](const monopoly::StateSnapshot& snapshot) {
        std::cout << "[MainWindow] ===== State Update Received =====" << std::endl;
        std::cout << "[MainWindow] Phase: " << static_cast<int>(snapshot.phase) << std::endl;
        std::cout << "[MainWindow] Players: " << snapshot.players.size() << std::endl;
        
        bool gameJustStarted = false;
        if (m_lastSnapshot.players.size() > 0) {
            gameJustStarted = (m_lastSnapshot.phase != monopoly::RoomPhase::InGame && 
                              snapshot.phase == monopoly::RoomPhase::InGame);
        }
        
        m_lastSnapshot = snapshot;
        
        int oldPlayerCount = m_game ? m_game->getPlayerCount() : 0;
        
        syncGameStateFromNetwork(snapshot);
        
        std::cout << "[MainWindow] ===== State Update Processed =====" << std::endl;
        
        if (snapshot.phase != monopoly::RoomPhase::InGame) {
            int newPlayerCount = static_cast<int>(snapshot.players.size());
            if (newPlayerCount > oldPlayerCount) {
                addGameLog(QString("New player joined! Total players: %1").arg(newPlayerCount));
            }
            
            if (m_statusLabel) {
                m_statusLabel->setText(QString("Connected to room. %1/%2 players. Waiting for game to start...")
                    .arg(newPlayerCount)
                    .arg(snapshot.settings.maxPlayers));
            }
            
            if (m_startGameButton && newPlayerCount >= 2) {
                m_startGameButton->setVisible(true);
                m_startGameButton->setEnabled(true);
                m_startGameButton->setText("Start Game");
            }
        }
        
        if (snapshot.phase == monopoly::RoomPhase::InGame) {
            if (gameJustStarted) {
                addGameLog("✓ Start game successful! Game is now in progress.");
                if (m_statusLabel) {
                    m_statusLabel->setText("✓ Game started successfully!");
                }
                m_lastTurnPlayerId = -1;
                m_hasRolledDiceThisTurn = false;
                m_hasUsedHintThisTurn = false;
                std::cout << "[MainWindow] Game started, reset turn tracking" << std::endl;
            }
            
            if (m_game && m_game->getCurrentState() != GameState::PlayerTurn) {
                std::cout << "[MainWindow] Game state is not PlayerTurn, calling startGame()" << std::endl;
                m_game->startGame();
                std::cout << "[MainWindow] Game state set to PlayerTurn" << std::endl;
            }
            
            if (m_startGameButton) {
                m_startGameButton->setVisible(false);
            }
            if (m_statusLabel) {
                if (snapshot.currentTurnIndex >= 0 && snapshot.currentTurnIndex < static_cast<int>(snapshot.players.size())) {
                    int currentTurnPlayerId = snapshot.players[snapshot.currentTurnIndex].playerId;
                    if (currentTurnPlayerId == m_myPlayerId) {
                        m_statusLabel->setText("Game started! It's your turn - click 'Roll Dice' to start.");

                        if (m_lastTurnPlayerId != currentTurnPlayerId) {
                            m_hasRolledDiceThisTurn = false;
                            m_hasUsedHintThisTurn = false;
                            m_lastTurnPlayerId = currentTurnPlayerId;
                            std::cout << "[MainWindow] Game started with my turn, reset flags" << std::endl;
                        }
                    } else {
                        QString playerName = QString::fromStdString(snapshot.players[snapshot.currentTurnIndex].displayName);
                        m_statusLabel->setText(QString("Game started! Waiting for %1 to roll dice...").arg(playerName));

                        if (m_lastTurnPlayerId != currentTurnPlayerId) {
                            m_hasRolledDiceThisTurn = false;
                            m_hasUsedHintThisTurn = false;
                            m_lastTurnPlayerId = currentTurnPlayerId;
                            std::cout << "[MainWindow] Game started with other player's turn, reset flags" << std::endl;
                        }
                    }
                } else {
                    m_statusLabel->setText("Game started. Waiting for first player to roll dice...");
                }
            }

            updatePlayerInfo();
        }
    });
    
    m_client->setOnEvent([this](const monopoly::GameEvent& event) {
        QString topic = QString::fromStdString(event.topic);
        std::cout << "[MainWindow] Received event: " << topic.toStdString() << std::endl;
        
        if (topic == "room.game_started") {
            addGameLog("✓ Start game successful! Game is now in progress.");
            if (m_statusLabel) {
                m_statusLabel->setText("✓ Game started successfully!");
            }
            if (m_startGameButton) {
                m_startGameButton->setVisible(false);
            }
        } else if (topic == "game.move") {
            int playerId = event.payload["playerId"].toInt();
            int newPosition = event.payload["newPosition"].toInt();
            int dice1 = event.payload["dice1"].toInt(0);
            int dice2 = event.payload["dice2"].toInt(0);
            int total = event.payload["total"].toInt(0);
            

            QString playerName = "Player " + QString::number(playerId);
            for (const auto& p : m_lastSnapshot.players) {
                if (p.playerId == playerId) {
                    playerName = QString::fromStdString(p.displayName);
                    break;
                }
            }
            
            if (dice1 > 0 && dice2 > 0) {
                addGameLog(QString("🎲 %1 rolled dice: %2 + %3 = %4")
                          .arg(playerName).arg(dice1).arg(dice2).arg(total));
            }
            
            bool wentToJail = event.payload["wentToJail"].toBool(false);
            if (wentToJail) {
                int jailPosition = event.payload["jailPosition"].toInt(10);
                int jailStatus = event.payload["jailStatus"].toInt(0);
                int jailTurnsRemaining = event.payload["jailTurnsRemaining"].toInt(3);
                addGameLog(QString("🚨 %1 went to jail! (position %2, %3 turns remaining)")
                          .arg(playerName).arg(jailPosition).arg(jailTurnsRemaining));
            }
            

            bool taxPaid = event.payload["taxPaid"].toBool(false);
            if (taxPaid) {
                int taxAmount = event.payload["taxAmount"].toInt(0);
                int cashAfterTax = event.payload["cashAfterTax"].toInt(0);
                addGameLog(QString("💰 %1 paid tax $%2 at position %3. Remaining cash: $%4")
                          .arg(playerName).arg(taxAmount).arg(newPosition).arg(cashAfterTax));
            }
            
            bool rentPaid = event.payload["rentPaid"].toBool(false);
            if (rentPaid) {
                int rentAmount = event.payload["rentAmount"].toInt(0);
                int ownerId = event.payload["ownerId"].toInt(-1);
                int payerCashAfterRent = event.payload["payerCashAfterRent"].toInt(0);
                int ownerCashAfterRent = event.payload["ownerCashAfterRent"].toInt(0);
                QString ownerName = "Player " + QString::number(ownerId);
                for (const auto& p : m_lastSnapshot.players) {
                    if (p.playerId == ownerId) {
                        ownerName = QString::fromStdString(p.displayName);
                        break;
                    }
                }
                addGameLog(QString("💵 %1 paid $%2 rent to %3 at position %4. %1's cash: $%5, %3's cash: $%6")
                          .arg(playerName).arg(rentAmount).arg(ownerName).arg(newPosition)
                          .arg(payerCashAfterRent).arg(ownerCashAfterRent));
            }
            

            bool passedGo = event.payload["passedGo"].toBool(false);
            if (passedGo) {
                int goBonus = event.payload["goBonus"].toInt(200);
                addGameLog(QString("🎉 %1 passed Go! Received $%2")
                          .arg(playerName).arg(goBonus));
            }
            
            bool drewCard = event.payload["drewCard"].toBool(false);
            if (drewCard) {
                QString cardType = event.payload["cardType"].toString();
                QString cardDescription = event.payload["cardDescription"].toString();
                int cardAmount = event.payload["cardAmount"].toInt(0);
                int cardTargetTile = event.payload["cardTargetTile"].toInt(-1);
                int cardStep = event.payload["cardStep"].toInt(0);
                bool cardGetOutOfJail = event.payload["cardGetOutOfJail"].toBool(false);
                bool cardGoToJail = event.payload["cardGoToJail"].toBool(false);
                
                QString cardEmoji = (cardType == "Chance") ? "🎲" : "📦";
                QString cardMsg = QString("%1 %2 drew %3 card: %4")
                                  .arg(cardEmoji).arg(playerName).arg(cardType).arg(cardDescription);
                addGameLog(cardMsg);
                
                if (cardAmount > 0) {
                    addGameLog(QString("💰 %1 received $%2 from card")
                              .arg(playerName).arg(cardAmount));
                } else if (cardAmount < 0) {
                    addGameLog(QString("💸 %1 paid $%2 from card")
                              .arg(playerName).arg(-cardAmount));
                }
                
                if (cardTargetTile >= 0) {
                    addGameLog(QString("📍 %1 moved to position %2 from card")
                              .arg(playerName).arg(cardTargetTile));
                }
                
                if (cardStep != 0) {
                    addGameLog(QString("📍 %1 moved %2 spaces from card")
                              .arg(playerName).arg(cardStep > 0 ? QString("+%1").arg(cardStep) : QString::number(cardStep)));
                }
                
                if (cardGetOutOfJail) {
                    addGameLog(QString("🎫 %1 received Get Out of Jail Free card")
                              .arg(playerName));
                }
                
                if (cardGoToJail) {
                    addGameLog(QString("🚨 %1 goes to jail from card")
                              .arg(playerName));
                }
            }
            
            addGameLog(QString("%1 moved to position %2").arg(playerName).arg(newPosition));

            if (m_boardWidget) {
                m_boardWidget->update();
                m_boardWidget->repaint();
            }
        } else if (topic == "game.buy") {

            int playerId = event.payload["playerId"].toInt();
            int propertyId = event.payload["propertyId"].toInt();
            int cost = event.payload["cost"].toInt();
            int cash = event.payload["cash"].toInt();
            bool success = event.payload["success"].toBool(true);
            
            QString playerName = "Player " + QString::number(playerId);
            for (const auto& p : m_lastSnapshot.players) {
                if (p.playerId == playerId) {
                    playerName = QString::fromStdString(p.displayName);
                    break;
                }
            }
            
            if (success) {
                addGameLog(QString("✓ %1 successfully bought property %2 for $%3. Remaining cash: $%4")
                    .arg(playerName).arg(propertyId).arg(cost).arg(cash));
            }
        } else if (topic == "game.hint") {
            
            int playerId = event.payload["playerId"].toInt();
            int cost = event.payload["cost"].toInt();
            int cash = event.payload["cash"].toInt();
            
            int myLocalIndex = getMyLocalPlayerIndex();
            int serverPlayerId = getServerPlayerId(myLocalIndex);
            
            if (playerId == serverPlayerId) {
                addGameLog(QString("AI hint used. $%1 deducted. Remaining cash: $%2")
                    .arg(cost).arg(cash));
            } else {
                addGameLog(QString("Player %1 used AI hint ($%2)")
                    .arg(playerId).arg(cost));
            }
        } else if (topic == "game.sell") {
            int playerId = event.payload["playerId"].toInt();
            int propertyId = event.payload["propertyId"].toInt();
            int price = event.payload["price"].toInt();
            int cash = event.payload["cash"].toInt();
            
            int myLocalIndex = getMyLocalPlayerIndex();
            int serverPlayerId = getServerPlayerId(myLocalIndex);
            
            if (playerId == serverPlayerId) {
                addGameLog(QString("✓ Successfully sold property %1 for $%2. Remaining cash: $%3")
                    .arg(propertyId).arg(price).arg(cash));
            } else {
                QString playerName = "Player " + QString::number(playerId);
                for (const auto& p : m_lastSnapshot.players) {
                    if (p.playerId == playerId) {
                        playerName = QString::fromStdString(p.displayName);
                        break;
                    }
                }
                addGameLog(QString("%1 sold property %2 for $%3")
                    .arg(playerName).arg(propertyId).arg(price));
            }
            
            if (m_boardWidget) {
                m_boardWidget->update();
                m_boardWidget->repaint();
            }
        } else if (topic == "game.turn_ended") {
            int playerId = event.payload["playerId"].toInt();
            int nextPlayerId = event.payload["nextPlayerId"].toInt();
            
            QString playerName = "Player " + QString::number(playerId);
            QString nextPlayerName = "Player " + QString::number(nextPlayerId);
            for (const auto& p : m_lastSnapshot.players) {
                if (p.playerId == playerId) {
                    playerName = QString::fromStdString(p.displayName);
                }
                if (p.playerId == nextPlayerId) {
                    nextPlayerName = QString::fromStdString(p.displayName);
                }
            }
            
            if (event.payload.contains("jailTurnsRemaining")) {
                int jailTurnsRemaining = event.payload["jailTurnsRemaining"].toInt(-1);
                if (jailTurnsRemaining >= 0) {
                    addGameLog(QString("🔒 %1 ended turn, still in jail (%2 turns remaining). Next player: %3")
                        .arg(playerName).arg(jailTurnsRemaining).arg(nextPlayerName));
                } else {
                    addGameLog(QString("%1 ended turn. Next player: %2")
                        .arg(playerName).arg(nextPlayerName));
                }
            } else {
                addGameLog(QString("%1 ended turn. Next player: %2")
                    .arg(playerName).arg(nextPlayerName));
            }
        } else if (topic == "game.jail_roll") {
            int playerId = event.payload["playerId"].toInt();
            int dice1 = event.payload["dice1"].toInt();
            int dice2 = event.payload["dice2"].toInt();
            int total = event.payload["total"].toInt(dice1 + dice2);
            bool isDouble = event.payload["isDouble"].toBool(false);
            bool gotOutOfJail = event.payload["gotOutOfJail"].toBool(false);
            int jailStatus = event.payload["jailStatus"].toInt(0);
            int jailTurnsRemaining = event.payload["jailTurnsRemaining"].toInt(0);
            
            QString playerName = "Player " + QString::number(playerId);
            for (const auto& p : m_lastSnapshot.players) {
                if (p.playerId == playerId) {
                    playerName = QString::fromStdString(p.displayName);
                    break;
                }
            }
            
            if (gotOutOfJail) {
                addGameLog(QString("🎲 %1 rolled double (%2,%3) and got out of jail!")
                          .arg(playerName).arg(dice1).arg(dice2));
            } else {
                addGameLog(QString("🔒 %1 rolled %2+%3=%4 (not double), stays in jail (%5 turns remaining)")
                          .arg(playerName).arg(dice1).arg(dice2).arg(total).arg(jailTurnsRemaining));
            }
        } else if (topic == "game.jail_released") {
            int playerId = event.payload["playerId"].toInt();
            QString method = event.payload["method"].toString();
            int cash = event.payload["cash"].toInt();
            
            QString playerName = "Player " + QString::number(playerId);
            for (const auto& p : m_lastSnapshot.players) {
                if (p.playerId == playerId) {
                    playerName = QString::fromStdString(p.displayName);
                    break;
                }
            }
            
            if (method == "pay_bail") {
                int bailAmount = event.payload["bailAmount"].toInt(50);
                addGameLog(QString("💰 %1 paid $%2 bail and got out of jail. Remaining cash: $%3")
                          .arg(playerName).arg(bailAmount).arg(cash));
            } else {
                addGameLog(QString("✅ %1 got out of jail").arg(playerName));
            }
        } else {
            std::cout << "[MainWindow] WARNING: Unhandled event topic: " << topic.toStdString() << std::endl;
        }
    });
    
    m_client->setOnChat([this](int playerId, const QString& playerName, const QString& message, bool isPrivate, int targetPlayerId) {

        int myLocalIndex = getMyLocalPlayerIndex();
        int myServerPlayerId = getServerPlayerId(myLocalIndex);
        bool isMyMessage = (playerId == myServerPlayerId);
        bool isToMe = (targetPlayerId == myServerPlayerId);

        if (isPrivate && !isMyMessage && !isToMe) {
            return;
        }

        QString formattedMsg;
        QString prefix = "";
        if (isPrivate) {
            if (isMyMessage) {
                prefix = "🔒 [Private to ";
                // 查找目标玩家名称
                QString targetName = "Unknown";
                for (const auto& [localIdx, serverId] : m_localToServerPlayerId) {
                    if (serverId == targetPlayerId && localIdx >= 0 && localIdx < m_game->getPlayerCount()) {
                        targetName = QString::fromStdString(m_game->getPlayer(localIdx).getName());
                        break;
                    }
                }
                prefix += targetName + "] ";
            } else if (isToMe) {
                prefix = "🔒 [Private from " + playerName + "] ";
            }
        }
        
        if (isMyMessage) {
            formattedMsg = QString("<div style='margin: 5px 0; padding: 8px; background-color: %1; border-left: 4px solid %2; border-radius: 4px;'>"
                                   "<b style='color: %2;'>%3You</b>: %4</div>")
                           .arg(isPrivate ? "#fff3e0" : "#e3f2fd")  // 私聊用橙色背景，群发用蓝色
                           .arg(isPrivate ? "#ff9800" : "#2196f3")
                           .arg(prefix)
                           .arg(message.toHtmlEscaped());
        } else {
            // 如果玩家名称为空，使用备用名称
            QString displayName = playerName;
            if (displayName.isEmpty()) {
                displayName = QString("Player %1").arg(playerId);
            }
            formattedMsg = QString("<div style='margin: 5px 0; padding: 8px; background-color: %1; border-left: 4px solid %2; border-radius: 4px;'>"
                                   "<b style='color: %2;'>%3%4</b>: %5</div>")
                           .arg(isPrivate ? "#fff3e0" : "#f5f5f5")
                           .arg(isPrivate ? "#ff9800" : "#757575")
                           .arg(prefix)
                           .arg(displayName.toHtmlEscaped())
                           .arg(message.toHtmlEscaped());
        }
        
        // 添加到游戏日志（简化格式）
        QString displayName = playerName;
        if (displayName.isEmpty()) {
            displayName = QString("Player %1").arg(playerId);
        }
        QString logMsg = prefix + QString("[%1]: %2").arg(displayName).arg(message);
        addGameLog(logMsg);
        
        // 添加到聊天室
        if (m_chatMessages) {
            m_chatMessages->append(formattedMsg);
            QTextCursor cursor = m_chatMessages->textCursor();
            cursor.movePosition(QTextCursor::End);
            m_chatMessages->setTextCursor(cursor);
            
            // 自动滚动到底部
            QScrollBar* scrollBar = m_chatMessages->verticalScrollBar();
            if (scrollBar) {
                scrollBar->setValue(scrollBar->maximum());
            }
        }
    });
}

void OnlineMainWindow::setupGameConnections() {
    connect(m_game, &Game::playerTurnStarted, this, &OnlineMainWindow::onPlayerTurnStarted);
    connect(m_game, &Game::playerTurnEnded, this, &OnlineMainWindow::onPlayerTurnEnded);
    connect(m_game, &Game::diceRolled, this, &OnlineMainWindow::onDiceRolled);
    connect(m_game, &Game::playerMoved, this, &OnlineMainWindow::onPlayerMoved);
    connect(m_game, &Game::purchaseOpportunity, this, &OnlineMainWindow::onPurchaseOpportunity);
    connect(m_game, &Game::gameLogMessage, this, &OnlineMainWindow::onGameLogMessage);
    connect(m_game, &Game::playerBankrupt, this, &OnlineMainWindow::onPlayerBankrupt);
    connect(m_game, &Game::rentPaymentRequired, this, &OnlineMainWindow::onRentPaymentRequired);
    connect(m_game, &Game::forceRaiseMoney, this, &OnlineMainWindow::onForceRaiseMoney);
}

void OnlineMainWindow::showRoomDialog() {
    if (m_roomDialog) {
        m_roomDialog->exec();
    }
}


void OnlineMainWindow::onConnectToServer() {
    QString serverUrl = m_serverUrlEdit->text();
    if (serverUrl.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter server URL");
        return;
    }
    m_client->connectToServer(serverUrl);
}

void OnlineMainWindow::onCreateRoom() {
    QString roomId = m_roomIdEdit->text();
    if (roomId.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter room ID");
        return;
    }
    m_client->createRoom(roomId, 4);
    addGameLog(QString("Creating room: %1").arg(roomId));
}

void OnlineMainWindow::onJoinRoom() {
    QString roomId = m_roomIdEdit->text();
    QString playerName = m_playerNameEdit->text();
    if (roomId.isEmpty() || playerName.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter room ID and player name");
        return;
    }
    m_client->joinRoom(roomId, playerName, false);
    addGameLog(QString("Joining room: %1 as %2").arg(roomId).arg(playerName));
}

void OnlineMainWindow::onRefreshRoomList() {
    m_client->listRooms();
}

void OnlineMainWindow::onRoomSelected() {
    QListWidgetItem* item = m_roomListWidget->currentItem();
    if (item) {
        QString text = item->text();
        QString roomId = text.split(" ")[0];
        m_roomIdEdit->setText(roomId);
    }
}


void OnlineMainWindow::onRollDiceClicked() {
    std::cout << "[MainWindow] ===== Roll Dice clicked =====" << std::endl;
    std::cout << "[MainWindow] m_myPlayerId: " << m_myPlayerId << std::endl;
    std::cout << "[MainWindow] m_lastSnapshot.phase: " << static_cast<int>(m_lastSnapshot.phase) << std::endl;
    std::cout << "[MainWindow] m_lastSnapshot.currentTurnIndex: " << m_lastSnapshot.currentTurnIndex << std::endl;
    std::cout << "[MainWindow] m_lastSnapshot.players.size(): " << m_lastSnapshot.players.size() << std::endl;
    
    bool myTurn = isMyTurn();
    bool canRoll = m_game ? m_game->canRollDice() : false;
    
    std::cout << "[MainWindow] isMyTurn(): " << myTurn << std::endl;
    std::cout << "[MainWindow] m_game->canRollDice(): " << canRoll << std::endl;
    std::cout << "[MainWindow] m_hasRolledDiceThisTurn: " << m_hasRolledDiceThisTurn << std::endl;
    
    if (!myTurn) {
        std::cout << "[MainWindow] Not my turn, showing message" << std::endl;
        QMessageBox::information(this, "Not Your Turn", "Please wait for your turn");
        return;
    }
    if (m_hasRolledDiceThisTurn) {
        std::cout << "[MainWindow] Already rolled dice this turn" << std::endl;
        QMessageBox::information(this, "Already Rolled", "You can only roll dice once per turn");
        return;
    }
    if (!canRoll) {
        std::cout << "[MainWindow] Cannot roll dice, showing message" << std::endl;
        QMessageBox::information(this, "Cannot Roll", "You cannot roll dice now");
        return;
    }
    
    std::cout << "[MainWindow] All checks passed, sending roll dice request to server" << std::endl;
    m_client->rollDice();
    addGameLog("Rolling dice...");
    

    m_hasRolledDiceThisTurn = true;
    

    if (m_rollButton) {
        m_rollButton->setEnabled(false);
        std::cout << "[MainWindow] Roll button disabled, waiting for server response" << std::endl;
    }
}

void OnlineMainWindow::onBuyPropertyClicked() {
    std::cout << "[MainWindow] Buy Property clicked" << std::endl;
    
    if (!isMyTurn()) {
        QMessageBox::information(this, "Not Your Turn", "Please wait for your turn");
        return;
    }
    
    int myLocalIndex = getMyLocalPlayerIndex();
    if (myLocalIndex < 0) {
        QMessageBox::warning(this, "Error", "Player not found");
        return;
    }
    
    const Player& player = m_game->getPlayer(myLocalIndex);
    int position = player.getPosition();
   
    Board& board = m_game->getBoard();
    Tile& tile = board.getTile(position);
    

    PropertyTile* propertyTile = dynamic_cast<PropertyTile*>(&tile);
    if (!propertyTile) {
        QMessageBox::information(this, "Cannot Buy", "This tile is not a property and cannot be purchased.");
        return;
    }
    

    if (propertyTile->getOwner() != nullptr) {
        QMessageBox::information(this, "Cannot Buy", "This property is already owned.");
        return;
    }
    
    int price = propertyTile->getPrice();
    if (player.getCash() < price) {
        QMessageBox::warning(this, "Insufficient Cash", 
            QString("You need $%1 to buy this property, but you only have $%2")
                .arg(price).arg(player.getCash()));
        return;
    }
    

    QString propertyName = QString::fromStdString(propertyTile->getName());
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Purchase Property",
        QString("Do you want to buy %1 for $%2?\n\nYour current cash: $%3")
            .arg(propertyName).arg(price).arg(player.getCash()),
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {

        std::cout << "[MainWindow] Sending buy property request: position=" << position 
                  << ", price=" << price << std::endl;
        m_client->buyProperty(QString::number(position), price);
        addGameLog(QString("Requesting to buy %1 at position %2 for $%3")
            .arg(propertyName).arg(position).arg(price));
    } else {
        addGameLog("Purchase cancelled.");
    }
}

void OnlineMainWindow::onSellPropertyClicked() {
    std::cout << "[MainWindow] ===== Sell Property button clicked =====" << std::endl;
    std::cout << "[MainWindow] Button enabled: " << (m_sellPropertyButton ? m_sellPropertyButton->isEnabled() : false) << std::endl;
    std::cout << "[MainWindow] m_client: " << (m_client ? "exists" : "null") << std::endl;
    std::cout << "[MainWindow] m_client->isConnected(): " << (m_client ? m_client->isConnected() : false) << std::endl;
    std::cout << "[MainWindow] m_game: " << (m_game ? "exists" : "null") << std::endl;
    

    if (!m_client || !m_client->isConnected()) {
        std::cout << "[MainWindow] ERROR: Not connected to server" << std::endl;
        QMessageBox::warning(this, "Error", "Not connected to server. Please reconnect.");
        return;
    }
    
    if (!m_game) {
        std::cout << "[MainWindow] ERROR: Game not initialized" << std::endl;
        QMessageBox::warning(this, "Error", "Game not initialized.");
        return;
    }
    
    
    int myLocalIndex = getMyLocalPlayerIndex();
    if (myLocalIndex < 0) {
        QMessageBox::warning(this, "Error", "Player not found");
        return;
    }
    
    const Player& player = m_game->getPlayer(myLocalIndex);
    std::vector<PropertyTile*> properties = player.getProperties();
    
    if (properties.empty()) {
        QMessageBox::information(this, "No Properties", "You don't have any properties to sell.");
        return;
    }
    

    QStringList propertyList;
    QMap<QString, PropertyTile*> propertyMap;
    for (PropertyTile* prop : properties) {
        QString propertyInfo = QString("%1 (Position %2, Price: $%3)")
            .arg(QString::fromStdString(prop->getName()))
            .arg(prop->getIndex())
            .arg(prop->getPrice());
        propertyList << propertyInfo;
        propertyMap[propertyInfo] = prop;
    }
    
    bool ok;
    QString selected = QInputDialog::getItem(this, "Sell Property",
        "Select a property to sell:", propertyList, 0, false, &ok);
    
    if (!ok || selected.isEmpty()) {
        addGameLog("Sell cancelled.");
        return;
    }
    
    PropertyTile* selectedProperty = propertyMap[selected];
    if (!selectedProperty) {
        QMessageBox::warning(this, "Error", "Invalid property selected");
        return;
    }
    
    int sellPrice = selectedProperty->getPrice() / 2;
    
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Sell Property",
        QString("Do you want to sell %1 for $%2?\n\nYour current cash: $%3")
            .arg(QString::fromStdString(selectedProperty->getName()))
            .arg(sellPrice)
            .arg(player.getCash()),
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        if (!m_client || !m_client->isConnected()) {
            QMessageBox::warning(this, "Error", "Not connected to server. Please reconnect.");
            return;
        }
        

        std::cout << "[MainWindow] Sending sell property request: propertyId=" 
                  << selectedProperty->getIndex() << ", price=" << sellPrice << std::endl;
        m_client->sellProperty(QString::number(selectedProperty->getIndex()), sellPrice);
        addGameLog(QString("Requesting to sell %1 (position %2) for $%3")
            .arg(QString::fromStdString(selectedProperty->getName()))
            .arg(selectedProperty->getIndex())
            .arg(sellPrice));
    } else {
        addGameLog("Sell cancelled.");
    }
}

void OnlineMainWindow::onEndTurnClicked() {
    if (!isMyTurn()) {
        QMessageBox::information(this, "Not Your Turn", "Please wait for your turn");
        return;
    }
    m_client->endTurn();
    addGameLog("Ending turn...");
}

void OnlineMainWindow::onHintClicked() {
    std::cout << "[MainWindow] Hint clicked" << std::endl;
    
    if (!isMyTurn()) {
        QMessageBox::information(this, "Not Your Turn", "Please wait for your turn");
        return;
    }
    
    if (m_hasUsedHintThisTurn) {
        QMessageBox::information(this, "Already Used", "You can only use AI hint once per turn");
        return;
    }
    
    int myLocalIndex = getMyLocalPlayerIndex();
    if (myLocalIndex < 0) {
        std::cout << "[MainWindow] Hint: Invalid player index" << std::endl;
        return;
    }
    
    const Player& player = m_game->getPlayer(myLocalIndex);
    if (player.getCash() < 50) {
        QMessageBox::warning(this, "Insufficient Cash", "You need at least $50 to use AI hint");
        return;
    }
    

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "AI Hint",
        QString("Use AI hint for $50?\n\nYour current cash: $%1").arg(player.getCash()),
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {

        StrategyEngine strategyEngine(m_game);
        QString hintResult = strategyEngine.getHintResultforQt(player);

        QMessageBox hintBox(this);
        hintBox.setWindowTitle("AI Hint Result");
        hintBox.setTextFormat(Qt::RichText);
        hintBox.setText(hintResult);
        hintBox.setStandardButtons(QMessageBox::Ok);
        hintBox.exec();

        std::cout << "[MainWindow] Sending hint request to server" << std::endl;
        m_client->useHint(50);
        addGameLog("AI hint used. $50 deducted.");

        m_hasUsedHintThisTurn = true;
        

        if (m_hintButton) {
            m_hintButton->setEnabled(false);
        }
    } else {
        addGameLog("AI hint cancelled.");
    }
}

void OnlineMainWindow::onStartGameClicked() {
    m_client->startGame();
    addGameLog("Requesting to start game...");
    if (m_startGameButton) {
        m_startGameButton->setEnabled(false);
        m_startGameButton->setText("Starting...");
    }
    if (m_statusLabel) {
        m_statusLabel->setText("Requesting to start game...");
    }
}

void OnlineMainWindow::onChatroomClicked() {
    if (m_chatDialog) {
        m_chatDialog->show();
        m_chatDialog->raise();
        m_chatDialog->activateWindow();
    }
}

void OnlineMainWindow::onPayBailClicked() {
    if (!m_client || !m_client->isConnected()) {
        QMessageBox::warning(this, "Error", "Not connected to server. Please reconnect.");
        return;
    }
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Pay Bail",
        QString("Do you want to pay $50 to get out of jail?"),
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        m_client->payBail();
        addGameLog("Requesting to pay $50 bail...");
    }
}

void OnlineMainWindow::onViewPropertyClicked() {
    std::cout << "[MainWindow] ===== View Property clicked =====" << std::endl;
    std::cout << "[MainWindow] m_propertyDialog: " << (m_propertyDialog ? "exists" : "null") << std::endl;
    std::cout << "[MainWindow] m_game: " << (m_game ? "exists" : "null") << std::endl;
    
    if (!m_propertyDialog) {
        std::cout << "[MainWindow] ERROR: m_propertyDialog is null! Creating it now..." << std::endl;
        setupPropertyDialog();
    }
    
    if (!m_game) {
        std::cout << "[MainWindow] ERROR: m_game is null!" << std::endl;
        QMessageBox::warning(this, "Error", "Game not initialized. Please join a room first.");
        return;
    }
    
    if (m_client && m_client->isConnected()) {
        std::cout << "[MainWindow] Sending view property request to server" << std::endl;
        m_client->viewProperty();
    } else {
        std::cout << "[MainWindow] WARNING: Not connected to server, skipping server request" << std::endl;
    }

    QTextEdit* propertyText = m_propertyDialog->property("propertyText").value<QTextEdit*>();
    if (!propertyText) {
        std::cout << "[MainWindow] ERROR: propertyText is null!" << std::endl;
        QMessageBox::warning(this, "Error", "Property dialog not properly initialized.");
        return;
    }
    
    std::cout << "[MainWindow] Generating property information..." << std::endl;
    

    QString html = "<html><body style='font-family: Arial, sans-serif;'>";
    html += "<h2 style='color: #2c3e50; margin-bottom: 20px;'>🏠 Property Overview</h2>";
    
    Board& board = m_game->getBoard();
    int playerCount = m_game->getPlayerCount();
    

    for (int i = 0; i < playerCount; i++) {
        const Player& player = m_game->getPlayer(i);
        std::vector<PropertyTile*> properties = player.getProperties();
        

        QString playerName = QString::fromStdString(player.getName());
        int serverPlayerId = getServerPlayerId(i);
        if (serverPlayerId >= 0 && !m_lastSnapshot.players.empty()) {
            for (const auto& p : m_lastSnapshot.players) {
                if (p.playerId == serverPlayerId) {
                    playerName = QString::fromStdString(p.displayName);
                    break;
                }
            }
        }
        
        html += QString("<div style='margin-bottom: 25px; padding: 15px; background-color: %1; border-left: 4px solid %2; border-radius: 4px;'>")
                .arg(i == getMyLocalPlayerIndex() ? "#e8f5e9" : "#f5f5f5")
                .arg(i == getMyLocalPlayerIndex() ? "#4caf50" : "#757575");
        
        html += QString("<h3 style='color: %1; margin-top: 0;'>👤 %2 (Cash: $%3)</h3>")
                .arg(i == getMyLocalPlayerIndex() ? "#2e7d32" : "#424242")
                .arg(playerName.toHtmlEscaped())
                .arg(player.getCash());
        
        if (properties.empty()) {
            html += "<p style='color: #757575; font-style: italic;'>No properties owned.</p>";
        } else {
            html += "<table style='width: 100%; border-collapse: collapse; margin-top: 10px;'>";
            html += "<tr style='background-color: #e0e0e0;'>";
            html += "<th style='padding: 8px; text-align: left; border: 1px solid #bdbdbd;'>Property</th>";
            html += "<th style='padding: 8px; text-align: left; border: 1px solid #bdbdbd;'>Position</th>";
            html += "<th style='padding: 8px; text-align: right; border: 1px solid #bdbdbd;'>Price</th>";
            html += "<th style='padding: 8px; text-align: right; border: 1px solid #bdbdbd;'>Houses</th>";
            html += "<th style='padding: 8px; text-align: right; border: 1px solid #bdbdbd;'>Rent</th>";
            html += "<th style='padding: 8px; text-align: center; border: 1px solid #bdbdbd;'>Status</th>";
            html += "</tr>";
            
            int totalValue = 0;
            for (const auto& prop : properties) {
                totalValue += prop->getPrice();
                if (prop->getHouses() > 0) {
                    totalValue += prop->getHouses() * prop->getHousePrice();
                }
                
                std::array<int, 6> rent = prop->getRent();
                int currentRent = prop->calculateRent(0);
                
                html += "<tr>";
                html += QString("<td style='padding: 6px; border: 1px solid #e0e0e0;'><b>%1</b><br><small style='color: #757575;'>%2</small></td>")
                        .arg(QString::fromStdString(prop->getName()).toHtmlEscaped())
                        .arg(QString::fromStdString(prop->getGroup()).toHtmlEscaped());
                html += QString("<td style='padding: 6px; border: 1px solid #e0e0e0;'>%1</td>").arg(prop->getIndex());
                html += QString("<td style='padding: 6px; border: 1px solid #e0e0e0; text-align: right;'>$%1</td>").arg(prop->getPrice());
                html += QString("<td style='padding: 6px; border: 1px solid #e0e0e0; text-align: right;'>%1</td>").arg(prop->getHouses());
                html += QString("<td style='padding: 6px; border: 1px solid #e0e0e0; text-align: right;'>$%1</td>").arg(currentRent);
                
                QString status = "";
                if (prop->isMortgaged()) {
                    status = "🔒 Mortgaged";
                } else if (prop->getHouses() > 0) {
                    status = QString("🏠 %1 house%2").arg(prop->getHouses()).arg(prop->getHouses() > 1 ? "s" : "");
                } else {
                    status = "✓ Owned";
                }
                html += QString("<td style='padding: 6px; border: 1px solid #e0e0e0; text-align: center;'>%1</td>").arg(status);
                html += "</tr>";
            }
            
            html += "</table>";
            html += QString("<p style='margin-top: 10px; color: #424242;'><b>Total Property Value: $%1</b></p>").arg(totalValue);
        }
        
        html += "</div>";
    }
    

    html += "<div style='margin-top: 30px; padding: 15px; background-color: #fff3e0; border-left: 4px solid #ff9800; border-radius: 4px;'>";
    html += "<h3 style='color: #e65100; margin-top: 0;'>🏘️ Available Properties</h3>";
    html += "<table style='width: 100%; border-collapse: collapse;'>";
    html += "<tr style='background-color: #ffe0b2;'>";
    html += "<th style='padding: 8px; text-align: left; border: 1px solid #ffcc80;'>Property</th>";
    html += "<th style='padding: 8px; text-align: left; border: 1px solid #ffcc80;'>Position</th>";
    html += "<th style='padding: 8px; text-align: right; border: 1px solid #ffcc80;'>Price</th>";
    html += "<th style='padding: 8px; text-align: center; border: 1px solid #ffcc80;'>Owner</th>";
    html += "</tr>";
    
    bool hasAvailable = false;
    for (int i = 0; i < board.getSize(); i++) {
        Tile& tile = board.getTile(i);
        PropertyTile* prop = dynamic_cast<PropertyTile*>(&tile);
        if (prop) {
            Player* owner = prop->getOwner();
            if (!owner) {
                hasAvailable = true;
                html += "<tr>";
                html += QString("<td style='padding: 6px; border: 1px solid #ffe0b2;'><b>%1</b><br><small style='color: #757575;'>%2</small></td>")
                        .arg(QString::fromStdString(prop->getName()).toHtmlEscaped())
                        .arg(QString::fromStdString(prop->getGroup()).toHtmlEscaped());
                html += QString("<td style='padding: 6px; border: 1px solid #ffe0b2;'>%1</td>").arg(i);
                html += QString("<td style='padding: 6px; border: 1px solid #ffe0b2; text-align: right;'>$%1</td>").arg(prop->getPrice());
                html += "<td style='padding: 6px; border: 1px solid #ffe0b2; text-align: center; color: #4caf50;'><b>Available</b></td>";
                html += "</tr>";
            }
        }
    }
    
    if (!hasAvailable) {
        html += "<tr><td colspan='4' style='padding: 10px; text-align: center; color: #757575; font-style: italic;'>All properties are owned.</td></tr>";
    }
    
    html += "</table>";
    html += "</div>";
    
    html += "</body></html>";
    
    propertyText->setHtml(html);
    
    std::cout << "[MainWindow] Property information generated, showing dialog..." << std::endl;
    

    m_propertyDialog->show();
    m_propertyDialog->raise();
    m_propertyDialog->activateWindow();
    
    std::cout << "[MainWindow] ===== View Property dialog shown =====" << std::endl;
}

void OnlineMainWindow::onSendChatMessage() {
    QString message = m_chatInput->text().trimmed();
    if (message.isEmpty()) {
        return;
    }
    

    if (!m_client->isConnected()) {
        QMessageBox::warning(this, "Not Connected", "You are not connected to the server. Please connect first.");
        return;
    }
    

    if (m_currentRoomId.isEmpty()) {
        QMessageBox::warning(this, "Not in Room", "You are not in a room. Please join a room first.");
        return;
    }
    
    
    int targetPlayerId = -1;
    if (m_chatModePrivate && m_chatModePrivate->isChecked()) {

        if (m_chatTargetCombo && m_chatTargetCombo->currentIndex() > 0) {

            QVariant data = m_chatTargetCombo->currentData();
            if (data.isValid()) {
                targetPlayerId = data.toInt();
            } else {
                QMessageBox::warning(this, "Invalid Target", "Please select a player to send private message.");
                return;
            }
        } else {
            QMessageBox::warning(this, "No Target", "Please select a player to send private message.");
            return;
        }
    }
    
    std::cout << "[MainWindow] Sending chat message: " << message.toStdString();
    if (targetPlayerId >= 0) {
        std::cout << " (private to player " << targetPlayerId << ")";
    } else {
        std::cout << " (group chat)";
    }
    std::cout << std::endl;
    
    m_client->sendChat(message, targetPlayerId);
    m_chatInput->clear();
    

    if (m_chatInput) {
        m_chatInput->setFocus();
    }
}

void OnlineMainWindow::onConnected() {
    addGameLog("Connected to server");
}

void OnlineMainWindow::onDisconnected() {
    addGameLog("Disconnected from server");
}

void OnlineMainWindow::onError(const QString& error) {
    addGameLog("Error: " + error);
    QMessageBox::warning(this, "Error", error);
}

void OnlineMainWindow::onJoinedRoom(const QString& roomId, int playerId, const monopoly::StateSnapshot& snapshot) {
}

void OnlineMainWindow::onRoomListReceived(const std::vector<monopoly::RoomSummary>& rooms) {
}

void OnlineMainWindow::onStateUpdate(const monopoly::StateSnapshot& snapshot) {
}

void OnlineMainWindow::onGameEvent(const monopoly::GameEvent& event) {
}

void OnlineMainWindow::onChatReceived(int playerId, const QString& playerName, const QString& message) {
}


void OnlineMainWindow::onPlayerTurnStarted(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= m_game->getPlayerCount()) return;
    
    const Player& player = m_game->getPlayer(playerIndex);
    int myLocalIndex = getMyLocalPlayerIndex();
    bool isMyTurn = (playerIndex == myLocalIndex);
    
    if (m_statusLabel) {
        if (isMyTurn) {
            m_statusLabel->setText(QString("Your turn - %1").arg(QString::fromStdString(player.getName())));
        } else {
            m_statusLabel->setText(QString("%1's turn").arg(QString::fromStdString(player.getName())));
        }
    }
    

    if (isMyTurn) {
        int myServerPlayerId = getServerPlayerId(playerIndex);
        if (myServerPlayerId != m_lastTurnPlayerId) {
            m_hasRolledDiceThisTurn = false;
            m_hasUsedHintThisTurn = false;
            m_lastTurnPlayerId = myServerPlayerId;
            std::cout << "[MainWindow] onPlayerTurnStarted: My turn started, reset roll dice and hint flags" << std::endl;
        }
    }
    
    if (m_rollButton) {
        m_rollButton->setEnabled(isMyTurn && m_game->canRollDice() && !m_hasRolledDiceThisTurn);
    }
    
    if (m_hintButton) {
        m_hintButton->setEnabled(isMyTurn && !m_hasUsedHintThisTurn);
    }

    if (m_buyPropertyButton) {
        bool canBuy = false;
        if (isMyTurn) {
            int myLocalIndex = getMyLocalPlayerIndex();
            if (myLocalIndex >= 0) {
                const Player& player = m_game->getPlayer(myLocalIndex);
                int position = player.getPosition();
                Board& board = m_game->getBoard();
                Tile& tile = board.getTile(position);
                PropertyTile* propertyTile = dynamic_cast<PropertyTile*>(&tile);
                if (propertyTile && propertyTile->getOwner() == nullptr) {
                    canBuy = player.getCash() >= propertyTile->getPrice();
                }
            }
        }
        m_buyPropertyButton->setEnabled(canBuy);
        std::cout << "[MainWindow] Buy property button enabled: " << canBuy << std::endl;
    }
    
    if (m_hintButton) {
        m_hintButton->setEnabled(isMyTurn && !m_hasUsedHintThisTurn);
        std::cout << "[MainWindow] Hint button enabled: " << (isMyTurn && !m_hasUsedHintThisTurn) << std::endl;
    }

    if (m_sellPropertyButton) {
        bool canSell = false;
        int myLocalIndex = getMyLocalPlayerIndex();
        if (myLocalIndex >= 0) {
            const Player& player = m_game->getPlayer(myLocalIndex);
            std::vector<PropertyTile*> properties = player.getProperties();
            canSell = !properties.empty();
        }
        m_sellPropertyButton->setEnabled(canSell);
        std::cout << "[MainWindow] Sell property button enabled: " << canSell << " (myLocalIndex=" << myLocalIndex << ")" << std::endl;
    }

    if (m_payBailButton) {
        bool canPayBail = false;
        if (isMyTurn) {
            int myLocalIndex = getMyLocalPlayerIndex();
            if (myLocalIndex >= 0 && myLocalIndex < m_game->getPlayerCount()) {
                const Player& player = m_game->getPlayer(myLocalIndex);
                int jailStatus = player.getJailStatus();
                bool inJail = (jailStatus >= 0);
                bool hasEnoughCash = (player.getCash() >= 50);
                canPayBail = inJail && hasEnoughCash;
            }
        }
        m_payBailButton->setEnabled(canPayBail);
    }
    
    if (isMyTurn) {
        addGameLog("It's your turn! Click 'Roll Dice' to start.");
    }
}

void OnlineMainWindow::onPlayerTurnEnded(int playerIndex) {
    if (m_rollButton) {
        m_rollButton->setEnabled(false);
    }
    if (m_buyPropertyButton) {
        m_buyPropertyButton->setEnabled(false);
    }
    if (m_sellPropertyButton) {
        m_sellPropertyButton->setEnabled(false);
    }
    if (m_hintButton) {
        m_hintButton->setEnabled(false);
    }
    if (m_endTurnButton) {
        m_endTurnButton->setEnabled(false);
    }
    if (m_payBailButton) {
        m_payBailButton->setEnabled(false);
    }
}

void OnlineMainWindow::onDiceRolled(int playerIndex, int diceValue) {
    if (playerIndex < 0 || playerIndex >= m_game->getPlayerCount()) return;
    const Player& player = m_game->getPlayer(playerIndex);
    if (m_statusLabel) {
        m_statusLabel->setText(QString("%1 rolled %2").arg(QString::fromStdString(player.getName())).arg(diceValue));
    }
}

void OnlineMainWindow::onPlayerMoved(int playerIndex, int oldPosition, int newPosition) {
    if (m_boardWidget) {
        m_boardWidget->update();
    }
}

void OnlineMainWindow::onPurchaseOpportunity(const QString& propertyName, int price, int playerIndex) {
    int myLocalIndex = getMyLocalPlayerIndex();
    if (playerIndex != myLocalIndex) return;
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Purchase Property",
        QString("Do you want to buy %1 for $%2?").arg(propertyName).arg(price),
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        m_client->buyProperty(propertyName, price);
    }
}

void OnlineMainWindow::onGameLogMessage(const QString& message) {
    addGameLog(message);
}

void OnlineMainWindow::onPlayerBankrupt(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= m_game->getPlayerCount()) return;
    const Player& player = m_game->getPlayer(playerIndex);
    addGameLog(QString("%1 is bankrupt!").arg(QString::fromStdString(player.getName())));
}

void OnlineMainWindow::onRentPaymentRequired(const QString& propertyName, int rent, int fromPlayer, int toPlayer) {
    QMessageBox::information(this, "Rent Payment", 
        QString("You need to pay $%1 rent for %2").arg(rent).arg(propertyName));
}

void OnlineMainWindow::onForceRaiseMoney(int payerIndex, int amountDue) {
    int myLocalIndex = getMyLocalPlayerIndex();
    if (payerIndex != myLocalIndex) return;
    
    QMessageBox::warning(this, "Insufficient Funds",
        QString("You need to raise $%1. Please sell or mortgage properties.").arg(amountDue));
}

void OnlineMainWindow::syncGameStateFromNetwork(const monopoly::StateSnapshot& snapshot) {
    if (!m_game) return;
    
    syncPlayersFromSnapshot(snapshot);
    
    int currentTurnPlayerId = -1;
    if (snapshot.currentTurnIndex >= 0 && snapshot.currentTurnIndex < static_cast<int>(snapshot.players.size())) {
        currentTurnPlayerId = snapshot.players[snapshot.currentTurnIndex].playerId;
        int localIndex = getLocalPlayerIndex(currentTurnPlayerId);
        if (localIndex >= 0) {
            m_game->setCurrentPlayerIndex(localIndex);
        }
    }
    
    bool isMyTurn = (currentTurnPlayerId == m_myPlayerId && currentTurnPlayerId >= 0);
    
    if (currentTurnPlayerId != m_lastTurnPlayerId && currentTurnPlayerId >= 0) {
        std::cout << "[MainWindow] Turn switched: last=" << m_lastTurnPlayerId 
                  << ", current=" << currentTurnPlayerId << ", myId=" << m_myPlayerId << std::endl;
        
        if (isMyTurn) {
            m_hasRolledDiceThisTurn = false;
            m_hasUsedHintThisTurn = false;
            std::cout << "[MainWindow] My turn started, reset roll dice and hint flags" << std::endl;
        } else {
            m_hasRolledDiceThisTurn = false;
            m_hasUsedHintThisTurn = false;
            std::cout << "[MainWindow] Other player's turn, reset flags for next round" << std::endl;
        }
        
        m_lastTurnPlayerId = currentTurnPlayerId;
    } else if (m_lastTurnPlayerId == -1 && currentTurnPlayerId >= 0) {
        m_lastTurnPlayerId = currentTurnPlayerId;
        if (isMyTurn) {
            m_hasRolledDiceThisTurn = false;
            m_hasUsedHintThisTurn = false;
            std::cout << "[MainWindow] First turn initialization: My turn, reset flags" << std::endl;
        } else {
            m_hasRolledDiceThisTurn = false;
            m_hasUsedHintThisTurn = false;
            std::cout << "[MainWindow] First turn initialization: Other player's turn, reset flags" << std::endl;
        }
    }
    
    std::cout << "[MainWindow] isMyTurn: " << isMyTurn << ", phase: " << static_cast<int>(snapshot.phase) << std::endl;
    
    if (m_rollButton) {
        bool isMyTurn = this->isMyTurn();
        bool phaseOk = (snapshot.phase == monopoly::RoomPhase::InGame);
        bool canRollDice = m_game ? m_game->canRollDice() : false;
        bool canRoll = isMyTurn && phaseOk && canRollDice;
        
        std::cout << "[MainWindow] Roll button check: isMyTurn=" << isMyTurn 
                  << ", phaseOk=" << phaseOk << " (phase=" << static_cast<int>(snapshot.phase) << ")"
                  << ", canRollDice=" << canRollDice 
                  << ", GameState=" << (m_game ? static_cast<int>(m_game->getCurrentState()) : -1)
                  << ", final canRoll=" << canRoll << std::endl;
        
        m_rollButton->setEnabled(canRoll);
        
        if (phaseOk && !canRollDice && m_game) {
            GameState currentState = m_game->getCurrentState();
            std::cout << "[MainWindow] Game state is " << static_cast<int>(currentState) 
                      << ", expected PlayerTurn (1)" << std::endl;
            if (currentState != GameState::PlayerTurn) {
                std::cout << "[MainWindow] Fixing game state: calling startGame()" << std::endl;
                m_game->startGame();
                canRollDice = m_game->canRollDice();
                canRoll = isMyTurn && phaseOk && canRollDice;
                std::cout << "[MainWindow] After fix: canRollDice=" << canRollDice 
                          << ", canRoll=" << canRoll << std::endl;
                m_rollButton->setEnabled(canRoll && !m_hasRolledDiceThisTurn);
            }
        }
    }
    if (m_endTurnButton) {
        bool canEnd = isMyTurn && snapshot.phase == monopoly::RoomPhase::InGame && m_game->canEndTurn();
        m_endTurnButton->setEnabled(canEnd);
        std::cout << "[MainWindow] End turn button enabled: " << canEnd << std::endl;
    }
    
    if (m_buyPropertyButton) {
        bool canBuy = false;
        if (isMyTurn) {
            int myLocalIndex = getMyLocalPlayerIndex();
            if (myLocalIndex >= 0) {
                const Player& player = m_game->getPlayer(myLocalIndex);
                int position = player.getPosition();
                Board& board = m_game->getBoard();
                Tile& tile = board.getTile(position);
                PropertyTile* propertyTile = dynamic_cast<PropertyTile*>(&tile);
                if (propertyTile && propertyTile->getOwner() == nullptr) {
                    canBuy = player.getCash() >= propertyTile->getPrice();
                }
            }
        }
        m_buyPropertyButton->setEnabled(canBuy);
        std::cout << "[MainWindow] Buy property button enabled: " << canBuy << std::endl;
    }
    
    if (m_hintButton) {
        m_hintButton->setEnabled(isMyTurn && !m_hasUsedHintThisTurn);
        std::cout << "[MainWindow] Hint button enabled: " << (isMyTurn && !m_hasUsedHintThisTurn) << std::endl;
    }
    
    if (m_sellPropertyButton) {
        bool canSell = false;
        int myLocalIndex = getMyLocalPlayerIndex();
        if (myLocalIndex >= 0 && myLocalIndex < m_game->getPlayerCount()) {
            const Player& player = m_game->getPlayer(myLocalIndex);
            std::vector<PropertyTile*> properties = player.getProperties();
            canSell = !properties.empty();
        }
        m_sellPropertyButton->setEnabled(canSell);
        std::cout << "[MainWindow] Sell property button enabled: " << canSell << " (myLocalIndex=" << myLocalIndex << ")" << std::endl;
    }

    if (m_payBailButton) {
        bool canPayBail = false;
        if (isMyTurn) {
            int myLocalIndex = getMyLocalPlayerIndex();
            if (myLocalIndex >= 0 && myLocalIndex < m_game->getPlayerCount()) {
                const Player& player = m_game->getPlayer(myLocalIndex);
                int jailStatus = player.getJailStatus();
                bool inJail = (jailStatus >= 0);
                bool hasEnoughCash = (player.getCash() >= 50);
                canPayBail = inJail && hasEnoughCash;
            }
        }
        m_payBailButton->setEnabled(canPayBail);
    }
    
    std::cout << "[MainWindow] Updating UI..." << std::endl;
    updatePlayerInfo();
    updateGameDisplay();
    
    if (m_boardWidget) {
        m_boardWidget->update();
        m_boardWidget->repaint();
        std::cout << "[MainWindow] Board widget updated and repainted" << std::endl;
    }
    
    QApplication::processEvents();
    std::cout << "[MainWindow] UI update completed" << std::endl;
}

void OnlineMainWindow::syncPlayersFromSnapshot(const monopoly::StateSnapshot& snapshot) {
    if (!m_game) return;
    
    std::cout << "[MainWindow] syncPlayersFromSnapshot: server has " << snapshot.players.size() << " players" << std::endl;

    int currentPlayerCount = m_game->getPlayerCount();
    int serverPlayerCount = static_cast<int>(snapshot.players.size());
    
    std::cout << "[MainWindow] Current local players: " << currentPlayerCount << ", server players: " << serverPlayerCount << std::endl;
    
    if (currentPlayerCount != serverPlayerCount) {
        std::cout << "[MainWindow] Player count mismatch! Resyncing..." << std::endl;
        
        m_game->clearPlayers();
        m_localToServerPlayerId.clear();
        
        for (const auto& serverPlayer : snapshot.players) {
            std::cout << "[MainWindow] Adding player: " << serverPlayer.displayName 
                      << " (ID: " << serverPlayer.playerId << ")" << std::endl;
            m_game->addPlayer(serverPlayer.displayName);
            int localIndex = m_game->getPlayerCount() - 1;
            m_localToServerPlayerId[localIndex] = serverPlayer.playerId;
        }
        
        std::cout << "[MainWindow] Added " << m_game->getPlayerCount() << " players" << std::endl;
        
        if (m_boardWidget) {
            m_boardWidget->update();
            m_boardWidget->repaint();
        }
        updatePlayerInfo();
        addGameLog(QString("Player list updated: %1 players in room").arg(serverPlayerCount));
    }
    

    for (size_t i = 0; i < snapshot.players.size(); ++i) {
        const auto& serverPlayer = snapshot.players[i];
        int localIndex = getLocalPlayerIndex(serverPlayer.playerId);
        if (localIndex >= 0) {
            syncPlayerState(localIndex, serverPlayer);
        } else {
            std::cout << "[MainWindow] WARNING: Could not find local index for server player ID " << serverPlayer.playerId << std::endl;
        }
    }

    bool needRebuild = m_localToServerPlayerId.empty();
    if (!needRebuild && m_game->getPlayerCount() > 0) {

        for (size_t i = 0; i < snapshot.players.size() && i < static_cast<size_t>(m_game->getPlayerCount()); ++i) {
            if (m_localToServerPlayerId.find(static_cast<int>(i)) == m_localToServerPlayerId.end()) {
                needRebuild = true;
                break;
            }
        }
    }
    
    if (needRebuild && !snapshot.players.empty() && m_game->getPlayerCount() > 0) {
        std::cout << "[MainWindow] Rebuilding player ID mapping..." << std::endl;
        m_localToServerPlayerId.clear();
        for (size_t i = 0; i < snapshot.players.size() && i < static_cast<size_t>(m_game->getPlayerCount()); ++i) {
            m_localToServerPlayerId[static_cast<int>(i)] = snapshot.players[i].playerId;
            std::cout << "[MainWindow] Mapped local[" << i << "] -> server[" << snapshot.players[i].playerId << "]" << std::endl;
        }
    }

    updatePlayerInfo();
    
    updateChatPlayerList();
}

void OnlineMainWindow::updateChatPlayerList() {
    if (!m_chatTargetCombo) return;

    int currentIndex = m_chatTargetCombo->currentIndex();
    QVariant currentData = m_chatTargetCombo->currentData();

    m_chatTargetCombo->clear();
    m_chatTargetCombo->addItem("Select player...", -1);
    
    int myLocalIndex = getMyLocalPlayerIndex();
    int myServerPlayerId = getServerPlayerId(myLocalIndex);
    
    for (const auto& [localIdx, serverId] : m_localToServerPlayerId) {
        if (serverId != myServerPlayerId && localIdx >= 0 && localIdx < m_game->getPlayerCount()) {
            QString playerName = QString::fromStdString(m_game->getPlayer(localIdx).getName());
            m_chatTargetCombo->addItem(QString("%1 (ID: %2)").arg(playerName).arg(serverId), serverId);
        }
    }
    
    if (currentData.isValid()) {
        int targetId = currentData.toInt();
        for (int i = 0; i < m_chatTargetCombo->count(); ++i) {
            if (m_chatTargetCombo->itemData(i).toInt() == targetId) {
                m_chatTargetCombo->setCurrentIndex(i);
                return;
            }
        }
    }
    
    m_chatTargetCombo->setCurrentIndex(0);
}

void OnlineMainWindow::syncPlayerState(int localIndex, const monopoly::PlayerState& serverState) {
    if (localIndex < 0 || localIndex >= m_game->getPlayerCount()) {
        std::cout << "[MainWindow] syncPlayerState: Invalid localIndex " << localIndex << std::endl;
        return;
    }
    
    Player& player = m_game->getPlayer(localIndex);
    
    int currentPos = player.getPosition();
    int serverPos = serverState.boardIndex;
    
    std::cout << "[MainWindow] Syncing player " << localIndex 
              << ": position " << currentPos << " -> " << serverPos 
              << ", cash " << player.getCash() << " -> " << serverState.cash << std::endl;
    
    if (currentPos != serverPos) {
        player.setPosition(serverPos);
        std::cout << "[MainWindow] Position updated for player " << localIndex << std::endl;
        if (m_boardWidget) {
            m_boardWidget->update();
            m_boardWidget->repaint();
        }
    }
    
    int currentCash = player.getCash();
    int serverCash = serverState.cash;
    if (currentCash != serverCash) {
        int diff = serverCash - currentCash;
        if (diff > 0) {
            player.addMoney(diff);
        } else {
            player.deductMoney(-diff);
        }
        std::cout << "[MainWindow] Cash updated for player " << localIndex << std::endl;
    }

    int currentJailStatus = player.getJailStatus();
    int serverJailStatus = serverState.jailStatus;
    
    if (currentJailStatus != serverJailStatus) {
        player.setJailStatus(serverJailStatus);
        std::cout << "[MainWindow] Jail status updated for player " << localIndex 
                  << ": " << currentJailStatus << " -> " << serverJailStatus << std::endl;
    }
    

    Board& board = m_game->getBoard();

    std::vector<PropertyTile*> currentProperties = player.getProperties();
    for (PropertyTile* prop : currentProperties) {
        if (prop && prop->getOwner() == &player) {
            prop->transferOwnership(player, nullptr, *m_game);
        }
    }
    

    std::cout << "[MainWindow] Syncing properties for player " << localIndex 
              << ": server has " << serverState.assets.size() << " properties" << std::endl;
    
    for (const std::string& assetStr : serverState.assets) {
        try {
            int propertyId = std::stoi(assetStr);
            if (propertyId >= 0 && propertyId < board.getSize()) {
                Tile& tile = board.getTile(propertyId);
                PropertyTile* prop = dynamic_cast<PropertyTile*>(&tile);
                if (prop) {
                    if (prop->getOwner() != &player) {
                        if (prop->getOwner() == nullptr) {

                            prop->setOwnerWithoutPayment(player);
                            std::cout << "[MainWindow] Property " << propertyId 
                                      << " assigned to player " << localIndex << " (without deducting money, server already handled)" << std::endl;
                        } else {
                            prop->transferOwnership(*prop->getOwner(), &player, *m_game);
                            std::cout << "[MainWindow] Property " << propertyId 
                                      << " transferred to player " << localIndex << std::endl;
                        }
                    }
                }
            }
        } catch (const std::exception& e) {
            std::cout << "[MainWindow] ERROR: Failed to parse property ID: " << assetStr 
                      << " (" << e.what() << ")" << std::endl;
        }
    }
    
    if (m_boardWidget) {
        m_boardWidget->update();
        m_boardWidget->repaint();
    }
}

int OnlineMainWindow::getMyLocalPlayerIndex() const {
    if (m_myPlayerId < 0) {
        std::cout << "[MainWindow] getMyLocalPlayerIndex: m_myPlayerId is invalid (" << m_myPlayerId << ")" << std::endl;
        return -1;
    }
    std::cout << "[MainWindow] getMyLocalPlayerIndex: Looking for m_myPlayerId=" << m_myPlayerId 
              << " in mapping (size=" << m_localToServerPlayerId.size() << ")" << std::endl;
    for (const auto& [localIdx, serverId] : m_localToServerPlayerId) {
        std::cout << "[MainWindow]   Checking local[" << localIdx << "] -> server[" << serverId << "]" << std::endl;
        if (serverId == m_myPlayerId) {
            std::cout << "[MainWindow]   Found! Returning local index " << localIdx << std::endl;
            return localIdx;
        }
    }
    std::cout << "[MainWindow] getMyLocalPlayerIndex: Not found! Returning -1" << std::endl;
    return -1;
}

bool OnlineMainWindow::isMyTurn() const {
    std::cout << "[MainWindow] isMyTurn: m_myPlayerId=" << m_myPlayerId << std::endl;
    if (m_myPlayerId < 0) {
        std::cout << "[MainWindow] isMyTurn: m_myPlayerId is invalid, returning false" << std::endl;
        return false;
    }
    if (m_lastSnapshot.players.empty()) {
        std::cout << "[MainWindow] isMyTurn: No players in snapshot, returning false" << std::endl;
        return false;
    }
    if (m_lastSnapshot.currentTurnIndex < 0 || 
        m_lastSnapshot.currentTurnIndex >= static_cast<int>(m_lastSnapshot.players.size())) {
        std::cout << "[MainWindow] isMyTurn: Invalid currentTurnIndex=" << m_lastSnapshot.currentTurnIndex 
                  << ", players.size()=" << m_lastSnapshot.players.size() << ", returning false" << std::endl;
        return false;
    }
    int currentTurnPlayerId = m_lastSnapshot.players[m_lastSnapshot.currentTurnIndex].playerId;
    bool result = (currentTurnPlayerId == m_myPlayerId);
    std::cout << "[MainWindow] isMyTurn: currentTurnPlayerId=" << currentTurnPlayerId 
              << ", m_myPlayerId=" << m_myPlayerId << ", result=" << result << std::endl;
    return result;
}

int OnlineMainWindow::getServerPlayerId(int localIndex) const {
    auto it = m_localToServerPlayerId.find(localIndex);
    return (it != m_localToServerPlayerId.end()) ? it->second : -1;
}

int OnlineMainWindow::getLocalPlayerIndex(int serverPlayerId) const {
    for (const auto& [localIdx, serverId] : m_localToServerPlayerId) {
        if (serverId == serverPlayerId) {
            return localIdx;
        }
    }
    return -1;
}

void OnlineMainWindow::updatePlayerInfo() {
    std::cout << "[MainWindow] updatePlayerInfo: m_myPlayerId=" << m_myPlayerId 
              << ", m_localToServerPlayerId.size()=" << m_localToServerPlayerId.size() << std::endl;
    
    int myLocalIndex = getMyLocalPlayerIndex();
    std::cout << "[MainWindow] updatePlayerInfo: myLocalIndex=" << myLocalIndex 
              << ", m_game->getPlayerCount()=" << (m_game ? m_game->getPlayerCount() : 0) << std::endl;
    
    if (myLocalIndex < 0 || myLocalIndex >= m_game->getPlayerCount()) {
        std::cout << "[MainWindow] updatePlayerInfo: Invalid player index " << myLocalIndex << std::endl;
        std::cout << "[MainWindow] Debug info:" << std::endl;
        std::cout << "  - m_myPlayerId: " << m_myPlayerId << std::endl;
        std::cout << "  - m_localToServerPlayerId mapping:" << std::endl;
        for (const auto& [localIdx, serverId] : m_localToServerPlayerId) {
            std::cout << "    local[" << localIdx << "] -> server[" << serverId << "]" << std::endl;
        }
        if (m_playerInfoLabel) {
            m_playerInfoLabel->setText("Player: Not in game");
        }
        if (m_cashLabel) {
            m_cashLabel->setText("Cash: $0");
        }
        return;
    }
    
    const Player& player = m_game->getPlayer(myLocalIndex);
    std::cout << "[MainWindow] updatePlayerInfo: Player " << myLocalIndex 
              << " (" << player.getName() << "), Cash: $" << player.getCash() 
              << ", Position: " << player.getPosition() << std::endl;
    
    if (m_playerInfoLabel) {
        QString text = QString("Player: %1").arg(QString::fromStdString(player.getName()));
        if (m_myPlayerId >= 0) {
            text += QString(" (ID: %1)").arg(m_myPlayerId);
        }
        m_playerInfoLabel->setText(text);
        std::cout << "[MainWindow] Player info label updated: " << text.toStdString() << std::endl;
    }
    
    if (m_cashLabel) {
        QString cashText = QString("Cash: $%1").arg(player.getCash());
        m_cashLabel->setText(cashText);
        std::cout << "[MainWindow] Cash label updated: " << cashText.toStdString() << std::endl;
    }
}

void OnlineMainWindow::updateGameDisplay() {
    std::cout << "[MainWindow] updateGameDisplay() called" << std::endl;
    if (m_boardWidget) {
        m_boardWidget->update();
        std::cout << "[MainWindow] Board widget update() called" << std::endl;
    } else {
        std::cout << "[MainWindow] WARNING: m_boardWidget is null!" << std::endl;
    }
}

void OnlineMainWindow::addGameLog(const QString& message) {
    if (m_gameLog) {
        m_gameLog->append(message);
        QTextCursor cursor = m_gameLog->textCursor();
        cursor.movePosition(QTextCursor::End);
        m_gameLog->setTextCursor(cursor);
    }
    std::cout << "[OnlineMainWindow] " << message.toStdString() << std::endl;
}

