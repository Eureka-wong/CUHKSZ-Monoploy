#pragma once

#include <chrono>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "json.hpp"

namespace monopoly {

using json = nlohmann::json;

enum class RoomPhase {
    Created,
    Waiting,
    InGame,
    Paused,
    Ended
};

enum class Reliability {
    Reliable,
    Unreliable
};

enum class PlayerConnectionState {
    Connected,
    Disconnected,
    ReplacedByAI
};

struct RoomSettings {
    std::string roomId;
    int maxPlayers{4};
    bool allowSpectators{true};
    bool passwordProtected{false};
    std::string password;
    std::string ruleVariant{"classic"};
    bool enableAiBackfill{true};
    bool allowObservers{true};
};

struct PlayerState {
    int playerId{0};
    std::string displayName;
    int boardIndex{0};
    int cash{1500};
    bool spectator{false};
    bool isAi{false};
    bool ready{false};
    std::vector<std::string> assets;
};

struct GameEvent {
    uint64_t seq{0};
    Reliability reliability{Reliability::Reliable};
    std::string topic;
    json payload;
    std::chrono::system_clock::time_point timestamp;
};

enum class PlayerActionType {
    RollDice,
    Move,
    BuyProperty,
    SellProperty,
    BuildHouse,
    Trade,
    UseCard,
    EndTurn,
    Chat
};

struct PlayerAction {
    PlayerActionType type{PlayerActionType::RollDice};
    int playerId{0};
    json details;
};

struct StateSnapshot {
    RoomPhase phase{RoomPhase::Created};
    RoomSettings settings;
    std::vector<PlayerState> players;
    uint64_t lastEventSeq{0};
    int currentTurnIndex{-1};
};

struct JoinResult {
    bool success{false};
    std::string message;
    int playerId{0};
    bool spectator{false};
    StateSnapshot snapshot;
};

struct RoomSummary {
    std::string roomId;
    std::string ruleVariant;
    int currentPlayers{0};
    int maxPlayers{0};
    RoomPhase phase{RoomPhase::Created};
    bool allowSpectators{true};
    bool passwordProtected{false};
};

}  // namespace monopoly

