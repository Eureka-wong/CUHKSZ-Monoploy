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
#include <QStackedWidget>
#include <QAbstractAnimation>
#include <QPropertyAnimation>
#include <QComboBox>
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_game(nullptr)
{
    m_game = new Game();
    setupUI();
    //在这个函数里面，把游戏里面的信号和ui里面的槽联系起来
    setupGameConnections();
    m_game->startGame();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 在这个函数里我们要建立游戏信号和主窗口里面接收槽的联系
void MainWindow::setupGameConnections(){
    connect(m_game, &Game::playerTurnStarted, this, &MainWindow::onPlayerTurnStarted);
    connect(m_game, &Game::playerTurnEnded, this, &MainWindow::onPlayerTurnEnded);
    connect(m_game, &Game::roundLabelChanged,this,&MainWindow::onRoundLabelChanged);

    // 玩家行动信号
    connect(m_game, &Game::diceRolled, this, &MainWindow::onDiceRolled);
    connect(m_game, &Game::playerMoved, this, &MainWindow::onPlayerMoved);

    // 格子事件信号
    connect(m_game, &Game::purchaseOpportunity, this, &MainWindow::onPurchaseOpportunity);
    connect(m_game, &Game::landOnGoTile,this,&MainWindow::onLandOnGoTile);
    connect(m_game, &Game::landOnGoToJailTile,this,&MainWindow::onLandOnGoToJailTile);

    // 经济信号
    connect(m_game, &Game::landOnSelfProperty,this,&MainWindow::onLandOnSelfProperty);
    connect(m_game,&Game::openChanceCard,this,&MainWindow::onOpenChanceCard);
    connect(m_game,&Game::openCommunityCard,this,&MainWindow::onOpenCommunityCard);

    // 游戏日志
    connect(m_game, &Game::gameLogMessage, this, &MainWindow::onGameLogMessage);
    connect(m_game, &Game::playerBankrupt, this, &MainWindow::onPlayerBankrupt);
    connect(m_game, &Game::rentPaymentRequired, this, &MainWindow::onrentPaymentRequired);
    connect(m_game, &Game::taxPaymentRequired, this, &MainWindow::ontaxPaymentRequired);
    connect(m_game, &Game::forceRaiseMoney, this, &MainWindow::onForceRaiseMoney);
    connect(m_game, &Game::warningSignal, this, &MainWindow::showWarning);

    //按钮信号
    connect(m_game,&Game::enableEndTurnAndDisableRoll,this,&MainWindow::onEnableEndTurnAndDisableRoll);
}

void MainWindow::onRoundLabelChanged(){
    m_roundLabel->setText(QString::fromStdString("Current Round: %1").arg(m_game->getRoundNum()));
}


void MainWindow::onEnableEndTurnAndDisableRoll(){
    // 更新按钮状态
    m_endTurnButton->setEnabled(m_game->canEndTurn());
    m_rollButton->setEnabled(false);

    // 更新按钮外观
    if (m_game->canEndTurn()) {
        setButtonEnabledStyle(m_endTurnButton, "End Turn");
    } else {
        setButtonDisabledStyle(m_endTurnButton, "End Turn");
    }
    setButtonDisabledStyle(m_rollButton, "Roll Dice");
}

void MainWindow::ontaxPaymentRequired(const QString& taxName, int tax, int fromPlayer){
    // 创建消息框
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Tax Payment Required");
    msgBox.setText(QString("Player %1 need to pay %2 for $%3").arg(fromPlayer+1).arg(taxName).arg(tax));

    // 设置为应用模态，必须处理
    msgBox.setWindowModality(Qt::ApplicationModal);
    // 移除关闭按钮
    msgBox.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    QPushButton *payButton = msgBox.addButton("Pay Now", QMessageBox::AcceptRole);
    msgBox.setDefaultButton(payButton);

    msgBox.exec();  // 强制用户点击按钮
    Player& currentPlayer = m_game->getPlayer(fromPlayer);
    currentPlayer.deductMoney(tax);
}

void MainWindow::onLandOnSelfProperty(int playerIndex, const QString& propertyName){
    QMessageBox::information(this, "Your Property",QString("Player %1 landed on its own property:\n%2")
                                 .arg(playerIndex+1)
                                 .arg(propertyName));
}

void MainWindow::onOpenChanceCard(int playerIndex, const QString& cardDescription) {
    QMessageBox::information(this,"Chance Card",QString("Player: %1\nChance Card:\n%2")
                                 .arg(playerIndex+1).arg(cardDescription));
}

void MainWindow::onOpenCommunityCard(int playerIndex, const QString& cardDescription) {
    QMessageBox::information(this,"Community Chest Card",QString("Player: %1\nCommunity Chest Card:\n%2").arg(playerIndex+1).arg(cardDescription));
}

void MainWindow::onLandOnFreeParking(int playerIndex, int type){
    if(type==10){
        QMessageBox::information(this,"Just Visiting",QString("Player %1 is Just Visiting the Predisent's office.").arg(playerIndex+1));
    }else if(type==20){
        QMessageBox::information(this,"Free Meal", QString("Player %1 landed on Canteen!").arg(playerIndex+1));
    }
}

void MainWindow::onLandOnGoTile(int playerIndex){
    QMessageBox::information(this,"GO Tile",QString("Player: %1\nCollect $200!!!").arg(playerIndex+1));
}

void MainWindow::onLandOnGoToJailTile(int playerIndex){
    QMessageBox::information(this,"GO to President's Office Tile",QString("Player %1\nHere's a invitation from our president").arg(playerIndex+1));
}


void MainWindow::onrentPaymentRequired(const QString& property, int rent, int fromPlayer, int toPlayer){
    // 创建消息框
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Rent Payment Required");
    msgBox.setText(QString("You need to pay rent for %1").arg(property));
    msgBox.setInformativeText(QString("You must pay $%1 to Player %2?").arg(rent).arg(toPlayer+1));


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
        m_gameLog->append(QString("[PURCHASE] Player %1 bought %2 for $%3")
                              .arg(playerIndex+1).arg(propertyName).arg(price));
    } else {
        // 用户选择不购买
        m_gameLog->append(QString("[PASS] Player %1 passed on buying %2").arg(playerIndex+1).arg(propertyName));
    }
}

void MainWindow::onPlayerTurnStarted(int playerIndex) {
    const Player& player = m_game->getPlayer(playerIndex);

    int jailStatus = player.getJailStatus();
    if(jailStatus != -1){
        // current player is in the jail
        if(jailStatus == 2){
            // last chance
            showJailLastChoiceDialog(playerIndex);
        }else if(jailStatus == 0 || jailStatus == 1){
            showJailChoiceDialog(playerIndex);
        }
    }else{
        // current player can play its turn normally
        // 更新按钮状态
        m_rollButton->setEnabled(m_game->canRollDice());
        if (m_game->canRollDice()) {
            setButtonEnabledStyle(m_rollButton, "Roll Dice");
        } else {
            setButtonDisabledStyle(m_rollButton, "Cannot Roll");
        }
        m_endTurnButton->setEnabled(false);
        setButtonDisabledStyle(m_endTurnButton, "End Turn");
    }
}

void MainWindow::setButtonEnabledStyle(QPushButton* button, const QString& text) {
    button->setText(text);
    button->setStyleSheet(
        "QPushButton {"
        "   background-color: #E4A823;"      // 指定的橙色
        "   color: white;"
        "   font-weight: bold;"
        "   border: 2px solid #D49A1F;"
        "   border-radius: 6px;"
        "   padding: 10px 20px;"
        "   font-size: 14px;"
        "   min-height: 40px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #D49A1F;"      // 悬停时稍深
        "   border: 2px solid #C48A1B;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #C48A1B;"      // 按下时更深
        "   border: 2px solid #B47A17;"
        "}"
        );
}

// 禁用按钮样式
void MainWindow::setButtonDisabledStyle(QPushButton* button, const QString& text) {
    button->setText(text);
    button->setStyleSheet(
        "QPushButton {"
        "   background-color: #BDBDBD;"      // 灰色
        "   color: #757575;"
        "   border: 2px solid #9E9E9E;"
        "   border-radius: 6px;"
        "   padding: 10px 20px;"
        "   font-size: 14px;"
        "   min-height: 40px;"
        "}"
        );
}


void MainWindow::showJailLastChoiceDialog(int playerIndex) {
/*  • On the 3rd turn (jailStatus == 2):
        - The player is required to try and roll a 12.
        - If they succeed, they are freed and move a 12.
        - If they fail, they must pay $50 or use a "Get out of Jail for Free" card to get out, and continue with a normal turn.
        - If they don't have enough money, they'll be forced to raise money by selling or mortgaging property.
        - If they cannot do so, they are considered bankrupt.
*/
    Player& player = m_game->getPlayer(playerIndex);
    bool jailChoiceMade = false;

    // 第一步：必须先尝试掷骰子
    bool diceRolled = false;
    bool diceSuccess = false;

    while (!diceRolled) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Last Chance in Jail - Must Roll Dice");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(QString("Player %1 is in jail (3rd and final turn)\n\nYou MUST try to roll doubles to get out!")
                           .arg(playerIndex+1));

        QPushButton* rollButton = nullptr;
        QPushButton* manageButton = nullptr;

        // 总是可以掷骰子和管理地产
        rollButton = msgBox.addButton("Roll Dice (Try for Doubles)", QMessageBox::ActionRole);
        manageButton = msgBox.addButton("Manage Properties First", QMessageBox::ActionRole);

        msgBox.setWindowFlags(msgBox.windowFlags() & ~Qt::WindowCloseButtonHint);

        msgBox.exec();

        QAbstractButton* clickedButton = msgBox.clickedButton();

        if (clickedButton == rollButton) {
            // 执行掷骰子
            diceRolled = true;
            m_game->processJailAction(JailAction::RollForFreedom);

            // 检查结果
            diceSuccess = (player.getJailStatus() == -1);

            if (!diceSuccess) {
                // 掷骰失败，进入第二步：支付或使用卡片
                handleAfterFailedRoll(playerIndex);
            }
            // 如果成功，流程结束

        } else if (clickedButton == manageButton) {
            // 先管理财产
            this->onPropertiesClicked();
            // 循环继续，必须回到掷骰子选择
        }
    }
}

void MainWindow::handleAfterFailedRoll(int playerIndex) {
    Player& player = m_game->getPlayer(playerIndex);

    // 掷骰失败后，必须支付或使用卡片
    bool choiceMade = false;

    while (!choiceMade) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Roll Failed - Must Pay or Use Card");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(QString("Player %1 failed to roll doubles!\n\nYou MUST now pay $50 or use a Get Out of Jail Free card.")
                           .arg(playerIndex+1));

        QHash<QAbstractButton*, JailAction> buttonActions;
        QPushButton* manageButton = msgBox.addButton("Manage Properties", QMessageBox::ActionRole);

        QString infoText;

        // 根据玩家能力提供选项
        if (player.hasGetOutOfJailCard()) {
            QPushButton* cardButton = msgBox.addButton("Use Jail Card", QMessageBox::ActionRole);
            buttonActions[cardButton] = JailAction::UseCard;
            infoText = "You have a Get Out of Jail Free card.";
        }

        if (player.getCash() >= 50) {
            QPushButton* payButton = msgBox.addButton("Pay $50 Bail", QMessageBox::ActionRole);
            buttonActions[payButton] = JailAction::PayBail;
            infoText += infoText.isEmpty() ? "You have enough cash to pay bail." : "\nYou also have enough cash to pay bail.";
        }

        msgBox.setInformativeText(infoText);
        msgBox.setWindowFlags(msgBox.windowFlags() & ~Qt::WindowCloseButtonHint);

        msgBox.exec();

        QAbstractButton* clickedButton = msgBox.clickedButton();

        if (buttonActions.contains(clickedButton)) {
            // 支付或使用卡片
            m_game->processJailAction(buttonActions[clickedButton]);
            choiceMade = true;

        } else if (clickedButton == manageButton) {
            // 管理财产筹集资金
            this->onPropertiesClicked();
        }
    }
}

void MainWindow::showJailChoiceDialog(int playerIndex) {
/*
    • On the 1st and 2nd turns (jailStatus < 2):
        - The player chooses between:
            'C' → Use a "Get out of Jail for Free" card if they have one.
            'P' → Pay $50 immediately to get out and get to roll the dice.
            'T' → Try to roll a 12 (double six). If successful, they are freed and move 12 on that turn, without additional dice rolls.
                  If not, they stay in jail until their next turn.
            'S' → Stay in jail.
            'M' → Allow player to raise money to get out of jail.
*/
    Player& player = m_game->getPlayer(playerIndex);
    bool jailChoiceMade = 0;
    while(!jailChoiceMade){
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("In Jail");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(QString("Player %1 is in jail (turn %2/3)")
                           .arg(playerIndex+1)
                           .arg(player.getJailStatus() + 1));

        QString infoText = "Choose an action:";
        msgBox.setInformativeText(infoText);

        // 创建按钮并记录对应的JailAction
        QHash<QAbstractButton*, JailAction> buttonActions;

        QPushButton* rollButton = msgBox.addButton("Try Rolling a 12", QMessageBox::ActionRole);
        buttonActions[rollButton] = JailAction::RollForFreedom;

        QPushButton* stayButton = msgBox.addButton("Stay in Jail", QMessageBox::ActionRole);
        buttonActions[stayButton] = JailAction::StayInJail;

        if (player.hasGetOutOfJailCard()) {
            QPushButton* cardButton = msgBox.addButton("Use Jail Card", QMessageBox::ActionRole);
            buttonActions[cardButton] = JailAction::UseCard;
        }
        if (player.getCash() >= 50) {
            QPushButton* payButton = msgBox.addButton("Pay $50 Bail", QMessageBox::ActionRole);
            buttonActions[payButton] = JailAction::PayBail;
        }

        QPushButton* manageButton = msgBox.addButton("Manage Properties", QMessageBox::ActionRole);

        msgBox.setWindowFlags(msgBox.windowFlags() & ~Qt::WindowCloseButtonHint);

        msgBox.exec();

        QAbstractButton* clickedButton = msgBox.clickedButton();

        if (buttonActions.contains(clickedButton)) {
            // 直接调用Game的处理函数
            m_game->processJailAction(buttonActions[clickedButton]);
            jailChoiceMade=1;
        } else if (clickedButton == manageButton) {
            // 打开财产管理界面
            this->onPropertiesClicked();
            //玩家已经筹到钱了，所以要更新引用
            player = m_game->getPlayer(playerIndex);
        }
    }
}

void MainWindow::onPlayerTurnEnded(int playerIndex){
    const Player& player = m_game->getPlayer(playerIndex);
    // 更新按钮状态
    m_endTurnButton->setEnabled(m_game->canEndTurn());
    m_rollButton->setEnabled(false);

    // 更新按钮外观
    if (m_game->canEndTurn()) {
        setButtonEnabledStyle(m_endTurnButton, "End Turn");
    } else {
        setButtonDisabledStyle(m_endTurnButton, "End Turn");
    }
    setButtonDisabledStyle(m_rollButton, "Roll Dice");

}

void MainWindow::onDiceRolled(int playerIndex, int diceValue) {
    const Player& player = m_game->getPlayer(playerIndex);
    //after the logic part rolls the dice, the ui part needs to display the value of dice on the status label
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


    // 3. Game Event
    m_gameLogTitle = new QLabel("Game Events:");
    m_gameLogTitle->setStyleSheet("font-weight: bold; font-size: 16px; color: #2c3e50; margin-top: 10px;");
    controlLayout->addWidget(m_gameLogTitle);
    m_gameLog = new QTextEdit();
    m_gameLog->setMaximumHeight(150);
    m_gameLog->setReadOnly(true);
    m_gameLog->setStyleSheet(
        "QTextEdit {"
        "   color: white;"
        "   background-color: black;"
        "   border: 1px solid #bdc3c7;"
        "   border-radius: 5px;"
        "   padding: 5px;"
        "   font-size: 12px;"
        "}"
        );
    controlLayout->addWidget(m_gameLog);

    // 统一的按钮样式字符串
    QString buttonStyle = QString(
        "QPushButton {"
        "   background-color: #E4A823;"
        "   color: white;"
        "   font-weight: bold;"
        "   border: 2px solid #D49A1F;"
        "   border-radius: 6px;"
        "   padding: 10px 20px;"
        "   font-size: 14px;"
        "   min-height: 40px;"
        "   min-width: 180px;"  // 统一最小宽度
        "}"
        "QPushButton:hover {"
        "   background-color: #D49A1F;"
        "   border: 2px solid #C48A1B;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #C48A1B;"
        "   border: 2px solid #B47A17;"
        "}"
        "QPushButton:disabled {"
        "   background-color: #BDBDBD;"
        "   color: #757575;"
        "   border: 2px solid #9E9E9E;"
        "}"
        );

    // 4. My properties Button
    m_propertiesButton = new QPushButton("View My Properties");
    m_propertiesButton->setStyleSheet(buttonStyle);
    connect(m_propertiesButton, &QPushButton::clicked, this, [this]() {
        onPropertiesClicked();
    });
    controlLayout->addWidget(m_propertiesButton);

    m_tradeButton = new QPushButton("Trade Assets");
    m_tradeButton->setStyleSheet(buttonStyle);
    connect(m_tradeButton, &QPushButton::clicked, this, [this]() {
        onTradeClicked(); // To use defaults;
    });
    controlLayout->addWidget(m_tradeButton);

    // 5. Chatroom Button
    m_chatroomButton = new QPushButton("Chatroom");
    m_chatroomButton->setStyleSheet(buttonStyle);
    connect(m_chatroomButton, &QPushButton::clicked, this, &MainWindow::onChatroomClicked);
    controlLayout->addWidget(m_chatroomButton);

    // 6. AI Hint Button
    m_hintButton = new QPushButton("Get AI Hint");
    m_hintButton->setStyleSheet(buttonStyle);
    connect(m_hintButton, &QPushButton::clicked, this, &MainWindow::onHintClicked);
    controlLayout->addWidget(m_hintButton);

    // 7. Roll Dice Button
    m_rollButton = new QPushButton("Roll the Dice");
    m_rollButton->setStyleSheet(buttonStyle);
    connect(m_rollButton, &QPushButton::clicked, this, &MainWindow::onRollDiceClicked);
    controlLayout->addWidget(m_rollButton);

    // 8. End Turn Button
    m_endTurnButton = new QPushButton("End Turn");
    m_endTurnButton->setStyleSheet(buttonStyle);
    connect(m_endTurnButton, &QPushButton::clicked, this, &MainWindow::onEndTurnClicked);
    controlLayout->addWidget(m_endTurnButton);

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
            if (property->getHouses() == 0) {
                sellBuildingButton->setText("Sell Property");
            }
            QPushButton *mortgageButton = new QPushButton(property->isMortgaged() ? "Lift Mortgage" : "Mortgage");

            // Style buttons
            QString buttonStyle = "QPushButton { font-size: 11px; padding: 4px 8px; border-radius: 3px; }";
            buyBuildingButton->setStyleSheet(buttonStyle + "QPushButton { background-color: #4CAF50; color: white; }");
            sellBuildingButton->setStyleSheet(buttonStyle + "QPushButton { background-color: #FF9800; color: white; }");
            mortgageButton->setStyleSheet(buttonStyle + "QPushButton { background-color: #2196F3; color: white; }");

            // Connect functions to slots (you'll need to implement these)
            connect(buyBuildingButton, &QPushButton::clicked, [this, property, currentPlayerIndex, &propertiesDialog, playerInfo, propertyInfo, sellBuildingButton]() {
                if (property->buyBuilding(m_game->getPlayer(currentPlayerIndex), *m_game)) {
                    updatePropertyDisplay(playerInfo, propertyInfo, property, currentPlayerIndex);
                    sellBuildingButton->setText("Sell Building");
                }
            });

            connect(sellBuildingButton, &QPushButton::clicked, [this, property, currentPlayerIndex, &propertiesDialog, playerInfo, propertyInfo,
                                                                propertyFrame, forced, amountDue, sellBuildingButton]() {
                if (property->getHouses() == 0) {
                    if (property->sellProperty(m_game->getPlayer(currentPlayerIndex), *m_game)) {
                        propertyFrame->setVisible(false);
                        propertyFrame->setMaximumHeight(0);
                    }
                }
                else if (property->sellBuilding(m_game->getPlayer(currentPlayerIndex), *m_game)) {
                    updatePropertyDisplay(playerInfo, propertyInfo, property, currentPlayerIndex);
                    if (property->getHouses() == 0) {
                        sellBuildingButton->setText("Sell Property");
                    }
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

void MainWindow::onTradeClicked() {
    QDialog tradeDialog(this);
    tradeDialog.setWindowTitle("Trade Options");
    tradeDialog.setFixedSize(250, 150); // Increased size slightly

    // Set white background
    tradeDialog.setStyleSheet("QDialog { background-color: white; }");

    QVBoxLayout *layout = new QVBoxLayout(&tradeDialog);
    layout->setSpacing(8); // Reduced spacing
    layout->setContentsMargins(20, 20, 20, 20);

    // Title
    QLabel *titleLabel = new QLabel("Select Trade Option:", &tradeDialog);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("margin-bottom: 10px; color: black;");

    // Create buttons with simple styling
    QPushButton *buyButton = new QPushButton("Buy from Players", &tradeDialog);
    QPushButton *sellButton = new QPushButton("Sell to Players", &tradeDialog);
    QPushButton *cancelButton = new QPushButton("Cancel", &tradeDialog);

    // Base button style with reasonable sizing
    QString buttonStyle = "QPushButton { "
                          "font-size: 12px; "
                          "padding: 2px; "
                          "border-radius: 5px; "
                          "font-weight: bold; "
                          "border: 1px solid #ddd; "
                          "max-height: 40px; "  // Fixed height
                          "margin-bottom: 10px; "  // Small margin between buttons
                          "}";

    // Apply colors
    buyButton->setStyleSheet(buttonStyle +
                             "QPushButton { background-color: #4CAF50; color: white; border: none; }"
                             "QPushButton:hover { background-color: #45a049; }");

    sellButton->setStyleSheet(buttonStyle +
                              "QPushButton { background-color: #FF9800; color: white; border: none; }"
                              "QPushButton:hover { background-color: #e68900; }");

    cancelButton->setStyleSheet(buttonStyle +
                                "QPushButton { background-color: #9E9E9E; color: white; border: none; }"
                                "QPushButton:hover { background-color: #757575; }");

    // Add stretch at top and bottom to center buttons
    layout->addStretch(1);
    layout->addWidget(titleLabel);
    layout->addWidget(buyButton);
    layout->addWidget(sellButton);
    layout->addWidget(cancelButton);
    layout->addStretch(1);

    // Connect signals
    connect(buyButton, &QPushButton::clicked, &tradeDialog, [&]() {
        tradeDialog.accept();
        qDebug() << "Buy from players selected";
        onBuyTrade();
        // TODO: Implement buy logic
    });

    connect(sellButton, &QPushButton::clicked, &tradeDialog, [&]() {
        tradeDialog.accept();
        onSellTrade();
        qDebug() << "Sell to players selected";
        // TODO: Implement sell logic
    });

    connect(cancelButton, &QPushButton::clicked, &tradeDialog, &QDialog::reject);

    if (tradeDialog.exec() == QDialog::Accepted) {
        // Already handled in lambdas
    } else {
        qDebug() << "Trade cancelled";
    }
}

void MainWindow::onSellTrade() {
    int currentPlayerIndex = m_game->getCurrentPlayerIndex();
    Player& currentPlayer = m_game->getPlayer(currentPlayerIndex);

    QDialog dialog(this);
    dialog.setWindowTitle("Sell Properties");
    dialog.setMinimumSize(500, 400);
    dialog.setStyleSheet("QDialog { background-color: white; }"
                         "QMessageBox QLabel { color: black; }"
                         "QMessageBox QPushButton { color: black; }");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(5);

    // Get current player's properties
    std::vector<PropertyTile*> myProperties = currentPlayer.getProperties();

    if (myProperties.empty()) {
        QLabel *noPropertiesLabel = new QLabel("You don't own any properties to sell");
        noPropertiesLabel->setStyleSheet("QLabel { color: gray; padding: 20px; font-weight: bold; }");
        noPropertiesLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(noPropertiesLabel);

        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
        connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
        layout->addWidget(buttonBox);

        dialog.exec();
        return;
    }

    // Property selection
    QLabel *propLabel = new QLabel("Select Property to Sell:");
    propLabel->setStyleSheet("QLabel { font-weight: bold; margin-top: 10px; color: black;}");
    layout->addWidget(propLabel);

    // Scroll area for properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: 1px solid #ccc; background-color: white; }");
    scrollArea->setMinimumHeight(150);

    QWidget *propertiesContainer = new QWidget();
    QVBoxLayout *propertiesLayout = new QVBoxLayout(propertiesContainer);
    propertiesLayout->setContentsMargins(5, 5, 5, 5);
    propertiesLayout->setSpacing(8);

    modifyTradeSelectionWidget(currentPlayer, myProperties, dialog, propertiesContainer, propertiesLayout);

    propertiesLayout->addStretch();
    scrollArea->setWidget(propertiesContainer);
    layout->addWidget(scrollArea);

    // Buyer label with better contrast
    QLabel *buyerLabel = new QLabel("Sell to:");
    buyerLabel->setStyleSheet("QLabel { font-weight: bold; margin-top: 10px; color: #222;}");  // Darker color
    layout->addWidget(buyerLabel);

    QComboBox *playerCombo = new QComboBox(&dialog);
    playerCombo->setStyleSheet("QComboBox { "
                               "border: 2px solid #888; "
                               "border-radius: 4px; "
                               "padding: 6px; "
                               "min-width: 200px; "
                               "}");

    // Add all players except current player
    for (int i = 0; i < 3; i++) {
        if (i != currentPlayerIndex) {
            const Player& otherPlayer = m_game->getPlayer(i);
            if (!otherPlayer.isBankrupt()) {
                playerCombo->addItem(QString("Player %1 (Cash: $%2)").arg(i + 1).arg(otherPlayer.getCash()), i);
            }
        }
    }
    layout->addWidget(playerCombo);

    // Amount section with better contrast
    QLabel *amountLabel = new QLabel("Asking Price:");
    amountLabel->setStyleSheet("QLabel { font-weight: bold; margin-top: 10px; color: #222; }");  // Darker color
    layout->addWidget(amountLabel);

    QHBoxLayout *amountLayout = new QHBoxLayout();
    QLabel *dollarLabel = new QLabel("$");
    dollarLabel->setStyleSheet("QLabel { font-weight: bold; color: #222; }");  // Darker color
    QLineEdit *amountEdit = new QLineEdit(&dialog);
    amountEdit->setPlaceholderText("Enter asking price...");
    amountEdit->setValidator(new QIntValidator(1, 1000000, this));
    amountEdit->setStyleSheet("QLineEdit { padding: 6px; border: 1px solid #999; border-radius: 3px; }");  // Better border

    amountLayout->addWidget(dollarLabel);
    amountLayout->addWidget(amountEdit);
    layout->addLayout(amountLayout);

    // Buttons with better contrast
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(10);  // More spacing

    QPushButton *makeOfferButton = new QPushButton("Make Offer");
    QPushButton *cancelButton = new QPushButton("Cancel");

    // Style buttons with better contrast
    QString buttonStyle = "QPushButton { ""font-size: 12px;" "padding: 8px 16px;" "border-radius: 3px;"
                          "font-weight: bold;" "border: 1px solid;" "color: black;" "}";

    makeOfferButton->setStyleSheet(buttonStyle + "background-color: #FF9800;" "color: white;" "border-color: #E68900;");  // Darker border for contrast

    cancelButton->setStyleSheet(buttonStyle + "background-color: #757575;" "color: white;" "border-color: #616161;");  // Darker border

    makeOfferButton->setMinimumWidth(120);
    cancelButton->setMinimumWidth(120);

    btnLayout->addStretch();
    btnLayout->addWidget(makeOfferButton);
    btnLayout->addWidget(cancelButton);
    layout->addLayout(btnLayout);

    // Connect signals
    connect(makeOfferButton, &QPushButton::clicked, &dialog, [&]() {
        bool isJailCard = dialog.property("isJailCard").toBool();

        if (!isJailCard) {
            // Check if property is selected
            if (!dialog.property("selectedProperty").value<PropertyTile*>()) {
                QMessageBox::warning(&dialog, "No Property Selected", "Please select a property to sell.");
                return;
            }
        }

        // Check if amount is entered
        if (amountEdit->text().isEmpty()) {
            QMessageBox::warning(&dialog, "No Amount",
                                 "Please enter an asking price.");
            return;
        }

        int buyerIndex = playerCombo->currentData().toInt();
        Player& buyer = m_game->getPlayer(buyerIndex);
        int amount = amountEdit->text().toInt();

        QString buyerName = QString("Player %1").arg(buyerIndex + 1);

        dialog.accept();

        if (isJailCard) {
            // Check if property is selected
            if (buyer.getCash() < amount) {
                QMessageBox::warning(&dialog, "Invalid Price",
                                     QString("Player %1 does not have the proposed amount of money. Please enter a lower price to proceed").arg(buyerIndex));
                return;
            }
            QMessageBox::information(&dialog, "Offer Sent",
                                     QString("Your offer to sell 🎫 Get Out of Jail Free Card to %2 for $%3 has been sent!").arg(buyerName).arg(amount));
            askTradeDecision(&currentPlayer, &buyer, amount, true, nullptr, false);
        }
        else {
            PropertyTile* selectedProperty = dialog.property("selectedProperty").value<PropertyTile*>();
            if (!selectedProperty) {
                QMessageBox::warning(&dialog, "No Property Selected",
                                     "Please select a property to sell.");
                return;
            }
            amount += selectedProperty->isMortgaged() ? static_cast<int>((selectedProperty->getPrice() / 2) * 0.1) : 0;
            if (buyer.getCash() < amount) {
                QMessageBox::warning(&dialog, "Invalid Price",
                                     QString("Player %1 does not have the proposed amount of money. Please enter a lower price to proceed").arg(buyerIndex));
                return;
            }
            QMessageBox::information(&dialog, "Offer Sent",
                                     QString("Your offer to sell %1 to %2 for $%3 has been sent!")
                                         .arg(QString::fromStdString(selectedProperty->getName())).arg(buyerName).arg(amount));
            askTradeDecision(&currentPlayer, &buyer, amount, false, selectedProperty, false);
        }
    });

    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.exec();
}

void MainWindow::onBuyTrade() {
    int currentPlayerIndex = m_game->getCurrentPlayerIndex();
    Player& currentPlayer = m_game->getPlayer(currentPlayerIndex);

    QDialog dialog(this);
    dialog.setWindowTitle("Buy Properties");
    dialog.setMinimumSize(500, 400);
    dialog.setStyleSheet("QDialog { background-color: white; }"
                         "QMessageBox QLabel { color: black; }"
                         "QMessageBox QPushButton { color: black; }");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(5);

    // Seller selection
    QLabel *sellerLabel = new QLabel("Buy from:");
    sellerLabel->setStyleSheet("QLabel { font-weight: bold; margin-top: 10px; color: black; }");
    layout->addWidget(sellerLabel);

    QComboBox *playerCombo = new QComboBox(&dialog);
    playerCombo->setStyleSheet("QComboBox { "
                               "border: 2px solid #888; "
                               "border-radius: 4px; "
                               "padding: 6px; "
                               "min-width: 200px; "
                               "}");

    // Store available sellers
    QVector<Player*> availableSellers;
    for (int i = 0; i < 3; i++) {
        if (i != currentPlayerIndex) {
            Player& otherPlayer = m_game->getPlayer(i);
            if (!otherPlayer.isBankrupt()) {
                playerCombo->addItem(QString("Player %1 (Cash: $%2)").arg(i + 1).arg(otherPlayer.getCash()), i);
                availableSellers.append(&otherPlayer);
            }
        }
    }

    if (availableSellers.isEmpty()) {
        QLabel *noSellersLabel = new QLabel("No other players available");
        noSellersLabel->setStyleSheet("QLabel { color: gray; padding: 20px; font-weight: bold; }");
        noSellersLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(noSellersLabel);

        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
        connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
        layout->addWidget(buttonBox);

        dialog.exec();
        return;
    }

    layout->addWidget(playerCombo);

    // Property selection
    QLabel *propLabel = new QLabel("Select Property to Buy:");
    propLabel->setStyleSheet("QLabel { font-weight: bold; margin-top: 10px; color: black; }");
    layout->addWidget(propLabel);

    // Scroll area for properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: 1px solid #ccc; background-color: white; }");
    scrollArea->setMinimumHeight(150);

    QWidget *propertiesContainer = new QWidget();
    QVBoxLayout *propertiesLayout = new QVBoxLayout(propertiesContainer);
    propertiesLayout->setContentsMargins(5, 5, 5, 5);
    propertiesLayout->setSpacing(8);

    // Function to update properties when seller changes
    auto updateProperties = [&]() {
        // Clear previous properties
        QLayoutItem* item;
        while ((item = propertiesLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }

        int sellerIndex = playerCombo->currentIndex();
        if (sellerIndex < 0 || sellerIndex >= availableSellers.size()) return;

        Player* seller = availableSellers[sellerIndex];
        std::vector<PropertyTile*> sellerProperties = seller->getProperties();

        if (sellerProperties.empty()) {
            QLabel *noPropsLabel = new QLabel("This player has no properties to sell");
            noPropsLabel->setStyleSheet("QLabel { color: gray; padding: 20px; }");
            noPropsLabel->setAlignment(Qt::AlignCenter);
            propertiesLayout->addWidget(noPropsLabel);
        } else {
            modifyTradeSelectionWidget(*seller, sellerProperties, dialog, propertiesContainer, propertiesLayout);
        }

        // Reset selection
        dialog.setProperty("selectedProperty", QVariant());
        dialog.setProperty("isJailCard", false);
    };

    // Initial property display
    updateProperties();

    propertiesLayout->addStretch();
    scrollArea->setWidget(propertiesContainer);
    layout->addWidget(scrollArea);

    // Connect seller combo change
    connect(playerCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [updateProperties](int index) { updateProperties(); });

    // Amount section
    QLabel *amountLabel = new QLabel("Your Offer:");
    amountLabel->setStyleSheet("QLabel { font-weight: bold; margin-top: 10px; color: #222; }");
    layout->addWidget(amountLabel);

    QHBoxLayout *amountLayout = new QHBoxLayout();
    QLabel *dollarLabel = new QLabel("$");
    dollarLabel->setStyleSheet("QLabel { font-weight: bold; color: #222; }");
    QLineEdit *amountEdit = new QLineEdit(&dialog);
    amountEdit->setPlaceholderText("Enter offer price...");
    amountEdit->setValidator(new QIntValidator(1, 1000000, this));
    amountEdit->setStyleSheet("QLineEdit { padding: 6px; border: 1px solid #999; border-radius: 3px; }");

    amountLayout->addWidget(dollarLabel);
    amountLayout->addWidget(amountEdit);
    layout->addLayout(amountLayout);

    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(10);

    QPushButton *makeOfferButton = new QPushButton("Make Offer");
    QPushButton *cancelButton = new QPushButton("Cancel");

    QString buttonStyle = "QPushButton { "
                          "font-size: 12px; "
                          "padding: 8px 16px; "
                          "border-radius: 3px; "
                          "font-weight: bold; "
                          "border: 1px solid; "
                          "color: black;"
                          "}";

    makeOfferButton->setStyleSheet(buttonStyle +
                                   "background-color: #4CAF50; "
                                   "color: white; "
                                   "border-color: #388E3C;");

    cancelButton->setStyleSheet(buttonStyle +
                                "background-color: #757575; "
                                "color: white; "
                                "border-color: #616161;");

    makeOfferButton->setMinimumWidth(120);
    cancelButton->setMinimumWidth(120);

    btnLayout->addStretch();
    btnLayout->addWidget(makeOfferButton);
    btnLayout->addWidget(cancelButton);
    layout->addLayout(btnLayout);

    // Connect signals
    connect(makeOfferButton, &QPushButton::clicked, &dialog, [&]() {
        bool isJailCard = dialog.property("isJailCard").toBool();

        if (!isJailCard) {
            // Check if property is selected
            if (!dialog.property("selectedProperty").value<PropertyTile*>()) {
                QMessageBox::warning(&dialog, "No Property Selected",
                                     "Please select a property to buy.");
                return;
            }
        }

        // Check if amount is entered
        if (amountEdit->text().isEmpty()) {
            QMessageBox::warning(&dialog, "No Amount",
                                 "Please enter an offer price.");
            return;
        }

        int sellerIndex = playerCombo->currentData().toInt();
        Player& seller = m_game->getPlayer(sellerIndex);
        int amount = amountEdit->text().toInt();

        QString sellerName = QString("Player %1").arg(sellerIndex + 1);

        dialog.accept();

        if (isJailCard) {
            // Check if player has enough cash
            if (currentPlayer.getCash() < amount) {
                QMessageBox::warning(&dialog, "Invalid Price",
                                     "You do not have the proposed amount of money. Please enter a lower price to proceed");
                return;
            }
            QMessageBox::information(this, "Offer Sent",
                                     QString("Your offer to buy 🎫 Get Out of Jail Free Card from %2 for $%3 has been sent!")
                                         .arg(sellerName)
                                         .arg(amount));
            askTradeDecision(&currentPlayer, &seller, amount, true, nullptr, true);
        } else {
            PropertyTile* selectedProperty = dialog.property("selectedProperty").value<PropertyTile*>();
            if (!selectedProperty) {
                QMessageBox::warning(&dialog, "No Property Selected",
                                     "Please select a property to buy.");
                return;
            }

            // Add mortgage fee if property is mortgaged
            amount += selectedProperty->isMortgaged() ? static_cast<int>((selectedProperty->getPrice() / 2) * 0.1) : 0;

            // Check if player has enough cash
            if (currentPlayer.getCash() < amount) {
                QMessageBox::warning(&dialog, "Invalid Price",
                                     "You do not have the proposed amount of money. Please enter a lower price to proceed");
                return;
            }

            amount -= selectedProperty->isMortgaged() ? static_cast<int>((selectedProperty->getPrice() / 2) * 0.1) : 0;

            QMessageBox::information(this, "Offer Sent",
                                     QString("Your offer to buy %1 from %2 for $%3 has been sent!")
                                         .arg(QString::fromStdString(selectedProperty->getName()))
                                         .arg(sellerName)
                                         .arg(amount));
            askTradeDecision(&currentPlayer, &seller, amount, false, selectedProperty, true);
        }
    });

    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.exec();
}

void MainWindow::modifyTradeSelectionWidget(const Player& currentPlayer,
                                            const std::vector<PropertyTile*>& myProperties,
                                            QDialog& dialog,
                                            QWidget* propertiesContainer,
                                            QVBoxLayout* propertiesLayout) {
    // Jail card if player has one
    if (currentPlayer.hasGetOutOfJailCard()) {
        QFrame *cardFrame = new QFrame();
        cardFrame->setFrameStyle(QFrame::Box);
        cardFrame->setStyleSheet("QFrame { border: 1px solid #ddd; border-radius: 4px; padding: 8px; background-color: #e3f2fd; }"); // Light blue background

        QVBoxLayout *cardLayout = new QVBoxLayout(cardFrame);
        cardLayout->setSpacing(4);

        // Card information
        QString cardText = QString("🎫 Get Out of Jail Free Card");
        QLabel *cardInfo = new QLabel(cardText);
        cardInfo->setTextFormat(Qt::PlainText);
        cardInfo->setStyleSheet("QLabel { font-size: 12px; margin: 0px; color: #1565C0; font-weight: bold; }");
        cardLayout->addWidget(cardInfo);

        // Select button for card
        QPushButton *selectCardButton = new QPushButton("Select This Card");
        selectCardButton->setStyleSheet("QPushButton { font-size: 11px; padding: 4px 8px; border-radius: 3px; background-color: #9C27B0; color: white; }");

        // Store special identifier for card
        selectCardButton->setProperty("isJailCard", true);

        connect(selectCardButton, &QPushButton::clicked,
                [&dialog, selectCardButton, propertiesContainer]() {
                    // Highlight selected button
                    QList<QPushButton*> allButtons = propertiesContainer->findChildren<QPushButton*>();
                    for (QPushButton* btn : allButtons) {
                        if (btn->property("isJailCard").toBool()) {
                            btn->setStyleSheet("QPushButton { font-size: 11px; padding: 4px 8px; border-radius: 3px; background-color: #9C27B0; color: white; }");
                        } else {
                            btn->setStyleSheet("QPushButton { font-size: 11px; padding: 4px 8px; border-radius: 3px; background-color: #2196F3; color: white; }");
                        }
                    }
                    selectCardButton->setStyleSheet("QPushButton { font-size: 11px; padding: 4px 8px; border-radius: 3px; background-color: #FF5722; color: white; }");

                    // Store selected card (use nullptr for property to indicate it's a card)
                    dialog.setProperty("selectedProperty", QVariant::fromValue<PropertyTile*>(nullptr));
                    dialog.setProperty("isJailCard", true);

                    // Update info label
                    QString info = QString("<b>Selected: Get Out of Jail Free Card</b><br>"
                                           "This card can be traded to other players.");
                });

        cardLayout->addWidget(selectCardButton);
        propertiesLayout->addWidget(cardFrame);
    }

    // Now the property loop
    for (PropertyTile* property : myProperties) {
        if (!property->allPropertyInGroupHasNoHouses()) {
            continue;
        }
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

        // Select button
        QPushButton *selectButton = new QPushButton("Select This Property");
        selectButton->setStyleSheet("QPushButton { font-size: 11px; padding: 4px 8px; border-radius: 3px; background-color: #2196F3; color: white; }");

        // Store property pointer as property data
        selectButton->setProperty("propertyPtr", QVariant::fromValue<PropertyTile*>(property));

        // FIXED: Proper lambda capture - capture all needed variables
        connect(selectButton, &QPushButton::clicked,
                [&dialog, property, selectButton, propertiesContainer]() {
                    // Highlight selected property
                    QList<QPushButton*> allButtons = propertiesContainer->findChildren<QPushButton*>();
                    for (QPushButton* btn : allButtons) {
                        if (btn->property("isJailCard").toBool()) {
                            btn->setStyleSheet("QPushButton { font-size: 11px; padding: 4px 8px; border-radius: 3px; background-color: #9C27B0; color: white; }");
                        } else {
                            btn->setStyleSheet("QPushButton { font-size: 11px; padding: 4px 8px; border-radius: 3px; background-color: #2196F3; color: white; }");
                        }
                    }
                    selectButton->setStyleSheet("QPushButton { font-size: 11px; padding: 4px 8px; border-radius: 3px; background-color: #FF5722; color: white; }");

                    // Store selected property
                    dialog.setProperty("selectedProperty", QVariant::fromValue<PropertyTile*>(property));

                    // Update info label
                    QString info = QString("<b>Selected: %1</b><br>"
                                           "Price: $%2 | Houses: %3")
                                       .arg(QString::fromStdString(property->getName()))
                                       .arg(property->getPrice())
                                       .arg(property->getHouses());

                    if (property->isMortgaged()) {
                        info += "<br><span style='color: red;'>This property is mortgaged</span>";
                    }
                });

        propertyLayout->addWidget(selectButton);
        propertiesLayout->addWidget(propertyFrame);
    }
}

void MainWindow::askTradeDecision(Player* offeringPlayer, Player* targetPlayer,
                                  int amount, bool card, PropertyTile* property, bool offerBuy) {

    QString title = QString("Trade Offer");
    QString message;

    QString action = offerBuy ? "buy from you" : "sell to you";


    message = QString("<b>Player %1's Screen</b><br><br>").arg(targetPlayer->getName());
    if (card) {
        message.append(QString("Player %1 offers to %2 <b>Get Out of Jail Free</b> card<br>for <b>$%3</b>.")
                           .arg(QString::fromStdString(offeringPlayer->getName()))
                           .arg(action)
                           .arg(amount));
    } else {
        QString mortgageNote = property->isMortgaged() ? "<br><i>(Mortgaged - fee included)</i>" : "";
        message.append(QString("Player %1 offers to %2 <b>%3</b>%4<br>for <b>$%5</b>.")
                           .arg(QString::fromStdString(offeringPlayer->getName()))
                           .arg(action)
                           .arg(QString::fromStdString(property->getName()))
                           .arg(mortgageNote)
                           .arg(amount));
    }

    QMessageBox tradeBox(this);
    tradeBox.setWindowTitle(title);
    tradeBox.setText(message);

    tradeBox.setIcon(QMessageBox::NoIcon);

    // Cleaner minimal styling
    tradeBox.setStyleSheet(
        "QMessageBox { background-color: white; }"
        "QLabel { color: black; font-size: 14px; padding: 4px; }"
        );

    // Buttons
    QPushButton *acceptButton = tradeBox.addButton("Accept", QMessageBox::AcceptRole);
    QPushButton *declineButton = tradeBox.addButton("Decline", QMessageBox::RejectRole);

    acceptButton->setStyleSheet(
        "QPushButton { padding: 8px 24px; background-color: green; color: white; border-radius: 4px; }"
        );
    declineButton->setStyleSheet(
        "QPushButton { padding: 8px 24px; background-color: red; color: white; border-radius: 4px; }"
        );

    // Ensure the button box behaves normally
    if (auto *buttonBox = tradeBox.findChild<QDialogButtonBox*>()) {
        buttonBox->setCenterButtons(false);
    }

    tradeBox.exec();

    if (tradeBox.clickedButton() == acceptButton) {
        if (offerBuy) {
            m_game->executeTrade(offeringPlayer, targetPlayer, amount, card, property);
        } else {
            m_game->executeTrade(targetPlayer, offeringPlayer, amount, card, property);
        }
        QMessageBox::information(&tradeBox, "", "✓ Trade Accepted!");
    } else if (tradeBox.clickedButton() == declineButton) {
        QMessageBox::information(&tradeBox, "", "Trade Declineds!");
    }
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


    std::vector<PropertyTile*> properties = currentPlayer.getProperties();

}
