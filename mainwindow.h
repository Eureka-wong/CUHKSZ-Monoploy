#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QTextEdit>
#include <QDialog>

class Game;
class BoardWidget;
class PropertyTile;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onRollDiceClicked();

    void onPropertiesClicked(int playerIndex = -1, bool forced = false, int amountDue = 0);

    void updatePropertyDisplay(QLabel* playerInfo, QLabel* propertyInfo, PropertyTile* property, int currentPlayerIndex);
    void showWarning(const QString& message);

    void onHintClicked();

    void onChatroomClicked();

    void onEndTurnClicked();


    void onPlayerTurnStarted(int playerIndex);
    void onPlayerTurnEnded(int playerIndex);

    void onDiceRolled(int playerIndex, int diceValue);

    void onPlayerMoved(int playerIndex, int oldPosition, int newPosition);

    void onPurchaseOpportunity(const QString& propertyName, int price, int playerIndex);

    void onGameLogMessage(const QString& message);

    // signal: gameStarted
    //void onGameStarted();

    // signal: gameStateChanged
    //void onGameStateChanged();

    //signal: gameOver
    //void onGameOver(int winnerPlayerIndex);

    void onPlayerBankrupt(int playerIndex);

    void onrentPaymentRequired(const QString& propertyName, int rent, int fromPlayer, int toPlayer);

    void onForceRaiseMoney(int payerIndex, int amountDue);


private:
    Ui::MainWindow *ui;
    Game* m_game;

    BoardWidget* m_boardWidget;

    QPushButton* m_rollButton;
    QPushButton* m_propertiesButton;
    QPushButton* m_endTurnButton;
    QPushButton* m_chatroomButton;
    QPushButton* m_hintButton;

    QLabel* m_roundLabel;
    QLabel* m_cashLabel;
    QLabel* m_gameLogTitle;
    QLabel* m_statusLabel;

    QLabel* m_playerInfoLabel;

    QDialog* m_propertiesDialog = nullptr;

    QTextEdit* m_gameLog;

    bool handlePurchaseDialog(const std::string& propertyName, int price, const std::string& playerName);
    void setupPurchaseCallback();

    void setupUI();
    void updateDisplay();

    void updatePlayerInfo();
    void updateOtherPlayersInfo();
    void addGameLog(const QString& message);
    int getMyPlayerId() const;  // 临时方法，后续替换为网络获取
    void setupGameConnections(); // 新增：设置游戏信号连接

    void showPropertiesDialog();
    void refreshPropertiesDialog();
};
#endif // MAINWINDOW_H
