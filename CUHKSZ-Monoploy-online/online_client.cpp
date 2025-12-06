#include "monopoly/online_client.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <iostream>

namespace monopoly {

OnlineGameClient::OnlineGameClient(QObject* parent)
    : QObject(parent)
    , websocket_(new QWebSocket("", QWebSocketProtocol::VersionLatest, this))
    , heartbeatTimer_(new QTimer(this))
    , myPlayerId_(-1)
    , connected_(false)
{
    // connect WebSocket signal
    connect(websocket_, &QWebSocket::connected, this, &OnlineGameClient::onConnected);
    connect(websocket_, &QWebSocket::disconnected, this, &OnlineGameClient::onDisconnected);
    connect(websocket_, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
            this, &OnlineGameClient::onError);
    connect(websocket_, &QWebSocket::textMessageReceived,
            this, &OnlineGameClient::onTextMessageReceived);

    // heartbeat timer
    heartbeatTimer_->setInterval(30000);
    connect(heartbeatTimer_, &QTimer::timeout, this, &OnlineGameClient::sendHeartbeat);
}

OnlineGameClient::~OnlineGameClient() {
    disconnect();
}

void OnlineGameClient::connectToServer(const QString& serverUrl) {
    if (websocket_->state() == QAbstractSocket::ConnectedState) {
        if (onError_) {
            onError_("Already connected to server");
        }
        return;
    }

    serverUrl_ = serverUrl;
    QUrl url(serverUrl);
    if (!url.isValid()) {
        if (onError_) {
            onError_("Invalid server URL: " + serverUrl);
        }
        return;
    }

    std::cout << "[OnlineClient] Connecting to " << serverUrl.toStdString() << "\n";
    websocket_->open(url);
}

void OnlineGameClient::disconnect() {
    heartbeatTimer_->stop();
    if (websocket_->state() == QAbstractSocket::ConnectedState) {
        websocket_->close();
    }
    connected_ = false;
    currentRoomId_.clear();
    myPlayerId_ = -1;
}

bool OnlineGameClient::isConnected() const {
    return connected_ && websocket_->state() == QAbstractSocket::ConnectedState;
}

void OnlineGameClient::createRoom(const QString& roomId, int maxPlayers, const QString& password) {
    QJsonObject msg;
    msg["type"] = "create_room";
    msg["roomId"] = roomId;
    msg["maxPlayers"] = maxPlayers;
    if (!password.isEmpty()) {
        msg["password"] = password;
    }
    sendMessage(msg);
}

void OnlineGameClient::listRooms() {
    QJsonObject msg;
    msg["type"] = "list_rooms";
    sendMessage(msg);
}

void OnlineGameClient::joinRoom(const QString& roomId, const QString& playerName, bool spectator, const QString& password) {
    QJsonObject msg;
    msg["type"] = "join_room";
    msg["roomId"] = roomId;
    msg["playerName"] = playerName;
    msg["spectator"] = spectator;
    if (!password.isEmpty()) {
        msg["password"] = password;
    }
    sendMessage(msg);
}

void OnlineGameClient::leaveRoom() {
    if (!currentRoomId_.isEmpty()) {
        QJsonObject msg;
        msg["type"] = "leave_room";
        sendMessage(msg);
    }
    disconnect();
}

void OnlineGameClient::startGame() {
    std::cout << "[Client] startGame() called" << std::endl;
    QJsonObject msg;
    msg["type"] = "action";
    msg["actionType"] = "start_game";
    std::cout << "[Client] Sending start_game action to server" << std::endl;
    sendMessage(msg);
}

void OnlineGameClient::rollDice() {
    std::cout << "[Client] rollDice() called" << std::endl;
    QJsonObject msg;
    msg["type"] = "action";
    msg["actionType"] = "roll";
    std::cout << "[Client] Sending roll action to server" << std::endl;
    sendMessage(msg);
}

void OnlineGameClient::buyProperty(const QString& property, int price) {
    std::cout << "[Client] buyProperty() called: " << property.toStdString() << " for $" << price << std::endl;
    QJsonObject msg;
    msg["type"] = "action";
    msg["actionType"] = "buy";
    QJsonObject details;
    details["propertyId"] = property.toInt();  
    details["cost"] = price;  
    msg["details"] = details;
    std::cout << "[Client] Sending buy property action to server: propertyId=" 
              << property.toInt() << ", cost=" << price << std::endl;
    sendMessage(msg);
}

void OnlineGameClient::sellProperty(const QString& property, int price) {
    std::cout << "[Client] sellProperty() called: " << property.toStdString() << " for $" << price << std::endl;
    QJsonObject msg;
    msg["type"] = "action";
    msg["actionType"] = "sell";
    QJsonObject details;
    details["propertyId"] = property.toInt(); 
    details["price"] = price;
    msg["details"] = details;
    std::cout << "[Client] Sending sell property action to server: propertyId=" 
              << property.toInt() << ", price=" << price << std::endl;
    sendMessage(msg);
}

void OnlineGameClient::endTurn() {
    std::cout << "[Client] endTurn() called" << std::endl;
    QJsonObject msg;
    msg["type"] = "action";
    msg["actionType"] = "end";
    std::cout << "[Client] Sending end turn action to server" << std::endl;
    sendMessage(msg);
}

void OnlineGameClient::payBail() {
    std::cout << "[Client] payBail() called" << std::endl;
    QJsonObject msg;
    msg["type"] = "action";
    msg["actionType"] = "pay_bail";
    std::cout << "[Client] Sending pay bail action to server" << std::endl;
    sendMessage(msg);
}

void OnlineGameClient::sendChat(const QString& message, int targetPlayerId) {
    std::cout << "[Client] sendChat() called: " << message.toStdString();
    if (targetPlayerId >= 0) {
        std::cout << " (private to player " << targetPlayerId << ")";
    } else {
        std::cout << " (group chat)";
    }
    std::cout << std::endl;
    
    QJsonObject msg;
    msg["type"] = "chat";
    msg["message"] = message;
    if (targetPlayerId >= 0) {
        msg["targetPlayerId"] = targetPlayerId;  
    }
    std::cout << "[Client] Sending chat message to server" << std::endl;
    sendMessage(msg);
}

void OnlineGameClient::useHint(int cost) {
    std::cout << "[Client] useHint() called with cost: $" << cost << std::endl;
    QJsonObject msg;
    msg["type"] = "action";
    msg["actionType"] = "hint";
    QJsonObject details;
    details["cost"] = cost;
    msg["details"] = details;
    std::cout << "[Client] Sending hint action to server" << std::endl;
    sendMessage(msg);
}

void OnlineGameClient::viewProperty() {
    std::cout << "[Client] viewProperty() called" << std::endl;
    QJsonObject msg;
    msg["type"] = "action";
    msg["actionType"] = "view_property";
    std::cout << "[Client] Sending view property request to server" << std::endl;
    sendMessage(msg);
}

void OnlineGameClient::onConnected() {
    connected_ = true;
    heartbeatTimer_->start();
    if (onConnected_) {
        onConnected_();
    }
}

void OnlineGameClient::onDisconnected() {
    connected_ = false;
    heartbeatTimer_->stop();
    if (onDisconnected_) {
        onDisconnected_();
    }
}

void OnlineGameClient::onError(QAbstractSocket::SocketError error) {
    QString errorMsg;
    switch (error) {
        case QAbstractSocket::ConnectionRefusedError:
            errorMsg = "Connection refused - Server may not be running or firewall is blocking";
            break;
        case QAbstractSocket::HostNotFoundError:
            errorMsg = "Host not found - Check server IP address";
            break;
        case QAbstractSocket::NetworkError:
            errorMsg = "Network error - Check network connection and server IP";
            break;
        case QAbstractSocket::SocketTimeoutError:
            errorMsg = "Connection timeout - Server may be unreachable";
            break;
        default:
            errorMsg = QString("WebSocket error (code: %1)").arg(error);
    }
    std::cout << "[OnlineClient] Connection error: " << errorMsg.toStdString() 
              << " (error code: " << error << ")" << std::endl;
    if (onError_) {
        onError_(errorMsg);
    }
}

void OnlineGameClient::onTextMessageReceived(const QString& message) {
    std::cout << "[Client] Received message: " << message.toStdString() << std::endl;
    
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (doc.isNull() || !doc.isObject()) {
        std::cout << "[Client] WARNING: Failed to parse message as JSON object" << std::endl;
        return;
    }
    
    QJsonObject msg = doc.object();
    QString type = msg["type"].toString();
    std::cout << "[Client] Message type: " << type.toStdString() << std::endl;
    
    handleMessage(msg);
}

void OnlineGameClient::sendHeartbeat() {
    QJsonObject msg;
    msg["type"] = "heartbeat";
    sendMessage(msg);
}

void OnlineGameClient::sendMessage(const QJsonObject& msg) {
    if (!isConnected()) {
        std::cout << "[Client] WARNING: Cannot send message, not connected!" << std::endl;
        return;
    }
    
    QJsonDocument doc(msg);
    QString jsonStr = doc.toJson(QJsonDocument::Compact);
    std::cout << "[Client] Sending message: " << jsonStr.toStdString() << std::endl;
    
    websocket_->sendTextMessage(jsonStr);
    std::cout << "[Client] Message sent successfully" << std::endl;
}

void OnlineGameClient::handleMessage(const QJsonObject& msg) {
    QString type = msg["type"].toString();
    std::cout << "[Client] handleMessage: type=" << type.toStdString() << std::endl;
    
    if (type == "joined") {
        handleJoined(msg);
    } else if (type == "room_list") {
        handleRoomList(msg);
    } else if (type == "state_update") {
        handleStateUpdate(msg);
    } else if (type == "event") {
        handleEvent(msg);
    } else if (type == "chat") {
        handleChat(msg);
    } else if (type == "error") {
        handleError(msg);
    } else {
        std::cout << "[Client] WARNING: Unknown message type: " << type.toStdString() << std::endl;
    }
}

void OnlineGameClient::handleJoined(const QJsonObject& msg) {
    QString roomId = msg["roomId"].toString();
    int playerId = msg["playerId"].toInt();
    
    std::cout << "[Client] handleJoined: roomId=" << roomId.toStdString() 
              << ", playerId=" << playerId << std::endl;
    
    currentRoomId_ = roomId;
    myPlayerId_ = playerId;
    
    QJsonObject snapshotObj = msg["snapshot"].toObject();
    
    StateSnapshot snapshot = StateSnapshot::fromJson(snapshotObj);
    
    std::cout << "[Client] handleJoined: snapshot has " << snapshot.players.size() << " players" << std::endl;
    for (const auto& p : snapshot.players) {
        std::cout << "[Client]   Player: " << p.displayName << " (ID: " << p.playerId << ")" << std::endl;
    }
    
    if (onJoinedRoom_) {
        onJoinedRoom_(roomId, playerId, snapshot);
    }
}

void OnlineGameClient::handleRoomList(const QJsonObject& msg) {
    QJsonArray roomsArray = msg["rooms"].toArray();
    std::vector<RoomSummary> rooms;
    rooms.reserve(roomsArray.size());
    
    for (const auto& roomValue : roomsArray) {
        if (roomValue.isObject()) {
            rooms.push_back(RoomSummary::fromJson(roomValue.toObject()));
        }
    }
    
    if (onRoomList_) {
        onRoomList_(rooms);
    }
}

void OnlineGameClient::handleStateUpdate(const QJsonObject& msg) {
    std::cout << "[Client] ===== handleStateUpdate called =====" << std::endl;
    std::cout << "[Client] Message keys: ";
    for (const QString& key : msg.keys()) {
        std::cout << key.toStdString() << " ";
    }
    std::cout << std::endl;
    
    QJsonObject snapshotObj;
    
    // 尝试多种可能的格式
    if (msg.contains("payload") && msg["payload"].isObject()) {
        QJsonObject payload = msg["payload"].toObject();
        if (payload.contains("snapshot")) {
            snapshotObj = payload["snapshot"].toObject();
            std::cout << "[Client] Found snapshot in payload.snapshot" << std::endl;
        }
    }
    
    if (snapshotObj.isEmpty() && msg.contains("snapshot")) {
        snapshotObj = msg["snapshot"].toObject();
        std::cout << "[Client] Found snapshot in root" << std::endl;
    }
    
    if (snapshotObj.isEmpty()) {
        std::cout << "[Client] ERROR: state_update message has no snapshot!" << std::endl;
        std::cout << "[Client] Full message: " << QJsonDocument(msg).toJson(QJsonDocument::Compact).toStdString() << std::endl;
        return;
    }
    
    std::cout << "[Client] Snapshot object keys: ";
    for (const QString& key : snapshotObj.keys()) {
        std::cout << key.toStdString() << " ";
    }
    std::cout << std::endl;
    
    StateSnapshot snapshot = StateSnapshot::fromJson(snapshotObj);
    std::cout << "[Client] Parsed snapshot: phase=" << static_cast<int>(snapshot.phase) 
              << ", players=" << snapshot.players.size() 
              << ", currentTurnIndex=" << snapshot.currentTurnIndex << std::endl;
    
    if (onStateUpdate_) {
        std::cout << "[Client] Calling onStateUpdate callback" << std::endl;
        onStateUpdate_(snapshot);
    } else {
        std::cout << "[Client] ERROR: onStateUpdate_ callback is not set!" << std::endl;
    }
    
    std::cout << "[Client] ===== handleStateUpdate completed =====" << std::endl;
}

void OnlineGameClient::handleEvent(const QJsonObject& msg) {
    std::cout << "[Client] handleEvent: Full message: " << QJsonDocument(msg).toJson(QJsonDocument::Compact).toStdString() << std::endl;
    
    QJsonObject eventObj;
    if (msg.contains("data") && msg["data"].isObject()) {
        eventObj = msg["data"].toObject();
        std::cout << "[Client] Found event in data field" << std::endl;
    } else if (msg.contains("event") && msg["event"].isObject()) {
        eventObj = msg["event"].toObject();
        std::cout << "[Client] Found event in event field" << std::endl;
    } else if (msg.contains("topic")) {
        eventObj = msg;
        std::cout << "[Client] Message itself is an event" << std::endl;
    } else {
        std::cout << "[Client] WARNING: Cannot find event object in message!" << std::endl;
        return;
    }
    
    std::cout << "[Client] Event object: " << QJsonDocument(eventObj).toJson(QJsonDocument::Compact).toStdString() << std::endl;
    
    GameEvent event = GameEvent::fromJson(eventObj);
    std::cout << "[Client] Parsed event: topic=" << event.topic << ", payload keys: ";
    for (const QString& key : event.payload.keys()) {
        std::cout << key.toStdString() << " ";
    }
    std::cout << std::endl;
    
    if (onEvent_) {
        std::cout << "[Client] Calling onEvent callback" << std::endl;
        onEvent_(event);
    } else {
        std::cout << "[Client] ERROR: onEvent_ callback is not set!" << std::endl;
    }
}

void OnlineGameClient::handleChat(const QJsonObject& msg) {
    QJsonObject chatData;
    if (msg.contains("data") && msg["data"].isObject()) {
        chatData = msg["data"].toObject();
        std::cout << "[Client] Received chat message (from broadcast, data field)" << std::endl;
    } else {
        chatData = msg;
        std::cout << "[Client] Received chat message (direct)" << std::endl;
    }
    
    int playerId = chatData["playerId"].toInt();
    QString playerName = chatData["playerName"].toString();
    QString message = chatData["message"].toString();
    bool isPrivate = chatData["isPrivate"].toBool(false);
    int targetPlayerId = chatData["targetPlayerId"].toInt(-1);
    
    std::cout << "[Client] Received chat from player " << playerId 
              << " (" << playerName.toStdString() << "): " 
              << message.toStdString();
    if (isPrivate) {
        std::cout << " (private message";
        if (targetPlayerId == myPlayerId_) {
            std::cout << " to you";
        }
        std::cout << ")";
    }
    std::cout << std::endl;
    
    if (onChat_) {
        onChat_(playerId, playerName, message, isPrivate, targetPlayerId);
    }
}

void OnlineGameClient::handleError(const QJsonObject& msg) {
    QString errorMsg = msg["message"].toString();
    if (errorMsg.isEmpty()) {
        errorMsg = msg["payload"].toObject()["message"].toString();
    }
    
    if (onError_) {
        onError_(errorMsg);
    }
}

} 

