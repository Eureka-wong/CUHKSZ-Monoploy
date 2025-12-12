#pragma once

#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <string>
#include <vector>

namespace monopoly {

// 房间阶段
enum class RoomPhase {
    Created = 0,
    Waiting = 1,
    InGame = 2,
    Paused = 3,
    Ended = 4
};

// 房间设置
struct RoomSettings {
    std::string roomId;
    int maxPlayers{4};
    bool allowSpectators{true};
    bool passwordProtected{false};
    std::string password;
    std::string ruleVariant{"classic"};
    
    static RoomSettings fromJson(const QJsonObject& obj) {
        RoomSettings settings;
        settings.roomId = obj["roomId"].toString().toStdString();
        settings.maxPlayers = obj["maxPlayers"].toInt(4);
        settings.allowSpectators = obj["allowSpectators"].toBool(true);
        settings.passwordProtected = obj["passwordProtected"].toBool(false);
        settings.password = obj["password"].toString().toStdString();
        settings.ruleVariant = obj["ruleVariant"].toString("classic").toStdString();
        return settings;
    }
    
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["roomId"] = QString::fromStdString(roomId);
        obj["maxPlayers"] = maxPlayers;
        obj["allowSpectators"] = allowSpectators;
        obj["passwordProtected"] = passwordProtected;
        obj["password"] = QString::fromStdString(password);
        obj["ruleVariant"] = QString::fromStdString(ruleVariant);
        return obj;
    }
};

// 玩家状态
struct PlayerState {
    int playerId{0};
    std::string displayName;
    int boardIndex{0};
    int cash{1500};
    bool spectator{false};
    bool isAi{false};
    bool ready{false};
    std::vector<std::string> assets;
    int jailStatus{-1};  // -1 = 不在监狱, 0-2 = 在监狱的回合数
    
    static PlayerState fromJson(const QJsonObject& obj) {
        PlayerState player;
        player.playerId = obj["playerId"].toInt(0);
        player.displayName = obj["displayName"].toString().toStdString();
        player.boardIndex = obj["boardIndex"].toInt(0);
        player.cash = obj["cash"].toInt(1500);
        player.spectator = obj["spectator"].toBool(false);
        player.isAi = obj["isAi"].toBool(false);
        player.ready = obj["ready"].toBool(false);
        player.jailStatus = obj["jailStatus"].toInt(-1);  // 解析 jailStatus
        
        QJsonArray assetsArray = obj["assets"].toArray();
        for (const auto& assetValue : assetsArray) {
            player.assets.push_back(assetValue.toString().toStdString());
        }
        return player;
    }
};

// 游戏状态快照
struct StateSnapshot {
    RoomPhase phase{RoomPhase::Created};
    RoomSettings settings;
    std::vector<PlayerState> players;
    uint64_t lastEventSeq{0};
    int currentTurnIndex{-1};
    
    static StateSnapshot fromJson(const QJsonObject& obj) {
        StateSnapshot snapshot;
        snapshot.phase = static_cast<RoomPhase>(obj["phase"].toInt(0));
        snapshot.lastEventSeq = static_cast<uint64_t>(obj["lastEventSeq"].toVariant().toULongLong());
        snapshot.currentTurnIndex = obj["currentTurnIndex"].toInt(-1);
        
        QJsonObject settingsObj = obj["settings"].toObject();
        if (settingsObj.isEmpty()) {
            snapshot.settings.roomId = obj["roomId"].toString().toStdString();
            snapshot.settings.maxPlayers = obj["maxPlayers"].toInt(4);
            snapshot.settings.allowSpectators = obj["allowSpectators"].toBool(true);
            snapshot.settings.passwordProtected = obj["passwordProtected"].toBool(false);
            snapshot.settings.ruleVariant = obj["ruleVariant"].toString("classic").toStdString();
        } else {
            snapshot.settings = RoomSettings::fromJson(settingsObj);
        }
        
        QJsonArray playersArray = obj["players"].toArray();
        snapshot.players.reserve(playersArray.size());
        for (const auto& playerValue : playersArray) {
            if (playerValue.isObject()) {
                snapshot.players.push_back(PlayerState::fromJson(playerValue.toObject()));
            }
        }
        
        return snapshot;
    }
};

// 房间摘要
struct RoomSummary {
    std::string roomId;
    std::string ruleVariant;
    int currentPlayers{0};
    int maxPlayers{0};
    RoomPhase phase{RoomPhase::Created};
    bool allowSpectators{true};
    bool passwordProtected{false};
    
    static RoomSummary fromJson(const QJsonObject& obj) {
        RoomSummary room;
        room.roomId = obj["roomId"].toString().toStdString();
        room.ruleVariant = obj["ruleVariant"].toString("classic").toStdString();
        room.currentPlayers = obj["currentPlayers"].toInt(0);
        room.maxPlayers = obj["maxPlayers"].toInt(4);
        room.phase = static_cast<RoomPhase>(obj["phase"].toInt(0));
        room.allowSpectators = obj["allowSpectators"].toBool(true);
        room.passwordProtected = obj["passwordProtected"].toBool(false);
        return room;
    }
};

// 游戏事件
struct GameEvent {
    uint64_t seq{0};
    std::string topic;
    QJsonObject payload;
    
    static GameEvent fromJson(const QJsonObject& obj) {
        GameEvent event;
        event.seq = static_cast<uint64_t>(obj["seq"].toVariant().toULongLong());
        event.topic = obj["topic"].toString().toStdString();
        event.payload = obj["payload"].toObject();
        return event;
    }
};

}  // namespace monopoly

