#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "BoardWidget.h"
#include "Game.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include "strategyEngine.h"

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
    m_cashLabel = new QLabel("My cash: $0");
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
    controlLayout->addWidget(m_gameLog);

    // 4. My properties Button
    m_propertiesButton = new QPushButton("View My Properties");
    m_propertiesButton->setStyleSheet("QPushButton {background-color: #E4A823;color: white;border: none;padding: 8px;border-radius: 4px;margin: 5px 10px;}");
    connect(m_propertiesButton, &QPushButton::clicked, this, &MainWindow::onPropertiesClicked);
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

void MainWindow::onPropertiesClicked(){

}

void MainWindow::onHintClicked(int playerIndex){
    Player& currentPlayer = m_game->getPlayer(playerIndex);
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("Strategy Engine"));
    QString infoStr = ("Do you want to use a strategy engine for $50?");

    // 检查现金 & SE是否充足
    bool canAfford = (currentPlayer.getCash() >= 100 && currentPlayer.getSE() > 0);

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
                                   .arg(playerIndex+1));
        m_gameLog->append(QString("[PURCHASE] Player %1 bought a Strategy Engine for $50")
                              .arg(playerIndex+1));
        //获得SE预测结果，该结果仅展示给用户，不在日志中显示
        StrategyEngine SE(m_game);
        QString hint = SE.getHintResultforQt(currentPlayer);
        QMessageBox SEbox;
        SEbox.setWindowTitle("Strategy Suggestions");
        SEbox.setText("Strategy Engine Result:");
        SEbox.setDetailedText(hint);
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
