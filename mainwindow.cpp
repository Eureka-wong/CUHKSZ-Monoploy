#include "mainwindow.h"
#include "StrategyEngine.h"
#include "ui_MainWindow.h"
#include "BoardWidget.h"
#include "Game.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QScrollArea>
#include <QDialog>
#include <QFrame>
#include <QDialogButtonBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_game(nullptr)
{
    m_game = new Game();
    setupUI();
    //在这个函数里面，把游戏里面的信号和ui里面的槽联系起来
    setupGameConnections();
    m_statusLabel->setText("Game is initialized. Ready to start!!!");
    m_game->startGame();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 在这个函数里我们要建立游戏信号和主窗口里面接收槽的联系
void MainWindow::setupGameConnections(){
    //gameStarted信号和onGameStarted槽
    //connect(m_game, &Game::gameStarted,this, &MainWindow::onGameStarted);

    //gameStateChanged signal connects to onGameStateChanged slot
    //connect(m_game, &Game::gameStateChanged, this, &MainWindow::onGameStateChanged);

    //gameOver signal connects to onGameOver slot
    //connect(m_game, &Game::gameOver,this,&MainWindow::onGameOver);

    connect(m_game, &Game::playerTurnStarted, this, &MainWindow::onPlayerTurnStarted);
    connect(m_game, &Game::playerTurnEnded, this, &MainWindow::onPlayerTurnEnded);

    // //playerTurnEnded connects to onPlayerTurnEnded
    // connect(m_game, &Game::playerTurnEnded,this,&MainWindow::onEndTurnClicked);

    // 玩家行动信号
    connect(m_game, &Game::diceRolled, this, &MainWindow::onDiceRolled);
    connect(m_game, &Game::playerMoved, this, &MainWindow::onPlayerMoved);

    // 格子事件信号
    connect(m_game, &Game::purchaseOpportunity, this, &MainWindow::onPurchaseOpportunity);

    // 经济信号
    //connect(m_game, &Game::moneyChanged, this, &MainWindow::onMoneyChanged);
    //connect(m_game, &Game::propertyPurchased,this,&MainWindow::onPropertyPurchased);

    // 游戏日志
    connect(m_game, &Game::gameLogMessage, this, &MainWindow::onGameLogMessage);
    connect(m_game, &Game::playerBankrupt, this, &MainWindow::onPlayerBankrupt);
    connect(m_game, &Game::rentPaymentRequired, this, &MainWindow::onrentPaymentRequired);

    connect(m_game, &Game::forceRaiseMoney, this, &MainWindow::onForceRaiseMoney);

    connect(m_game, &Game::warningSignal, this, &MainWindow::showWarning);
}

void MainWindow::onrentPaymentRequired(const QString& property, int rent, int fromPlayer, int toPlayer){
    // 创建消息框
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Rent Payment Required");
    msgBox.setText(QString("You need to pay rent for %1").arg(property));
    msgBox.setInformativeText(QString("You must pay $%1 to Player %2?").arg(rent).arg(toPlayer));

    // 设置为应用模态，必须处理
    msgBox.setWindowModality(Qt::ApplicationModal);
    // 移除关闭按钮
    msgBox.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    QPushButton *payButton = msgBox.addButton("Pay Now", QMessageBox::AcceptRole);
    msgBox.setDefaultButton(payButton);

    msgBox.exec();  // 强制用户点击按钮
    m_game->payRent(fromPlayer, toPlayer, rent);
}

void MainWindow::onPlayerBankrupt(int playerIndex){
    //把游戏头像从里面抹掉
}

void MainWindow::onGameLogMessage(const QString& message) {
    m_gameLog->append(message);

    QTextCursor cursor = m_gameLog->textCursor();
    cursor.movePosition(QTextCursor::End);
    m_gameLog->setTextCursor(cursor);
}

void MainWindow::onPurchaseOpportunity(const QString& propertyName, int price, int playerIndex){
    const Player& currentPlayer = m_game->getPlayer(playerIndex);

    // 创建购买确认对话框
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Purchase Opportunity");
    msgBox.setIcon(QMessageBox::Question);

    // 检查现金是否充足
    bool canAfford = (currentPlayer.getCash() >= price);

    if (canAfford) {
        msgBox.setInformativeText(QString("Do you want to buy %1 for $%2?").arg(propertyName).arg(price));

        // 设置标准按钮
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
    } else {
        msgBox.setInformativeText(QString("Do you want to buy %1 for $%2?\n\nYou don't have enough cash!")
                                      .arg(propertyName).arg(price));

        // 现金不足时，只显示No按钮，禁用购买
        msgBox.setStandardButtons(QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
    }
    // 添加详细说明
    QString details = QString("Property: %1\nPrice: $%2\nYour cash: $%3\nBalance after purchase: $%4")
                          .arg(propertyName)
                          .arg(price)
                          .arg(currentPlayer.getCash())
                          .arg(currentPlayer.getCash() - price);
    msgBox.setDetailedText(details);

    // 显示对话框并获取结果
    int result = msgBox.exec();

    if (result == QMessageBox::Yes) {
        // 用户选择购买
        m_game->purchaseCurrentProperty();
        // 购买成功
        m_statusLabel->setText(QString("%1 purchased %2 for $%3")
                                   .arg(playerIndex+1).arg(propertyName).arg(price));
        m_gameLog->append(QString("[PURCHASE] Player %1 bought %2 for $%3")
                              .arg(playerIndex+1).arg(propertyName).arg(price));
    } else {
        // 用户选择不购买
        m_statusLabel->setText(QString("%1 decided not to buy %2").arg(playerIndex+1).arg(propertyName));
        m_gameLog->append(QString("[PASS] Player %1 passed on buying %2").arg(playerIndex+1).arg(propertyName));
    }
}

void MainWindow::onPlayerTurnStarted(int playerIndex) {
    const Player& player = m_game->getPlayer(playerIndex);

    // update status label
    m_statusLabel->setText(QString("Player %1's turn").arg(QString::fromStdString(player.getName())));

    // 更新按钮状态
    m_rollButton->setEnabled(m_game->canRollDice());
    m_endTurnButton->setEnabled(false);
}

void MainWindow::onPlayerTurnEnded(int playerIndex){
    const Player& player = m_game->getPlayer(playerIndex);
    // update status label
    m_statusLabel->setText(QString("Player %1's turn ends").arg(QString::fromStdString(player.getName())));

    // 更新按钮状态
    m_endTurnButton->setEnabled(m_game->canEndTurn());
    m_rollButton->setEnabled(false);
}

void MainWindow::onDiceRolled(int playerIndex, int diceValue) {
    const Player& player = m_game->getPlayer(playerIndex);
    //after the logic part rolls the dice, the ui part needs to display the value of dice on the status label
    m_statusLabel->setText(QString("%1 rolled %2").arg(QString::fromStdString(player.getName())).arg(diceValue));
}

void MainWindow::onPlayerMoved(int playerIndex, int oldPosition, int newPosition) {
    // in logic part, player has moved, so we want the board's ui change as well
    // by calling the update method for m_boardWidget, it calls the paintEvent method in mainwindow.cpp
    // in which, it repaint player's location
    // and later I should add more to the paintEvent code so that it can paint user's houses and hotels
    m_boardWidget->update();
}

void MainWindow::setupUI()
{
    // 创建中央部件
    QWidget* centralWidget = new QWidget();
    setCentralWidget(centralWidget);

    // 创建主布局
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    // 左侧 - 棋盘
    qDebug() << "创建BoardWidget，m_game指针:" << m_game;
    if (!m_game) {
        qDebug() << "错误: m_game 为 nullptr!";
        return;
    }

    m_boardWidget = new BoardWidget(m_game, this);
    qDebug() << "BoardWidget创建完成";
    mainLayout->addWidget(m_boardWidget);

    // Right hand side, the control panel
    QWidget* controlPanel = new QWidget();
    QVBoxLayout* controlLayout = new QVBoxLayout(controlPanel);


    // 1. Current round
    m_roundLabel = new QLabel("Current Round: Waiting to start");
    m_roundLabel->setStyleSheet("font-weight: bold; margin: 10px; color: #e74c3c;");
    controlLayout->addWidget(m_roundLabel);

    // 2. My balance
    m_cashLabel = new QLabel("My cash: $1000");
    m_cashLabel->setStyleSheet("margin-left: 10px;");
    controlLayout->addWidget(m_cashLabel);

    // 3. Game Event
    m_gameLogTitle = new QLabel("Game Events:");
    m_gameLogTitle->setStyleSheet("font-weight: bold; margin-top: 10px; color: #2c3e50;");
    controlLayout->addWidget(m_gameLogTitle);
    m_gameLog = new QTextEdit();
    m_gameLog->setMaximumHeight(150);
    m_gameLog->setReadOnly(true);
    m_gameLog->setStyleSheet("QTextEdit { border: 1px solid #bdc3c7; border-radius: 3px; background-color: #f8f9fa; }");
    m_gameLog->setStyleSheet("QTextEdit { color: white; background-color: black; }");
    controlLayout->addWidget(m_gameLog);

    // 4. My properties Button
    m_propertiesButton = new QPushButton("View My Properties");
    m_propertiesButton->setStyleSheet("QPushButton {background-color: #E4A823;color: white;border: none;padding: 8px;border-radius: 4px;margin: 5px 10px;}");
    connect(m_propertiesButton, &QPushButton::clicked, this, [this]() {
        onPropertiesClicked(); // To use defaults;
    });
    controlLayout->addWidget(m_propertiesButton);

    // 5. Chatroom Button
    m_chatroomButton = new QPushButton("Chatroom");
    m_chatroomButton->setStyleSheet("QPushButton { background-color: #E4A823; color: white; border: none; padding: 8px; border-radius: 4px; margin: 5px 10px; }");
    connect(m_chatroomButton, &QPushButton::clicked, this, &MainWindow::onChatroomClicked);
    controlLayout->addWidget(m_chatroomButton);

    // 6. AI Hint Button
    m_hintButton = new QPushButton("Get AI Hint");
    m_hintButton->setStyleSheet("QPushButton { background-color: #E4A823; color: white; border: none; padding: 8px; border-radius: 4px; margin: 5px 10px; }");
    connect(m_hintButton, &QPushButton::clicked, this, &MainWindow::onHintClicked);
    controlLayout->addWidget(m_hintButton);

    // 7. Roll Dice Button
    m_rollButton = new QPushButton("Roll the dices");
    m_rollButton->setStyleSheet("QPushButton { background-color: #E4A823; color: white; border: none; padding: 10px; border-radius: 4px; margin: 5px 10px; font-weight: bold; }");
    connect(m_rollButton, &QPushButton::clicked, this, &MainWindow::onRollDiceClicked);
    controlLayout->addWidget(m_rollButton);

    // 8. End Turn Button
    m_endTurnButton = new QPushButton("End Turn");
    m_endTurnButton->setStyleSheet("QPushButton { background-color: #E4A823; color: white; border: none; padding: 8px; border-radius: 4px; margin: 5px 10px; }");
    connect(m_endTurnButton, &QPushButton::clicked, this, &MainWindow::onEndTurnClicked);
    controlLayout->addWidget(m_endTurnButton);

    // 9. Game Status Lable
    m_statusLabel = new QLabel();

    // 添加拉伸空间
    controlLayout->addStretch();

    mainLayout->addWidget(controlPanel);

    // 设置窗口标题和大小
    setWindowTitle("Monopoly Game in CUHKSZ");
    resize(1100, 800);

    qDebug() << "setupUI完成";
}

void MainWindow::onRollDiceClicked()
{
    if (m_game->canRollDice()) {
        // call m_game's method
        m_game->rollDiceAndMoveAndProcessEvent();
    }
}

void MainWindow::onPropertiesClicked(int playerIndex, bool forced, int amountDue) {
    int currentPlayerIndex = playerIndex;
    if (currentPlayerIndex == -1) {
        currentPlayerIndex = m_game->getCurrentPlayerIndex();
    }
    const Player& currentPlayer = m_game->getPlayer(currentPlayerIndex);

    // Create properties dialog
    QDialog propertiesDialog(this);
    propertiesDialog.setWindowTitle(QString("Player %1's Properties").arg(currentPlayerIndex + 1));
    propertiesDialog.setMinimumSize(500, 400);
    propertiesDialog.setStyleSheet("QDialog { background-color: white; }");

    if (forced) {
        propertiesDialog.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    }

    QVBoxLayout *layout = new QVBoxLayout(&propertiesDialog);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(5);

    // Player info
    QLabel *playerInfo = new QLabel(QString("Player %1 - Cash: $%2")
                                        .arg(currentPlayerIndex + 1)
                                        .arg(currentPlayer.getCash()));
    playerInfo->setStyleSheet("QLabel { font-weight: bold; font-size: 14px; padding: 2px; margin: 0px; color: black; }");
    layout->addWidget(playerInfo);

    if (forced) {
        QLabel *warningLabel = new QLabel("You must raise money by selling or mortgaging your properties.");
        warningLabel->setStyleSheet("QLabel { color: red; font-weight: bold; padding: 5px; }");
        layout->addWidget(warningLabel);
    }

    // Scroll area for properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: 1px solid #ccc; background-color: white; }");

    QWidget *propertiesContainer = new QWidget();
    QVBoxLayout *propertiesLayout = new QVBoxLayout(propertiesContainer);
    propertiesLayout->setContentsMargins(5, 5, 5, 5);
    propertiesLayout->setSpacing(8);

    std::vector<PropertyTile*> ownedProperties = currentPlayer.getProperties();

    if (ownedProperties.empty()) {
        QLabel *noPropertiesLabel = new QLabel("No properties owned");
        noPropertiesLabel->setStyleSheet("QLabel { color: gray; padding: 10px; }");
        noPropertiesLabel->setAlignment(Qt::AlignCenter);
        propertiesLayout->addWidget(noPropertiesLabel);
    } else {
        // Create a widget for each property
        for (PropertyTile* property : ownedProperties) {
            QFrame *propertyFrame = new QFrame();
            propertyFrame->setFrameStyle(QFrame::Box);
            propertyFrame->setStyleSheet("QFrame { border: 1px solid #ddd; border-radius: 4px; padding: 8px; background-color: #f9f9f9; }");

            QVBoxLayout *propertyLayout = new QVBoxLayout(propertyFrame);
            propertyLayout->setSpacing(4);

            // Property information
            QString propertyText = QString("Tile %1: %2\nPrice: $%3 | Houses: %4")
                                       .arg(property->getIndex())
                                       .arg(property->getName())
                                       .arg(property->getPrice())
                                       .arg(property->getHouses());

            if (property->isMortgaged()) {
                propertyText += " <span style='color: red;'>[MORTGAGED]</span>";
            }

            QLabel *propertyInfo = new QLabel(propertyText);
            propertyInfo->setTextFormat(Qt::RichText);
            propertyInfo->setStyleSheet("QLabel { font-size: 12px; margin: 0px; color: black; }");
            propertyLayout->addWidget(propertyInfo);

            // Buttons layout
            QHBoxLayout *buttonsLayout = new QHBoxLayout();
            buttonsLayout->setSpacing(5);


            QPushButton *buyBuildingButton = new QPushButton("Buy Building");
            QPushButton *sellBuildingButton = new QPushButton("Sell Building");
            QPushButton *mortgageButton = new QPushButton(property->isMortgaged() ? "Lift Mortgage" : "Mortgage");

            // Style buttons
            QString buttonStyle = "QPushButton { font-size: 11px; padding: 4px 8px; border-radius: 3px; }";
            buyBuildingButton->setStyleSheet(buttonStyle + "QPushButton { background-color: #4CAF50; color: white; }");
            sellBuildingButton->setStyleSheet(buttonStyle + "QPushButton { background-color: #FF9800; color: white; }");
            mortgageButton->setStyleSheet(buttonStyle + "QPushButton { background-color: #2196F3; color: white; }");

            // Connect functions to slots (you'll need to implement these)
            connect(buyBuildingButton, &QPushButton::clicked, [this, property, currentPlayerIndex, &propertiesDialog, playerInfo, propertyInfo]() {
                if (property->buyBuilding(m_game->getPlayer(currentPlayerIndex), *m_game)) {
                    updatePropertyDisplay(playerInfo, propertyInfo, property, currentPlayerIndex);
                }
            });

            connect(sellBuildingButton, &QPushButton::clicked, [this, property, currentPlayerIndex, &propertiesDialog, playerInfo, propertyInfo,
                                                                propertyFrame, forced, amountDue]() {
                if (property->getHouses() == 0) {
                    if (property->sellProperty(m_game->getPlayer(currentPlayerIndex), *m_game)) {
                        propertyFrame->setVisible(false);
                        propertyFrame->setMaximumHeight(0);
                    }
                }
                else if (property->sellBuilding(m_game->getPlayer(currentPlayerIndex), *m_game)) {
                    updatePropertyDisplay(playerInfo, propertyInfo, property, currentPlayerIndex);
                }

                if (forced) {
                    if (m_game->getPlayer(currentPlayerIndex).getCash() >= amountDue) {
                        propertiesDialog.close();
                    }
                }
            });

            connect(mortgageButton, &QPushButton::clicked, [this, property, currentPlayerIndex, &propertiesDialog, playerInfo, propertyInfo,
                                                            forced, amountDue]() {
                if (property->mortgageProperty(m_game->getPlayer(currentPlayerIndex), *m_game)) {
                    updatePropertyDisplay(playerInfo, propertyInfo, property, currentPlayerIndex);
                }
                if (forced) {
                    if (m_game->getPlayer(currentPlayerIndex).getCash() >= amountDue) {
                        propertiesDialog.close();
                    }
                }
            });

            if (!forced) {
                buttonsLayout->addWidget(buyBuildingButton);
            }
            buttonsLayout->addWidget(sellBuildingButton);
            buttonsLayout->addWidget(mortgageButton);
            buttonsLayout->addStretch(); // Push buttons to the left

            propertyLayout->addLayout(buttonsLayout);
            propertiesLayout->addWidget(propertyFrame);
        }
        if (currentPlayer.hasGetOutOfJailCard()) {
            QFrame *jailCardFrame = new QFrame();
            jailCardFrame->setFrameStyle(QFrame::Box);
            jailCardFrame->setStyleSheet("QFrame { border: 1px solid #ddd; border-radius: 4px; padding: 8px; background-color: #f9f9f9; }");

            QVBoxLayout *jailCardLayout = new QVBoxLayout(jailCardFrame);
            jailCardLayout->setSpacing(4);

            // Jail card information
            QString jailCardText = QString("Get Out of Jail Card\nOwned: %1")
                                       .arg(currentPlayer.ownedGetOutOfJailCard());

            QLabel *jailCardInfo = new QLabel(jailCardText);
            jailCardInfo->setStyleSheet("QLabel { font-size: 12px; margin: 0px; color: black; }");
            jailCardLayout->addWidget(jailCardInfo);

            // Buttons layout for jail card
            QHBoxLayout *jailButtonsLayout = new QHBoxLayout();
            jailButtonsLayout->setSpacing(5);

            QPushButton *tradeJailCardButton = new QPushButton("Trade Card");

            // Style button
            QString jailButtonStyle = "QPushButton { font-size: 11px; padding: 4px 8px; border-radius: 3px; }";
            tradeJailCardButton->setStyleSheet(jailButtonStyle + "QPushButton { background-color: #9C27B0; color: white; }");

            // Connect trade button (placeholder for now)
            connect(tradeJailCardButton, &QPushButton::clicked, [this, currentPlayerIndex]() {
                // TODO: Implement jail card trading logic
                qDebug() << "Trade jail card clicked for player" << currentPlayerIndex;
            });

            jailButtonsLayout->addWidget(tradeJailCardButton);
            jailButtonsLayout->addStretch();

            jailCardLayout->addLayout(jailButtonsLayout);
            propertiesLayout->addWidget(jailCardFrame);
        }
    }

    propertiesLayout->addStretch(); // Push all content to the top
    scrollArea->setWidget(propertiesContainer);
    layout->addWidget(scrollArea);

    // Close button
    if (!forced) {
        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
        buttonBox->setStyleSheet("QDialogButtonBox { padding: 5px; margin: 0px; }");
        connect(buttonBox, &QDialogButtonBox::rejected, &propertiesDialog, &QDialog::reject);
        layout->addWidget(buttonBox);
    }

    // Show the dialog
    propertiesDialog.exec();
}

void MainWindow::updatePropertyDisplay(QLabel* playerInfo, QLabel* propertyInfo, PropertyTile* property, int currentPlayerIndex) {
    playerInfo->setText(QString("Player %1 - Cash: $%2")
                                 .arg(currentPlayerIndex + 1)
                                 .arg(m_game->getPlayer(currentPlayerIndex).getCash()));

    QString propertyText = QString("Tile %1: %2\nPrice: $%3 | Houses: %4")
                               .arg(property->getIndex())
                               .arg(property->getName())
                               .arg(property->getPrice())
                               .arg(property->getHouses());

    if (property->isMortgaged()) {
        propertyText += " <span style='color: red;'>[MORTGAGED]</span>";
    }

    propertyInfo->setText(propertyText);
    propertyInfo->setTextFormat(Qt::RichText);
}

void MainWindow::onForceRaiseMoney(int payerIndex, int amountDue) {
    onPropertiesClicked(payerIndex, true, amountDue);
}

void MainWindow::showWarning(const QString& message) {
    QMessageBox::warning(this, "Warning", message);
}


void MainWindow::onHintClicked(int playerIndex){
    int currentPlayerIndex = m_game->getCurrentPlayerIndex();
    // if (currentPlayerIndex == -1) {
    //     currentPlayerIndex = m_game->getCurrentPlayerIndex();
    // }
    Player& currentPlayer = m_game->getPlayer(currentPlayerIndex);
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("Strategy Engine"));
    QString infoStr = ("Do you want to use a strategy engine for $50?");

    // 检查现金 & SE是否充足
    bool canAfford = (currentPlayer.getCash() >= 100 && currentPlayer.getSE() > 0);
    // bool canAfford = true;

    if (canAfford) {

        // 设置选择按钮
        msgBox.setText(infoStr);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
    } else {
        msgBox.setInformativeText(infoStr + "\n\nYou don't have enough cash!");

        // 现金不足时，只显示No按钮，禁止使用Strategy Engine
        msgBox.setStandardButtons(QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
    }
    // 添加详细说明
    QString details = QString("Price of Strategy Engine: $50\nYour cash: $%1\nBalance after purchase: $%2\nAvailable Strategy Engines after purchase:%3")
                          .arg(currentPlayer.getCash())
                          .arg(currentPlayer.getCash() - 50)
                          .arg(currentPlayer.getSE() - 1);
    msgBox.setDetailedText(details);

    // 显示对话框并获取结果
    int result = msgBox.exec();

    if (result == QMessageBox::Yes) {
        // 用户选择使用SE
        currentPlayer.deductSE();
        // 购买成功
        m_statusLabel->setText(QString("%1 purchased a Strategy Engine for $50")
                                   .arg(currentPlayerIndex+1));
        m_gameLog->append(QString("[PURCHASE] Player %1 bought a Strategy Engine for $50")
                              .arg(currentPlayerIndex+1));
        //获得SE预测结果，该结果仅展示给用户，不在日志中显示
        StrategyEngine SE(m_game);
        QString hint = SE.getHintResultforQt(currentPlayer);
        QMessageBox SEbox;
        SEbox.setStyleSheet("QLabel{min-width: 500px; min-height: 200px;}");
        SEbox.setWindowTitle("Strategy Suggestions");
        SEbox.setText(hint);
        // SEbox.setDetailedText(hint);
        SEbox.exec();


    }

}

void MainWindow::onChatroomClicked(){

}

void MainWindow::onEndTurnClicked(){
    if (m_game->canEndTurn()) {
        m_game->endTurn();
    }
}

void MainWindow::updatePlayerInfo()
{
    if (!m_game || m_game->getPlayerCount() == 0) {
        return;
    }

    // 获取当前玩家（回合开始前的玩家）
    int currentPlayerIndex = m_game->getCurrentPlayerIndex();
    const Player& currentPlayer = m_game->getPlayer(currentPlayerIndex);

    // 更新玩家信息标签
    m_playerInfoLabel->setText(QString("Player: %1")
                                   .arg(QString::fromStdString(currentPlayer.getName())));

    // 更新现金显示
    m_cashLabel->setText(QString("Cash: $%1")
                             .arg(currentPlayer.getCash()));

    std::vector<PropertyTile*> properties = currentPlayer.getProperties();

}
