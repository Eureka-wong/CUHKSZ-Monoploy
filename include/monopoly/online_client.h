#pragma once

#include <QWebSocket>
#include <QObject>
#include <QTimer>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "core_types.h"

namespace monopoly {

// 在线游戏客户端（基于WebSocket）
class OnlineGameClient : public QObject {
    Q_OBJECT

public:
    explicit OnlineGameClient(QObject* parent = nullptr);
    ~OnlineGameClient();

    // 连接管理
    void connectToServer(const QString& serverUrl);
    void disconnect();
    bool isConnected() const;

    // 房间操作
    void createRoom(const QString& roomId, int maxPlayers = 4, const QString& password = "");
    void listRooms();
    void joinRoom(const QString& roomId, const QString& playerName, bool spectator = false, const QString& password = "");
    void leaveRoom();

    // 游戏操作
    void startGame();
    void rollDice();
    void buyProperty(const QString& property, int price);
    void sellProperty(const QString& property, int price);
    void endTurn();
    void sendChat(const QString& message, int targetPlayerId = -1);  // -1 表示群发，否则为私聊目标
    void useHint(int cost = 50);
    void viewProperty();  // 查看房产信息
    void payBail();  // 支付 $50 保释金出狱

    // 回调设置
    void setOnConnected(std::function<void()> cb) { onConnected_ = cb; }
    void setOnDisconnected(std::function<void()> cb) { onDisconnected_ = cb; }
    void setOnError(std::function<void(const QString&)> cb) { onError_ = cb; }
    void setOnJoinedRoom(std::function<void(const QString&, int, const StateSnapshot&)> cb) { onJoinedRoom_ = cb; }
    void setOnRoomList(std::function<void(const std::vector<RoomSummary>&)> cb) { onRoomList_ = cb; }
    void setOnStateUpdate(std::function<void(const StateSnapshot&)> cb) { onStateUpdate_ = cb; }
    void setOnEvent(std::function<void(const GameEvent&)> cb) { onEvent_ = cb; }
    void setOnChat(std::function<void(int, const QString&, const QString&, bool, int)> cb) { onChat_ = cb; }

    // 获取当前状态
    QString getCurrentRoomId() const { return currentRoomId_; }
    int getMyPlayerId() const { return myPlayerId_; }

private slots:
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError error);
    void onTextMessageReceived(const QString& message);
    void sendHeartbeat();

private:
    void sendMessage(const QJsonObject& msg);
    void handleMessage(const QJsonObject& msg);
    void handleJoined(const QJsonObject& msg);
    void handleRoomList(const QJsonObject& msg);
    void handleStateUpdate(const QJsonObject& msg);
    void handleEvent(const QJsonObject& msg);
    void handleChat(const QJsonObject& msg);
    void handleError(const QJsonObject& msg);

    QWebSocket* websocket_;
    QTimer* heartbeatTimer_;
    QString serverUrl_;
    QString currentRoomId_;
    int myPlayerId_{-1};
    bool connected_{false};

    // 回调函数
    std::function<void()> onConnected_;
    std::function<void()> onDisconnected_;
    std::function<void(const QString&)> onError_;
    std::function<void(const QString&, int, const StateSnapshot&)> onJoinedRoom_;
    std::function<void(const std::vector<RoomSummary>&)> onRoomList_;
    std::function<void(const StateSnapshot&)> onStateUpdate_;
    std::function<void(const GameEvent&)> onEvent_;
    std::function<void(int, const QString&, const QString&, bool, int)> onChat_;  // playerId, playerName, message, isPrivate, targetPlayerId
};

}  // namespace monopoly

