#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "BoardWidget.h"
#include "Game.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_game(nullptr)
{
    qDebug() << "1. MainWindow构造函数开始";

    // 先创建 Game 对象
    qDebug() << "2. 创建Game对象";
    m_game = new Game();
    qDebug() << "3. Game对象创建完成，玩家数量:" << m_game->getPlayerCount();

    // 设置购买回调
    setupPurchaseCallback();

    qDebug() << "4. 调用setupUI";
    setupUI();
    qDebug() << "5. setupUI完成";

    m_statusLabel->setText("Game is initialized. Ready to start!!!");
    qDebug() << "6. MainWindow构造函数完成";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    qDebug() << "setupUI开始";

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
    if (!m_game || m_game->getPlayerCount() == 0) {
        m_statusLabel->setText("Game not initialized!");
        return;
    }

    // 1. 掷骰子
    int diceValue = m_game->rollDice();

    // 2. 获取当前玩家信息（移动前）
    int currentPlayerIndex = m_game->getCurrentPlayerIndex();
    Player& currentPlayer = m_game->getPlayer(currentPlayerIndex);

    // 3. 立即显示骰子点数
    QString status = QString("%1 rolled %2")
                         .arg(QString::fromStdString(currentPlayer.getName()))
                         .arg(diceValue);
    m_statusLabel->setText(status);
    qApp->processEvents();  // 强制更新界面

    m_game->movePlayer(currentPlayer,diceValue);

    // 5. First update the board on the left, then update the status lable on the right
    updateDisplay();
    qApp->processEvents();  // 再次强制更新

    // // 7. 获取事件处理后的玩家状态
    // const Player& playerAfterEvent = m_game->getPlayer(currentPlayerIndex);

    // // 9. 更新界面
    // updateDisplay();

    // // 10. 显示下一个玩家信息
    // const Player& nextPlayer = m_game->getPlayer(m_game->getCurrentPlayerIndex());
    // m_statusLabel->setText(QString("Player %1's turn")
    //                            .arg(QString::fromStdString(nextPlayer.getName())));

    // // 11. 控制台日志（使用事件处理后的玩家状态）
    // std::cout << "=== Turn Event ===" << std::endl;
    // std::cout << "Player " << currentPlayer.getName() << " rolled " << diceValue << std::endl;
    // std::cout << "Landed on position: " << playerAfterEvent.getPosition() << std::endl;
    // std::cout << "Cash: $" << playerAfterEvent.getCash() << std::endl;
    // std::cout << "Next player: " << nextPlayer.getName() << std::endl;
    // std::cout << "==================" << std::endl << std::endl;

}

void MainWindow::onPropertiesClicked(){

}

void MainWindow::onHintClicked(){

}

void MainWindow::onChatroomClicked(){

}

void MainWindow::onEndTurnClicked(){

}


void MainWindow::updateDisplay()
{
    // Get current play
    int currentPlayerIndex = m_game->getCurrentPlayerIndex();
    const Player& currentPlayer = m_game->getPlayer(currentPlayerIndex);
    // Refresh the board
    m_boardWidget->update();
    // Update the status lable
    QString status = QString("%1 moved to %2")
                         .arg(QString::fromStdString(currentPlayer.getName()))
                         .arg(currentPlayer.getPosition());
}

void MainWindow::setupPurchaseCallback() {
    if (m_game) {
        m_game->setPurchaseCallback([this](const std::string& propertyName, int price, const std::string& playerName) {
            return handlePurchaseDialog(propertyName, price, playerName);
        });
    }
}

bool MainWindow::handlePurchaseDialog(const std::string& propertyName, int price, const std::string& playerName) {
    // 使用QMessageBox创建购买确认对话框
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Purchase Property");
    msgBox.setText(QString("Purchase Opportunity for %1").arg(QString::fromStdString(playerName)));
    msgBox.setInformativeText(QString("Do you want to buy %1 for $%2?").arg(QString::fromStdString(propertyName)).arg(price));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);

    // 添加图标和详细说明
    msgBox.setIcon(QMessageBox::Question);

    // 显示玩家当前现金
    int playerCash = 0;
    if (m_game) {
        for (int i = 0; i < m_game->getPlayerCount(); ++i) {
            if (m_game->getPlayer(i).getName() == playerName) {
                playerCash = m_game->getPlayer(i).getCash();
                break;
            }
        }
    }

    msgBox.setDetailedText(QString("Property: %1\nPrice: $%2\nYour cash: $%3\nBalance after purchase: $%4")
                               .arg(QString::fromStdString(propertyName))
                               .arg(price)
                               .arg(playerCash)
                               .arg(playerCash - price));

    int result = msgBox.exec();
    return (result == QMessageBox::Yes);
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
