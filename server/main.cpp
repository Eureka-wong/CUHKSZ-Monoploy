#include "include/monopoly/core_types.h"
#include "include/json.hpp"

#include <uwebsockets/App.h>
#include <iostream>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <random>
#include <deque>
#include <string>

using namespace monopoly;
using json = nlohmann::json;

// PerSocketData for uWebSockets
struct PerSocketData {
    int playerId{0};
};

// property information structure
struct PropertyInfo {
    int position;
    std::string group;
    std::array<int, 6> rent;
    std::vector<int> groupProperties;
    

    PropertyInfo() : position(0), group(""), rent({0, 0, 0, 0, 0, 0}) {}
    
    PropertyInfo(int pos, const std::string& grp, const std::array<int, 6>& r, const std::vector<int>& groupProps)
        : position(pos), group(grp), rent(r), groupProperties(groupProps) {}
};

std::unordered_map<int, PropertyInfo> initializePropertyInfo() {
    std::unordered_map<int, PropertyInfo> propertyMap;
    
    // Brown (1, 3)
    propertyMap[1] = PropertyInfo(1, "Brown", {2, 10, 30, 90, 160, 250}, {1, 3});
    propertyMap[3] = PropertyInfo(3, "Brown", {4, 20, 60, 180, 320, 450}, {1, 3});
    
    // Station (5, 15, 25, 35)
    propertyMap[5] = PropertyInfo(5, "Station", {25, 50, 100, 200, 0, 0}, {5, 15, 25, 35});
    propertyMap[15] = PropertyInfo(15, "Station", {25, 50, 100, 200, 0, 0}, {5, 15, 25, 35});
    propertyMap[25] = PropertyInfo(25, "Station", {25, 50, 100, 200, 0, 0}, {5, 15, 25, 35});
    propertyMap[35] = PropertyInfo(35, "Station", {25, 50, 100, 200, 0, 0}, {5, 15, 25, 35});
    
    // Light Blue (6, 8, 9)
    propertyMap[6] = PropertyInfo(6, "Light Blue", {6, 30, 90, 270, 400, 550}, {6, 8, 9});
    propertyMap[8] = PropertyInfo(8, "Light Blue", {6, 30, 90, 270, 400, 550}, {6, 8, 9});
    propertyMap[9] = PropertyInfo(9, "Light Blue", {8, 40, 100, 300, 450, 600}, {6, 8, 9});
    
    // Utility (2, 28)
    propertyMap[12] = PropertyInfo(12, "Utility", {0, 0, 0, 0, 0, 0}, {12, 28});
    propertyMap[28] = PropertyInfo(28, "Utility", {0, 0, 0, 0, 0, 0}, {12, 28});
    
    // Pink (11, 13, 14)
    propertyMap[11] = PropertyInfo(11, "Pink", {10, 50, 150, 450, 625, 750}, {11, 13, 14});
    propertyMap[13] = PropertyInfo(13, "Pink", {10, 50, 150, 450, 625, 750}, {11, 13, 14});
    propertyMap[14] = PropertyInfo(14, "Pink", {12, 60, 180, 500, 700, 900}, {11, 13, 14});
    
    // Orange (16, 18, 19)
    propertyMap[16] = PropertyInfo(16, "Orange", {14, 70, 200, 550, 750, 950}, {16, 18, 19});
    propertyMap[18] = PropertyInfo(18, "Orange", {14, 70, 200, 550, 750, 950}, {16, 18, 19});
    propertyMap[19] = PropertyInfo(19, "Orange", {16, 80, 220, 600, 800, 1000}, {16, 18, 19});
    
    // Red (21, 23, 24)
    propertyMap[21] = PropertyInfo(21, "Red", {18, 90, 250, 700, 875, 1050}, {21, 23, 24});
    propertyMap[23] = PropertyInfo(23, "Red", {18, 90, 250, 700, 875, 1050}, {21, 23, 24});
    propertyMap[24] = PropertyInfo(24, "Red", {20, 100, 300, 750, 925, 1100}, {21, 23, 24});
    
    // Yellow (26, 27, 29)
    propertyMap[26] = PropertyInfo(26, "Yellow", {22, 110, 330, 800, 975, 1150}, {26, 27, 29});
    propertyMap[27] = PropertyInfo(27, "Yellow", {22, 110, 330, 800, 975, 1150}, {26, 27, 29});
    propertyMap[29] = PropertyInfo(29, "Yellow", {24, 120, 360, 850, 1025, 1200}, {26, 27, 29});
    
    // Green (31, 32, 34)
    propertyMap[31] = PropertyInfo(31, "Green", {26, 130, 390, 900, 1100, 1275}, {31, 32, 34});
    propertyMap[32] = PropertyInfo(32, "Green", {26, 130, 390, 900, 1100, 1275}, {31, 32, 34});
    propertyMap[34] = PropertyInfo(34, "Green", {28, 150, 450, 1000, 1200, 1400}, {31, 32, 34});
    
    // Dark Blue (37, 39)
    propertyMap[37] = PropertyInfo(37, "Dark Blue", {35, 175, 500, 1100, 1300, 1500}, {37, 39});
    propertyMap[39] = PropertyInfo(39, "Dark Blue", {50, 200, 600, 1400, 1700, 2000}, {37, 39});
    
    return propertyMap;
}

static const std::unordered_map<int, PropertyInfo> PROPERTY_INFO = initializePropertyInfo();

int calculateRent(int propertyPosition, int ownerPlayerId, const std::unordered_map<int, PlayerState>& allPlayers, int diceTotal = 0) {
    auto propIt = PROPERTY_INFO.find(propertyPosition);
    if (propIt == PROPERTY_INFO.end()) {
        return 0;
    }
    
    const PropertyInfo& propInfo = propIt->second;
    auto ownerIt = allPlayers.find(ownerPlayerId);
    if (ownerIt == allPlayers.end()) {
        return 0;
    }
    const PlayerState& owner = ownerIt->second;
    
    bool ownsColorGroup = true;
    for (int groupPropPos : propInfo.groupProperties) {
        bool ownsThisProp = false;
        for (const std::string& assetStr : owner.assets) {
            try {
                int assetPos = std::stoi(assetStr);
                if (assetPos == groupPropPos) {
                    ownsThisProp = true;
                    break;
                }
            } catch (...) {
            }
        }
        if (!ownsThisProp) {
            ownsColorGroup = false;
            break;
        }
    }
    
    // rent methods
    if (propInfo.group == "Station") {
        int ownedStations = 0;
        for (int stationPos : propInfo.groupProperties) {
            for (const std::string& assetStr : owner.assets) {
                try {
                    int assetPos = std::stoi(assetStr);
                    if (assetPos == stationPos) {
                        ownedStations++;
                        break;
                    }
                } catch (...) {
                }
            }
        }
        if (ownedStations >= 1 && ownedStations <= 4) {
            return propInfo.rent[ownedStations - 1];
        }
        return propInfo.rent[0];
    } else if (propInfo.group == "Utility") {
        int ownedUtilities = 0;
        for (int utilPos : propInfo.groupProperties) {
            for (const std::string& assetStr : owner.assets) {
                try {
                    int assetPos = std::stoi(assetStr);
                    if (assetPos == utilPos) {
                        ownedUtilities++;
                        break;
                    }
                } catch (...) {
                }
            }
        }
        if (diceTotal > 0) {
            return (ownedUtilities == 1) ? diceTotal * 4 : diceTotal * 10;
        }
        return 0;
    } else {
        if (ownsColorGroup) {
            return propInfo.rent[0] * 2;
        } else {
            return propInfo.rent[0];
        }
    }
}

struct SimpleRoom {
    std::string roomId;
    RoomSettings settings;
    RoomPhase phase{RoomPhase::Created};
    std::unordered_map<int, PlayerState> players;
    std::unordered_map<int, int> playerJailStatus;
    int nextPlayerId{1};
    int currentTurnIndex{-1};
    mutable std::mutex mutex; 
    
    StateSnapshot getSnapshot() const {
        std::lock_guard<std::mutex> lock(mutex);
        StateSnapshot snap;
        snap.phase = phase;
        snap.settings = settings;
        snap.currentTurnIndex = currentTurnIndex;
        snap.lastEventSeq = 0;
        
        for (const auto& [id, player] : players) {
            snap.players.push_back(player);
        }
        std::sort(snap.players.begin(), snap.players.end(),
                  [](const PlayerState& a, const PlayerState& b) {
                      return a.playerId < b.playerId;
                  });
        return snap;
    }
};

enum class CardType {
    MOVE_TO,
    MOVE,
    MOVE_TO_NEAREST,
    COLLECT_MONEY,
    PAY_MONEY,
    COLLECT_FROM_PLAYERS,
    PAY_TO_PLAYERS,
    REPAIRS,
    GET_OUT_OF_JAIL,
    GO_TO_JAIL
};

struct Card {
    std::string description;
    CardType type;
    int amount{0};
    int targetTile{-1};
    int step{0};
    std::string nearest{""};
    int houseCost{0};
    int hotelCost{0};
    std::string sourceDeck{""};
    
    Card(const std::string& desc, CardType t, int amt = 0, int target = -1, int stp = 0,
         const std::string& near = "", int hCost = 0, int hoCost = 0, const std::string& deck = "")
        : description(desc), type(t), amount(amt), targetTile(target), step(stp),
          nearest(near), houseCost(hCost), hotelCost(hoCost), sourceDeck(deck) {}
};

class CardDeck {
private:
    std::deque<Card> deck;
    std::mt19937 rng{std::random_device{}()};
    
public:
    CardDeck() : rng(std::random_device{}()) {
    }
    
    CardDeck(const std::string& deckType) {
        std::vector<Card> cards;
        
        if (deckType == "Community Chest") {
            cards = {
                Card("Advance to Go", CardType::MOVE_TO, 0, 0),
                Card("Bank error in your favor. Collect $200.", CardType::COLLECT_MONEY, 200),
                Card("Doctor's fees. Pay $50.", CardType::PAY_MONEY, 50),
                Card("Get $50 from sale of stock", CardType::COLLECT_MONEY, 50),
                Card("Get Out of Jail Free. This card may be kept until needed or sold.", CardType::GET_OUT_OF_JAIL, 0, -1, 0, "", 0, 0, "Community Chest"),
                Card("Go to Jail. Go directly to Jail. Do not pass Go, do not collect $200.", CardType::GO_TO_JAIL),
                Card("Grand Opera Night. Collect $50 from every player for opening night seats.", CardType::COLLECT_FROM_PLAYERS, 50),
                Card("Holiday Fund matures. Receive $100.", CardType::COLLECT_MONEY, 100),
                Card("Income tax refund. Collect $20.", CardType::COLLECT_MONEY, 20),
                Card("It's your birthday! Collect $10 from each player.", CardType::COLLECT_FROM_PLAYERS, 10),
                Card("Life insurance matures. Collect $100.", CardType::COLLECT_MONEY, 100),
                Card("Pay hospital fees of $100.", CardType::PAY_MONEY, 100),
                Card("Pay school fees of $150.", CardType::PAY_MONEY, 150),
                Card("Receive $25 for services.", CardType::COLLECT_MONEY, 25),
                Card("Street repairs: Pay $40 per house and $115 per hotel you own.", CardType::REPAIRS, 0, -1, 0, "", 40, 115),
                Card("You have won second prize in a beauty contest. Collect $10.", CardType::COLLECT_MONEY, 10),
            };
        } else if (deckType == "Chance") {
            cards = {
                Card("Advance to Go", CardType::MOVE_TO, 0, 0),
                Card("Advance to 24. If you pass Go, collect $200.", CardType::MOVE_TO, 0, 24),
                Card("Advance to 11. If you pass Go, collect $200.", CardType::MOVE_TO, 0, 11),
                Card("Advance to the nearest Station.", CardType::MOVE_TO_NEAREST, 0, -1, 0, "Station"),
                Card("Advance to the nearest Utility.", CardType::MOVE_TO_NEAREST, 0, -1, 0, "Utility"),
                Card("Advance to 39.", CardType::MOVE_TO, 0, 39),
                Card("Advance to 5. If you pass Go, collect $200.", CardType::MOVE_TO, 0, 5),
                Card("Bank pays you dividend of $50.", CardType::COLLECT_MONEY, 50),
                Card("Get Out of Jail Free. This card may be kept until needed or sold.", CardType::GET_OUT_OF_JAIL, 0, -1, 0, "", 0, 0, "Chance"),
                Card("Go Back 3 Spaces.", CardType::MOVE, 0, -1, -3),
                Card("Go to Jail. Go directly to Jail. Do not pass Go, do not collect $200.", CardType::GO_TO_JAIL),
                Card("Pay poor tax of $15.", CardType::PAY_MONEY, 15),
                Card("Take a trip to Reading Railroad. If you pass Go, collect $200.", CardType::MOVE_TO, 0, 5),
                Card("Take a walk on the Boardwalk. Advance to Boardwalk.", CardType::MOVE_TO, 0, 39),
                Card("You have been elected Chairman of the Board. Pay each player $50.", CardType::PAY_TO_PLAYERS, 50),
                Card("Your building loan matures. Collect $150.", CardType::COLLECT_MONEY, 150),
            };
        }
        
        std::shuffle(cards.begin(), cards.end(), rng);
        deck = std::deque<Card>(cards.begin(), cards.end());
    }
    
    Card draw() {
        if (deck.empty()) {
            return Card("", CardType::COLLECT_MONEY, 0);
        }
        Card card = deck.front();
        deck.pop_front();
        return card;
    }
    
    void returnCard(const Card& card) {
        deck.push_back(card);
    }
};

// global server state
struct ServerState {
    std::mutex mutex;
    std::unordered_map<std::string, std::shared_ptr<SimpleRoom>> rooms;
    std::unordered_map<int, std::string> playerToRoom;  // playerId -> roomId
    std::unordered_map<int, uWS::WebSocket<false, true, PerSocketData>*> connections;  // playerId -> WebSocket
    int nextConnectionId{1};
    
    // card decks
    std::unordered_map<std::string, CardDeck> chanceDecks;
    std::unordered_map<std::string, CardDeck> communityChestDecks;
    
    std::shared_ptr<SimpleRoom> findOrCreateRoom(const std::string& roomId) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = rooms.find(roomId);
        if (it != rooms.end()) {
            return it->second;
        }
        // new room
        auto room = std::make_shared<SimpleRoom>();
        room->roomId = roomId;
        room->settings.roomId = roomId;
        room->settings.maxPlayers = 4;
        rooms[roomId] = room;
        std::cout << "[Server] Created room: " << roomId << std::endl;
        return room;
    }
    
    void broadcastToRoom(const std::string& roomId, const std::string& type, const json& data) {
        std::shared_ptr<SimpleRoom> room;
        {
            std::lock_guard<std::mutex> lock(mutex);
            auto roomIt = rooms.find(roomId);
            if (roomIt == rooms.end()) return;
            room = roomIt->second;
        }
        
        json message;
        message["type"] = type;
        message["data"] = data;
        std::string msgStr = message.dump();
        
        std::cout << "[Server] Broadcasting " << type << " to room " << roomId << std::endl;
        

        std::lock_guard<std::mutex> globalLock(mutex);
        std::lock_guard<std::mutex> roomLock(room->mutex);
        for (const auto& [playerId, player] : room->players) {
            auto connIt = connections.find(playerId);
            if (connIt != connections.end() && connIt->second) {
                try {
                    connIt->second->send(msgStr, uWS::OpCode::TEXT);
                    std::cout << "[Server] Sent to player " << playerId << std::endl;
                } catch (...) {
                    std::cerr << "[Server] Failed to send to player " << playerId << std::endl;
                }
            }
        }
    }
    
    void broadcastStateUpdate(const std::string& roomId) {
        std::shared_ptr<SimpleRoom> room;
        {
            std::lock_guard<std::mutex> lock(mutex);
            auto roomIt = rooms.find(roomId);
            if (roomIt == rooms.end()) return;
            room = roomIt->second;
        }
        
        auto snap = room->getSnapshot();
        json snapshotJson;
        snapshotJson["phase"] = static_cast<int>(snap.phase);
        snapshotJson["currentTurnIndex"] = snap.currentTurnIndex;
        snapshotJson["lastEventSeq"] = snap.lastEventSeq;
        
        // Settings
        json settingsJson;
        settingsJson["roomId"] = snap.settings.roomId;
        settingsJson["maxPlayers"] = snap.settings.maxPlayers;
        settingsJson["allowSpectators"] = snap.settings.allowSpectators;
        settingsJson["passwordProtected"] = snap.settings.passwordProtected;
        settingsJson["ruleVariant"] = snap.settings.ruleVariant;
        settingsJson["enableAiBackfill"] = snap.settings.enableAiBackfill;
        snapshotJson["settings"] = settingsJson;
        
        // Players
        snapshotJson["players"] = json::array();
        {
            std::lock_guard<std::mutex> roomLock(room->mutex);
            for (const auto& p : snap.players) {
                json playerJson;
                playerJson["playerId"] = p.playerId;
                playerJson["displayName"] = p.displayName;
                playerJson["boardIndex"] = p.boardIndex;
                playerJson["cash"] = p.cash;
                playerJson["spectator"] = p.spectator;
                playerJson["isAi"] = p.isAi;
                playerJson["assets"] = p.assets;
                auto jailIt = room->playerJailStatus.find(p.playerId);
                if (jailIt != room->playerJailStatus.end()) {
                    playerJson["jailStatus"] = jailIt->second;
                } else {
                    playerJson["jailStatus"] = -1;
                }
                snapshotJson["players"].push_back(playerJson);
            }
        }
        
        json message;
        message["type"] = "state_update";
        message["snapshot"] = snapshotJson;
        std::string msgStr = message.dump();
        
        std::lock_guard<std::mutex> globalLock(mutex);
        std::lock_guard<std::mutex> roomLock(room->mutex);
        for (const auto& [playerId, player] : room->players) {
            auto connIt = connections.find(playerId);
            if (connIt != connections.end() && connIt->second) {
                try {
                    connIt->second->send(msgStr, uWS::OpCode::TEXT);
                } catch (...) {
                    std::cerr << "[Server] Failed to send state update to player " << playerId << std::endl;
                }
            }
        }
    }
};

ServerState g_serverState;

int main() {
    std::cout << "=== Monopoly Online Server ===" << std::endl;
    std::cout << "Starting WebSocket server on port 9002..." << std::endl;
    
    uWS::App()
        .ws<PerSocketData>("/*", {
            .compression = uWS::DISABLED,
            .maxPayloadLength = 16 * 1024 * 1024,
            .idleTimeout = 120,
            .maxBackpressure = 1 * 1024 * 1024,
            
            .open = [](auto* ws) {
                std::lock_guard<std::mutex> lock(g_serverState.mutex);
                int playerId = g_serverState.nextConnectionId++;
                auto* data = static_cast<PerSocketData*>(ws->getUserData());
                data->playerId = playerId;
                g_serverState.connections[playerId] = ws;
                std::cout << "[Server] Player #" << playerId << " connected" << std::endl;
            },
            
            .message = [](auto* ws, std::string_view message, uWS::OpCode opCode) {
                auto* data = static_cast<PerSocketData*>(ws->getUserData());
                int playerId = data->playerId;
                
                try {
                    json msg = json::parse(message);
                    std::string type = msg.value("type", "");
                    
                    std::cout << "[Server] ===== Received from player " << playerId << ": " << type << " =====" << std::endl;
                    if (type == "action") {
                        std::string actionType = msg.value("actionType", "");
                        std::cout << "[Server] ===== Action type: " << actionType << " =====" << std::endl;
                    }
                    std::cout << "[Server] Full message: " << msg.dump(2) << std::endl;
                    
                    if (type == "join_room") {
                        std::string roomId = msg.value("roomId", "");
                        std::string playerName = msg.value("playerName", "");
                        bool spectator = msg.value("spectator", false);
                        
                        auto room = g_serverState.findOrCreateRoom(roomId);
                        
                        {
                            std::lock_guard<std::mutex> lock(room->mutex);
                            
                            // full check
                            if (room->players.size() >= static_cast<size_t>(room->settings.maxPlayers)) {
                                json error;
                                error["type"] = "error";
                                error["message"] = "Room is full";
                                ws->send(error.dump(), uWS::OpCode::TEXT);
                                return;
                            }
                            
                            // add player
                            PlayerState player;
                            player.playerId = playerId;
                            player.displayName = playerName;
                            player.boardIndex = 0;
                            player.cash = 1500;
                            player.spectator = spectator;
                            player.isAi = false;
                            room->players[playerId] = player;
                        }
                        
                        {
                            std::lock_guard<std::mutex> lock(g_serverState.mutex);
                            g_serverState.playerToRoom[playerId] = roomId;
                        }
                        
                        std::cout << "[Server] Player " << playerId << " (" << playerName 
                                  << ") joined room " << roomId << std::endl;
                        
                        auto snap = room->getSnapshot();
                        json snapshotJson;
                        snapshotJson["phase"] = static_cast<int>(snap.phase);
                        snapshotJson["currentTurnIndex"] = snap.currentTurnIndex;
                        snapshotJson["lastEventSeq"] = snap.lastEventSeq;
                        
                        json settingsJson;
                        settingsJson["roomId"] = snap.settings.roomId;
                        settingsJson["maxPlayers"] = snap.settings.maxPlayers;
                        settingsJson["allowSpectators"] = snap.settings.allowSpectators;
                        settingsJson["passwordProtected"] = snap.settings.passwordProtected;
                        settingsJson["ruleVariant"] = snap.settings.ruleVariant;
                        settingsJson["enableAiBackfill"] = snap.settings.enableAiBackfill;
                        snapshotJson["settings"] = settingsJson;
                        
                        snapshotJson["players"] = json::array();
                        for (const auto& p : snap.players) {
                            json playerJson;
                            playerJson["playerId"] = p.playerId;
                            playerJson["displayName"] = p.displayName;
                            playerJson["boardIndex"] = p.boardIndex;
                            playerJson["cash"] = p.cash;
                            playerJson["spectator"] = p.spectator;
                            playerJson["isAi"] = p.isAi;
                            playerJson["assets"] = p.assets;
                            auto jailIt = room->playerJailStatus.find(p.playerId);
                            if (jailIt != room->playerJailStatus.end()) {
                                playerJson["jailStatus"] = jailIt->second;
                            } else {
                                playerJson["jailStatus"] = -1; 
                            }
                            snapshotJson["players"].push_back(playerJson);
                        }
                        
                        json joinedMsg;
                        joinedMsg["type"] = "joined";
                        joinedMsg["roomId"] = roomId;
                        joinedMsg["playerId"] = playerId;
                        joinedMsg["snapshot"] = snapshotJson;
                        ws->send(joinedMsg.dump(), uWS::OpCode::TEXT);
                        
                        g_serverState.broadcastStateUpdate(roomId);
                        
                    } else if (type == "list_rooms") {
                        std::lock_guard<std::mutex> lock(g_serverState.mutex);
                        json roomsArray = json::array();
                        for (const auto& [roomId, room] : g_serverState.rooms) {
                            auto snap = room->getSnapshot();
                            json roomJson;
                            roomJson["roomId"] = roomId;
                            roomJson["currentPlayers"] = static_cast<int>(room->players.size());
                            roomJson["maxPlayers"] = room->settings.maxPlayers;
                            roomJson["phase"] = static_cast<int>(snap.phase);
                            roomsArray.push_back(roomJson);
                        }
                        
                        json response;
                        response["type"] = "room_list";
                        response["rooms"] = roomsArray;
                        ws->send(response.dump(), uWS::OpCode::TEXT);
                        
                    } else if (type == "action") {
                        std::string actionType = msg.value("actionType", "");
                        std::string roomId;
                        std::shared_ptr<SimpleRoom> room;
                        {
                            std::lock_guard<std::mutex> lock(g_serverState.mutex);
                            auto roomIt = g_serverState.playerToRoom.find(playerId);
                            if (roomIt == g_serverState.playerToRoom.end()) {
                                json error;
                                error["type"] = "error";
                                error["message"] = "Player not in any room";
                                ws->send(error.dump(), uWS::OpCode::TEXT);
                                return;
                            }
                            roomId = roomIt->second;
                            room = g_serverState.rooms[roomId];
                        }
                        
                        bool needBroadcastEvent = false;
                        json event;
                        {
                            std::lock_guard<std::mutex> lock(room->mutex);
                            
                            auto playerIt = room->players.find(playerId);
                            if (playerIt == room->players.end()) {
                                json error;
                                error["type"] = "error";
                                error["message"] = "Player not found in room";
                                ws->send(error.dump(), uWS::OpCode::TEXT);
                                return;
                            }
                            
                            PlayerState& player = playerIt->second;
                            
                            if (actionType == "start_game") {
                                if (room->phase == RoomPhase::Created || room->phase == RoomPhase::Waiting) {
                                    if (room->players.size() >= 2) {
                                        room->phase = RoomPhase::InGame;
                                        room->currentTurnIndex = 0;
                                        std::cout << "[Server] Game started in room " << room->roomId << std::endl;
                                        needBroadcastEvent = true;
                                        event["type"] = "event";
                                        event["topic"] = "room.game_started";
                                        event["payload"] = {{"playerId", playerId}};
                                    }
                                }
                            
                            } else if (actionType == "roll") {
                                if (room->phase != RoomPhase::InGame) {
                                    json error;
                                    error["type"] = "error";
                                    error["message"] = "Game not started";
                                    ws->send(error.dump(), uWS::OpCode::TEXT);
                                    return;
                                }
                                
                                std::vector<PlayerState> playersList;
                                for (const auto& [id, p] : room->players) {
                                    playersList.push_back(p);
                                }
                                std::sort(playersList.begin(), playersList.end(),
                                          [](const PlayerState& a, const PlayerState& b) {
                                              return a.playerId < b.playerId;
                                          });
                                
                                if (room->currentTurnIndex >= 0 && 
                                    room->currentTurnIndex < static_cast<int>(playersList.size()) &&
                                    playersList[room->currentTurnIndex].playerId == playerId) {
                                    
                                    bool isInJail = false;
                                    int jailStatus = -1;
                                    auto jailIt = room->playerJailStatus.find(playerId);
                                    if (jailIt != room->playerJailStatus.end() && jailIt->second >= 0) {
                                        isInJail = true;
                                        jailStatus = jailIt->second;
                                        std::cout << "[Server] Player " << playerId << " is in jail (turn " << (jailStatus + 1) << ")" << std::endl;
                                    }
                                    
                                    std::random_device rd;
                                    std::mt19937 gen(rd());
                                    std::uniform_int_distribution<> dis(1, 6);
                                    int dice1 = dis(gen);
                                    int dice2 = dis(gen);
                                    int total = dice1 + dice2;
                                    bool isDouble = (dice1 == dice2);
                                    
                                    bool gotOutOfJail = false;
                                    if (isInJail) {
                                        if (isDouble) {
                                            room->playerJailStatus.erase(jailIt);
                                            gotOutOfJail = true;
                                            std::cout << "[Server] ===== Player " << playerId 
                                                      << " rolled double (" << dice1 << "," << dice2 
                                                      << ") and got out of jail! =====" << std::endl;
                                        } else {
                                            std::cout << "[Server] Player " << playerId 
                                                      << " rolled " << dice1 << "+" << dice2 << "=" << total 
                                                      << " (not double), stays in jail" << std::endl;
                                            
                                            needBroadcastEvent = true;
                                            event["type"] = "event";
                                            event["topic"] = "game.jail_roll";
                                            event["payload"] = {
                                                {"playerId", playerId},
                                                {"dice1", dice1},
                                                {"dice2", dice2},
                                                {"total", total},
                                                {"isDouble", false},
                                                {"gotOutOfJail", false},
                                                {"jailStatus", jailStatus},
                                                {"jailTurnsRemaining", 3 - jailStatus} 
                                            };
                                            return;
                                        }
                                    }
                                    
                                    int oldPosition = player.boardIndex;
                                    if (!isInJail || gotOutOfJail) {
                                        player.boardIndex = (player.boardIndex + total) % 40;
                                    }
                                    
                                    std::cout << "[Server] Player " << playerId << " rolled " 
                                              << dice1 << "+" << dice2 << "=" << total 
                                              << (isDouble ? " (DOUBLE)" : "")
                                              << ", moved from " << oldPosition << " to " << player.boardIndex << std::endl;
                                    
                                    // bonus for passing Go
                                    bool passedGo = false;
                                    if (oldPosition > 0 && oldPosition + total >= 40) {
                                        player.cash += 200;
                                        passedGo = true;
                                        std::cout << "[Server] ===== Player " << playerId 
                                                  << " passed Go! Received $200, new cash: $" << player.cash << " =====" << std::endl;
                                    }
                                    
                                    //Go to Jail 
                                    bool wentToJail = false;
                                    if (player.boardIndex == 30) {
                                        player.boardIndex = 10;
                                        room->playerJailStatus[playerId] = 0;
                                        wentToJail = true;
                                        std::cout << "[Server] ===== Player " << playerId << " went to jail (position 10) =====" << std::endl;
                                    } else {
                                        std::cout << "[Server] Player " << playerId << " landed on position " << player.boardIndex << " (not jail)" << std::endl;
                                    }
                                    
                                    // Tax
                                    bool paidTax = false;
                                    int taxAmount = 0;
                                    if (player.boardIndex == 4) {
                                        // Income Tax: $200
                                        taxAmount = 200;
                                        paidTax = true;
                                        std::cout << "[Server] ===== Player " << playerId << " landed on Income Tax (position 4) =====" << std::endl;
                                    } else if (player.boardIndex == 38) {
                                        // Luxury Tax: $100
                                        taxAmount = 100;
                                        paidTax = true;
                                        std::cout << "[Server] ===== Player " << playerId << " landed on Luxury Tax (position 38) =====" << std::endl;
                                    }
                                    
                                    if (paidTax) {
                                        if (player.cash >= taxAmount) {
                                            player.cash -= taxAmount;
                                            std::cout << "[Server] ===== Player " << playerId 
                                                      << " paid tax $" << taxAmount 
                                                      << " at position " << player.boardIndex 
                                                      << ", remaining cash: $" << player.cash << " =====" << std::endl;
                                        } else {
                                            std::cout << "[Server] ===== Player " << playerId 
                                                      << " cannot pay tax $" << taxAmount 
                                                      << " (has $" << player.cash << ") =====" << std::endl;
                                        }
                                    }
                                    
                                    bool drewCard = false;
                                    std::string cardType = "";
                                    std::string cardDescription = "";
                                    int cardAmount = 0;
                                    int cardTargetTile = -1;
                                    int cardStep = 0;
                                    bool cardGetOutOfJail = false;
                                    bool cardGoToJail = false;
                                    
                                    if (player.boardIndex == 7 || player.boardIndex == 22 || player.boardIndex == 36) {
                                        // Chance
                                        {
                                            std::lock_guard<std::mutex> lock(g_serverState.mutex);
                                            if (g_serverState.chanceDecks.find(roomId) == g_serverState.chanceDecks.end()) {
                                                g_serverState.chanceDecks[roomId] = CardDeck("Chance");
                                            }
                                            Card card = g_serverState.chanceDecks[roomId].draw();
                                            cardType = "Chance";
                                            cardDescription = card.description;
                                            drewCard = true;
                                            
                                            std::cout << "[Server] ===== Player " << playerId << " drew Chance card: " << cardDescription << " =====" << std::endl;
                                            
                                            if (card.type == CardType::MOVE_TO) {
                                                int oldPos = player.boardIndex;
                                                player.boardIndex = card.targetTile;
                                                cardTargetTile = card.targetTile;
                                                if (oldPos > 0 && oldPos + (player.boardIndex - oldPos + 40) % 40 >= 40) {
                                                    player.cash += 200;
                                                    std::cout << "[Server] Player " << playerId << " passed Go, received $200" << std::endl;
                                                }
                                            } else if (card.type == CardType::MOVE) {
                                                player.boardIndex = (player.boardIndex + card.step + 40) % 40;
                                                cardStep = card.step;
                                            } else if (card.type == CardType::MOVE_TO_NEAREST) {
                                                int oldPos = player.boardIndex;
                                                if (card.nearest == "Station") {
                                                    std::array<int, 4> stations = {5, 15, 25, 35};
                                                    for (int station : stations) {
                                                        if (station > oldPos || (oldPos > 35 && station == 5)) {
                                                            int steps = (station - oldPos + 40) % 40;
                                                            player.boardIndex = station;
                                                            cardTargetTile = station;
                                                            if (oldPos > 0 && oldPos + steps >= 40) {
                                                                player.cash += 200;
                                                                std::cout << "[Server] Player " << playerId << " passed Go, received $200" << std::endl;
                                                            }
                                                            break;
                                                        }
                                                    }
                                                } else if (card.nearest == "Utility") {
                                                    std::array<int, 2> utilities = {12, 28};
                                                    for (int utility : utilities) {
                                                        if (utility > oldPos || (oldPos > 28 && utility == 12)) {
                                                            int steps = (utility - oldPos + 40) % 40;
                                                            player.boardIndex = utility;
                                                            cardTargetTile = utility;
                                                            if (oldPos > 0 && oldPos + steps >= 40) {
                                                                player.cash += 200;
                                                                std::cout << "[Server] Player " << playerId << " passed Go, received $200" << std::endl;
                                                            }
                                                            break;
                                                        }
                                                    }
                                                }
                                            } else if (card.type == CardType::COLLECT_MONEY) {
                                                player.cash += card.amount;
                                                cardAmount = card.amount;
                                            } else if (card.type == CardType::PAY_MONEY) {
                                                if (player.cash >= card.amount) {
                                                    player.cash -= card.amount;
                                                    cardAmount = -card.amount;
                                                }
                                            } else if (card.type == CardType::COLLECT_FROM_PLAYERS) {
                                                int totalCollected = 0;
                                                for (auto& [otherId, otherPlayer] : room->players) {
                                                    if (otherId != playerId && otherPlayer.cash >= card.amount) {
                                                        otherPlayer.cash -= card.amount;
                                                        totalCollected += card.amount;
                                                    }
                                                }
                                                player.cash += totalCollected;
                                                cardAmount = totalCollected;
                                            } else if (card.type == CardType::PAY_TO_PLAYERS) {
                                                int numOtherPlayers = 0;
                                                for (auto& [otherId, otherPlayer] : room->players) {
                                                    if (otherId != playerId) numOtherPlayers++;
                                                }
                                                int perPlayerAmount = card.amount;
                                                int totalPaid = 0;
                                                if (player.cash >= perPlayerAmount * numOtherPlayers) {
                                                    for (auto& [otherId, otherPlayer] : room->players) {
                                                        if (otherId != playerId) {
                                                            player.cash -= perPlayerAmount;
                                                            otherPlayer.cash += perPlayerAmount;
                                                            totalPaid += perPlayerAmount;
                                                        }
                                                    }
                                                    cardAmount = -totalPaid;
                                                }
                                            } else if (card.type == CardType::GET_OUT_OF_JAIL) {
                                                cardGetOutOfJail = true;
                                            } else if (card.type == CardType::GO_TO_JAIL) {
                                                player.boardIndex = 10;
                                                room->playerJailStatus[playerId] = 0;
                                                cardGoToJail = true;
                                                wentToJail = true;
                                            }
                                            
                                            if (card.type != CardType::GET_OUT_OF_JAIL) {
                                                g_serverState.chanceDecks[roomId].returnCard(card);
                                            }
                                        }
                                    } else if (player.boardIndex == 2 || player.boardIndex == 17 || player.boardIndex == 33) {
                                        // Community Chest
                                        {
                                            std::lock_guard<std::mutex> lock(g_serverState.mutex);
                                            if (g_serverState.communityChestDecks.find(roomId) == g_serverState.communityChestDecks.end()) {
                                                g_serverState.communityChestDecks[roomId] = CardDeck("Community Chest");
                                            }
                                            Card card = g_serverState.communityChestDecks[roomId].draw();
                                            cardType = "Community Chest";
                                            cardDescription = card.description;
                                            drewCard = true;
                                            
                                            std::cout << "[Server] ===== Player " << playerId << " drew Community Chest card: " << cardDescription << " =====" << std::endl;

                                            if (card.type == CardType::MOVE_TO) {
                                                int oldPos = player.boardIndex;
                                                player.boardIndex = card.targetTile;
                                                cardTargetTile = card.targetTile;
                                                if (oldPos > 0 && oldPos + (player.boardIndex - oldPos + 40) % 40 >= 40) {
                                                    player.cash += 200;
                                                }
                                            } else if (card.type == CardType::MOVE) {
                                                player.boardIndex = (player.boardIndex + card.step + 40) % 40;
                                                cardStep = card.step;
                                            } else if (card.type == CardType::COLLECT_MONEY) {
                                                player.cash += card.amount;
                                                cardAmount = card.amount;
                                            } else if (card.type == CardType::PAY_MONEY) {
                                                if (player.cash >= card.amount) {
                                                    player.cash -= card.amount;
                                                    cardAmount = -card.amount;
                                                }
                                            } else if (card.type == CardType::COLLECT_FROM_PLAYERS) {
                                                int totalCollected = 0;
                                                for (auto& [otherId, otherPlayer] : room->players) {
                                                    if (otherId != playerId && otherPlayer.cash >= card.amount) {
                                                        otherPlayer.cash -= card.amount;
                                                        totalCollected += card.amount;
                                                    }
                                                }
                                                player.cash += totalCollected;
                                                cardAmount = totalCollected;
                                            } else if (card.type == CardType::PAY_TO_PLAYERS) {
                                                int numOtherPlayers = 0;
                                                for (auto& [otherId, otherPlayer] : room->players) {
                                                    if (otherId != playerId) numOtherPlayers++;
                                                }
                                                int perPlayerAmount = card.amount;
                                                int totalPaid = 0;
                                                if (player.cash >= perPlayerAmount * numOtherPlayers) {
                                                    for (auto& [otherId, otherPlayer] : room->players) {
                                                        if (otherId != playerId) {
                                                            player.cash -= perPlayerAmount;
                                                            otherPlayer.cash += perPlayerAmount;
                                                            totalPaid += perPlayerAmount;
                                                        }
                                                    }
                                                    cardAmount = -totalPaid;
                                                }
                                            } else if (card.type == CardType::GET_OUT_OF_JAIL) {
                                                cardGetOutOfJail = true;
                                            } else if (card.type == CardType::GO_TO_JAIL) {
                                                player.boardIndex = 10;
                                                room->playerJailStatus[playerId] = 0;
                                                cardGoToJail = true;
                                                wentToJail = true;
                                            }
                                            
                                            if (card.type != CardType::GET_OUT_OF_JAIL) {
                                                g_serverState.communityChestDecks[roomId].returnCard(card);
                                            }
                                        }
                                    }
                                    
                                    int newPosition = player.boardIndex;
                                    bool needsRent = false;
                                    int rentAmount = 0;
                                    int ownerPlayerId = -1;
                                    
                                    std::cout << "[Server] Checking for rent at position " << newPosition << std::endl;
                                    std::cout << "[Server] Total players in room: " << room->players.size() << std::endl;
                                    
                                    for (const auto& [otherPlayerId, otherPlayer] : room->players) {
                                        if (otherPlayerId != playerId) {
                                            std::cout << "[Server] Checking player " << otherPlayerId << " assets (count: " << otherPlayer.assets.size() << ")" << std::endl;
                                            for (const std::string& assetStr : otherPlayer.assets) {
                                                try {
                                                    int propertyId = std::stoi(assetStr);
                                                    std::cout << "[Server]   Player " << otherPlayerId << " owns property " << propertyId << std::endl;
                                                    if (propertyId == newPosition) {
                                                        ownerPlayerId = otherPlayerId;
                                                        rentAmount = calculateRent(newPosition, ownerPlayerId, room->players, total);
                                                        needsRent = true;
                                                        std::cout << "[Server] ===== Property at position " << newPosition 
                                                                  << " is owned by player " << ownerPlayerId 
                                                                  << ", rent: $" << rentAmount << " =====" << std::endl;
                                                        break;
                                                    }
                                                } catch (...) {
                                                    std::cout << "[Server] WARNING: Failed to parse asset: " << assetStr << std::endl;
                                                }
                                            }
                                            if (needsRent) break;
                                        }
                                    }
                                    
                                    if (!needsRent) {
                                        std::cout << "[Server] No rent needed at position " << newPosition << std::endl;
                                    }

                                    if (needsRent && ownerPlayerId >= 0) {
                                        if (player.cash >= rentAmount) {
                                            player.cash -= rentAmount;
                                            auto ownerIt = room->players.find(ownerPlayerId);
                                            if (ownerIt != room->players.end()) {
                                                ownerIt->second.cash += rentAmount;
                                                std::cout << "[Server] ===== Player " << playerId 
                                                          << " paid $" << rentAmount << " rent to player " << ownerPlayerId 
                                                          << ", payer cash: $" << player.cash 
                                                          << ", owner cash: $" << ownerIt->second.cash << " =====" << std::endl;
                                            }
                                        } else {
                                            std::cout << "[Server] ===== Player " << playerId 
                                                      << " cannot pay rent $" << rentAmount 
                                                      << " (has $" << player.cash << ") =====" << std::endl;
                                        }
                                    }
                                    
                                    needBroadcastEvent = true;
                                    event["type"] = "event";
                                    event["topic"] = "game.move";
                                    event["payload"] = {
                                        {"playerId", playerId},
                                        {"dice1", dice1},
                                        {"dice2", dice2},
                                        {"total", total},
                                        {"newPosition", player.boardIndex}
                                    };
                                    
                                    // special
                                    if (wentToJail) {
                                        event["payload"]["wentToJail"] = true;
                                        event["payload"]["jailPosition"] = 10;
                                        event["payload"]["jailStatus"] = 0; 
                                        event["payload"]["jailTurnsRemaining"] = 3;
                                        std::cout << "[Server] ===== Added wentToJail=true, jailStatus=0, jailTurnsRemaining=3 to event payload =====" << std::endl;
                                    }
                                    
                                    if (paidTax) {
                                        event["payload"]["taxPaid"] = true;
                                        event["payload"]["taxAmount"] = taxAmount;
                                        event["payload"]["cashAfterTax"] = player.cash; 
                                        std::cout << "[Server] ===== Added taxPaid=true, taxAmount=" << taxAmount 
                                                  << ", cashAfterTax=" << player.cash << " to event payload =====" << std::endl;
                                    }
                                    
                                    if (passedGo) {
                                        event["payload"]["passedGo"] = true;
                                        event["payload"]["goBonus"] = 200;
                                        std::cout << "[Server] ===== Added passedGo=true, goBonus=200 to event payload =====" << std::endl;
                                    }
                                    
                                    if (drewCard) {
                                        event["payload"]["drewCard"] = true;
                                        event["payload"]["cardType"] = cardType;
                                        event["payload"]["cardDescription"] = cardDescription;
                                        if (cardAmount != 0) {
                                            event["payload"]["cardAmount"] = cardAmount;
                                        }
                                        if (cardTargetTile >= 0) {
                                            event["payload"]["cardTargetTile"] = cardTargetTile;
                                        }
                                        if (cardStep != 0) {
                                            event["payload"]["cardStep"] = cardStep;
                                        }
                                        if (cardGetOutOfJail) {
                                            event["payload"]["cardGetOutOfJail"] = true;
                                        }
                                        if (cardGoToJail) {
                                            event["payload"]["cardGoToJail"] = true;
                                        }
                                        std::cout << "[Server] ===== Added card info to event payload =====" << std::endl;
                                    }
                                    
                                    if (gotOutOfJail) {
                                        event["payload"]["gotOutOfJail"] = true;
                                        event["payload"]["jailRoll"] = true;
                                        event["payload"]["isDouble"] = true;
                                        std::cout << "[Server] ===== Added gotOutOfJail=true to event payload =====" << std::endl;
                                    }
                                    
                                    if (needsRent && ownerPlayerId >= 0) {
                                        event["payload"]["rentPaid"] = true;
                                        event["payload"]["rentAmount"] = rentAmount;
                                        event["payload"]["ownerId"] = ownerPlayerId;
                                        event["payload"]["payerCashAfterRent"] = player.cash;
                                        auto ownerIt = room->players.find(ownerPlayerId);
                                        if (ownerIt != room->players.end()) {
                                            event["payload"]["ownerCashAfterRent"] = ownerIt->second.cash;
                                        }
                                        std::cout << "[Server] ===== Added rentPaid=true, rentAmount=" << rentAmount 
                                                  << ", ownerId=" << ownerPlayerId 
                                                  << ", payerCashAfterRent=" << player.cash 
                                                  << " to event payload =====" << std::endl;
                                    }
                                    
                                    std::cout << "[Server] ===== Final event payload: " << event["payload"].dump() << " =====" << std::endl;
                                } else {
                                    std::cout << "[Server] WARNING: Player " << playerId << " tried to roll but it's not their turn!" << std::endl;
                                    std::cout << "[Server]   currentTurnIndex=" << room->currentTurnIndex 
                                              << ", playersList.size()=" << playersList.size() << std::endl;
                                    if (room->currentTurnIndex >= 0 && room->currentTurnIndex < static_cast<int>(playersList.size())) {
                                        std::cout << "[Server]   Current turn player: " << playersList[room->currentTurnIndex].playerId << std::endl;
                                    }
                                    json error;
                                    error["type"] = "error";
                                    error["message"] = "Not your turn";
                                    ws->send(error.dump(), uWS::OpCode::TEXT);
                                    return;
                                }
                                
                            } else if (actionType == "buy") {
                                std::cout << "[Server] Processing buy action from player " << playerId << std::endl;
                                
                                std::vector<PlayerState> playersList;
                                for (const auto& [id, p] : room->players) {
                                    playersList.push_back(p);
                                }
                                std::sort(playersList.begin(), playersList.end(),
                                          [](const PlayerState& a, const PlayerState& b) {
                                              return a.playerId < b.playerId;
                                          });
                                
                                bool isPlayerTurn = (room->currentTurnIndex >= 0 && 
                                                    room->currentTurnIndex < static_cast<int>(playersList.size()) &&
                                                    playersList[room->currentTurnIndex].playerId == playerId);
                                
                                if (!isPlayerTurn) {
                                    std::cout << "[Server] Player " << playerId << " tried to buy but it's not their turn" << std::endl;
                                    json error;
                                    error["type"] = "error";
                                    error["message"] = "Not your turn";
                                    ws->send(error.dump(), uWS::OpCode::TEXT);
                                    return;
                                }
                                
                                json details = msg.value("details", json::object());
                                std::cout << "[Server] Buy details: " << details.dump() << std::endl;
                                
                                int propertyId = details.value("propertyId", msg.value("propertyId", -1));
                                int cost = details.value("cost", details.value("price", msg.value("cost", 0)));
                                
                                std::cout << "[Server] Parsed propertyId=" << propertyId << ", cost=" << cost << std::endl;
                                
                                if (propertyId < 0) {
                                    propertyId = player.boardIndex;
                                    std::cout << "[Server] Using player position as propertyId: " << propertyId << std::endl;
                                }
                                
                                bool alreadyOwned = false;
                                int ownerPlayerId = -1;
                                for (const auto& [otherPlayerId, otherPlayer] : room->players) {
                                    for (const std::string& assetStr : otherPlayer.assets) {
                                        try {
                                            int ownedPropertyId = std::stoi(assetStr);
                                            if (ownedPropertyId == propertyId) {
                                                alreadyOwned = true;
                                                ownerPlayerId = otherPlayerId;
                                                break;
                                            }
                                        } catch (...) {
                                        }
                                    }
                                    if (alreadyOwned) break;
                                }
                                
                                if (alreadyOwned) {
                                    std::cout << "[Server] Property " << propertyId 
                                              << " is already owned by player " << ownerPlayerId << std::endl;
                                    json error;
                                    error["type"] = "error";
                                    error["message"] = "Property already owned";
                                    ws->send(error.dump(), uWS::OpCode::TEXT);
                                    return;
                                }
                                
                                if (cost <= 0) {
                                    std::cout << "[Server] Invalid cost: " << cost << std::endl;
                                    json error;
                                    error["type"] = "error";
                                    error["message"] = "Invalid cost";
                                    ws->send(error.dump(), uWS::OpCode::TEXT);
                                    return;
                                }
                                
                                if (player.cash >= cost) {
                                    player.cash -= cost;
                                    player.assets.push_back(std::to_string(propertyId));
                                    
                                    std::cout << "[Server] Player " << playerId << " bought property " 
                                              << propertyId << " for $" << cost 
                                              << ", remaining cash: $" << player.cash << std::endl;
                                    
                                    needBroadcastEvent = true;
                                    event["type"] = "event";
                                    event["topic"] = "game.buy";
                                    event["payload"] = {
                                        {"playerId", playerId},
                                        {"propertyId", propertyId},
                                        {"cost", cost},
                                        {"cash", player.cash},
                                        {"success", true}
                                    };
                                } else {
                                    std::cout << "[Server] Player " << playerId << " insufficient cash: $" 
                                              << player.cash << " < $" << cost << std::endl;
                                    json error;
                                    error["type"] = "error";
                                    error["message"] = "Insufficient cash";
                                    ws->send(error.dump(), uWS::OpCode::TEXT);
                                    return;
                                }
                                
                            } else if (actionType == "sell") {
                                
                                json details = msg.value("details", json::object());
                                int propertyId = details.value("propertyId", msg.value("propertyId", -1));
                                int price = details.value("price", msg.value("price", 0));
                                
                                if (propertyId < 0 && details.contains("property")) {
                                    try {
                                        propertyId = std::stoi(details["property"].get<std::string>());
                                    } catch (...) {
                                        std::cout << "[Server] WARNING: Failed to parse property ID from string" << std::endl;
                                    }
                                }
                                
                                std::cout << "[Server] Processing sell action: propertyId=" << propertyId << ", price=" << price << std::endl;
                                
                                if (propertyId < 0) {
                                    std::cout << "[Server] Invalid propertyId: " << propertyId << std::endl;
                                    json error;
                                    error["type"] = "error";
                                    error["message"] = "Invalid property ID";
                                    ws->send(error.dump(), uWS::OpCode::TEXT);
                                    return;
                                }
                                
                                if (price <= 0) {
                                    std::cout << "[Server] Invalid price: " << price << std::endl;
                                    json error;
                                    error["type"] = "error";
                                    error["message"] = "Invalid price";
                                    ws->send(error.dump(), uWS::OpCode::TEXT);
                                    return;
                                }
                                
                                auto it = std::find(player.assets.begin(), player.assets.end(), 
                                                    std::to_string(propertyId));
                                if (it != player.assets.end()) {
                                    player.assets.erase(it);
                                    player.cash += price;
                                    
                                    std::cout << "[Server] Player " << playerId << " sold property " 
                                              << propertyId << " for $" << price 
                                              << ", new cash: $" << player.cash << std::endl;
                                    
                                    needBroadcastEvent = true;
                                    event["type"] = "event";
                                    event["topic"] = "game.sell";
                                    event["payload"] = {
                                        {"playerId", playerId},
                                        {"propertyId", propertyId},
                                        {"price", price},
                                        {"cash", player.cash}
                                    };
                                } else {
                                    std::cout << "[Server] Player " << playerId << " does not own property " << propertyId << std::endl;
                                    json error;
                                    error["type"] = "error";
                                    error["message"] = "You do not own this property";
                                    ws->send(error.dump(), uWS::OpCode::TEXT);
                                    return;
                                }
                                
                            } else if (actionType == "end") {
                                std::vector<PlayerState> playersList;
                                for (const auto& [id, p] : room->players) {
                                    playersList.push_back(p);
                                }
                                std::sort(playersList.begin(), playersList.end(),
                                          [](const PlayerState& a, const PlayerState& b) {
                                              return a.playerId < b.playerId;
                                          });
                                
                                if (room->currentTurnIndex >= 0 && 
                                    room->currentTurnIndex < static_cast<int>(playersList.size()) &&
                                    playersList[room->currentTurnIndex].playerId == playerId) {
                                    
                                    auto jailIt = room->playerJailStatus.find(playerId);
                                    int jailTurnsRemaining = -1;
                                    if (jailIt != room->playerJailStatus.end() && jailIt->second >= 0) {
                                        jailIt->second++; 
                                        jailTurnsRemaining = 3 - jailIt->second;
                                        std::cout << "[Server] Player " << playerId 
                                                  << " is in jail, turn count: " << jailIt->second 
                                                  << ", turns remaining: " << jailTurnsRemaining << std::endl;
                                        
                                        if (jailIt->second >= 3) {
                                            room->playerJailStatus.erase(jailIt);
                                            jailTurnsRemaining = 0; 
                                            std::cout << "[Server] Player " << playerId 
                                                      << " released from jail after 3 turns" << std::endl;
                                        }
                                    }
                                    
                                    int nextPlayerId = playersList[(room->currentTurnIndex + 1) % playersList.size()].playerId;
                                    room->currentTurnIndex = (room->currentTurnIndex + 1) % playersList.size();
                                    
                                    std::cout << "[Server] Player " << playerId << " ended turn, "
                                              << "next player: " << nextPlayerId << std::endl;
                                    
                                    needBroadcastEvent = true;
                                    event["type"] = "event";
                                    event["topic"] = "game.turn_ended";
                                    event["payload"] = {
                                        {"playerId", playerId},
                                        {"nextPlayerId", nextPlayerId}
                                    };
                                    
                                    if (jailTurnsRemaining >= 0) {
                                        event["payload"]["jailTurnsRemaining"] = jailTurnsRemaining;
                                    }
                                }
                                
                            } else if (actionType == "pay_bail") {
                                auto jailIt = room->playerJailStatus.find(playerId);
                                if (jailIt == room->playerJailStatus.end() || jailIt->second < 0) {

                                    std::cout << "[Server] Player " << playerId << " tried to pay bail but is not in jail" << std::endl;
                                    json error;
                                    error["type"] = "error";
                                    error["message"] = "You are not in jail";
                                    ws->send(error.dump(), uWS::OpCode::TEXT);
                                    return;
                                }
                                
                                if (player.cash >= 50) {
                                    player.cash -= 50;
                                    room->playerJailStatus.erase(jailIt);
                                    
                                    std::cout << "[Server] ===== Player " << playerId 
                                              << " paid $50 bail and got out of jail, remaining cash: $" 
                                              << player.cash << " =====" << std::endl;
                                    
                                    needBroadcastEvent = true;
                                    event["type"] = "event";
                                    event["topic"] = "game.jail_released";
                                    event["payload"] = {
                                        {"playerId", playerId},
                                        {"method", "pay_bail"},
                                        {"bailAmount", 50},
                                        {"cash", player.cash}
                                    };
                                } else {
                                    std::cout << "[Server] Player " << playerId 
                                              << " cannot pay bail: insufficient cash ($" << player.cash << " < $50)" << std::endl;
                                    json error;
                                    error["type"] = "error";
                                    error["message"] = "Insufficient cash to pay bail ($50 required)";
                                    ws->send(error.dump(), uWS::OpCode::TEXT);
                                    return;
                                }
                                
                            } else if (actionType == "view_property") {
                                std::cout << "[Server] Player " << playerId << " requested to view properties" << std::endl;
                                
                            } else if (actionType == "hint") {
                                std::cout << "[Server] Processing hint action from player " << playerId << std::endl;
                                
                                std::vector<PlayerState> playersList;
                                for (const auto& [id, p] : room->players) {
                                    playersList.push_back(p);
                                }
                                std::sort(playersList.begin(), playersList.end(),
                                          [](const PlayerState& a, const PlayerState& b) {
                                              return a.playerId < b.playerId;
                                          });
                                
                                bool isPlayerTurn = (room->currentTurnIndex >= 0 && 
                                                    room->currentTurnIndex < static_cast<int>(playersList.size()) &&
                                                    playersList[room->currentTurnIndex].playerId == playerId);
                                
                                if (!isPlayerTurn) {
                                    std::cout << "[Server] Player " << playerId << " tried to use hint but it's not their turn" << std::endl;
                                    json error;
                                    error["type"] = "error";
                                    error["message"] = "Not your turn";
                                    ws->send(error.dump(), uWS::OpCode::TEXT);
                                    return;
                                }
                                
                                json details = msg.value("details", json::object());
                                std::cout << "[Server] Hint details: " << details.dump() << std::endl;
                                
                                int cost = details.value("cost", msg.value("cost", 50));
                                std::cout << "[Server] Parsed hint cost=" << cost << std::endl;
                                
                                if (player.cash >= cost) {
                                    player.cash -= cost;
                                    
                                    std::cout << "[Server] Player " << playerId << " used hint, "
                                              << "cost: $" << cost << ", remaining cash: $" << player.cash << std::endl;
                                    
                                    // boadcast
                                    needBroadcastEvent = true;
                                    event["type"] = "event";
                                    event["topic"] = "game.hint";
                                    event["payload"] = {
                                        {"playerId", playerId},
                                        {"cost", cost},
                                        {"cash", player.cash}
                                    };
                                } else {
                                    std::cout << "[Server] Player " << playerId << " insufficient cash for hint: $" 
                                              << player.cash << " < $" << cost << std::endl;
                                    json error;
                                    error["type"] = "error";
                                    error["message"] = "Insufficient cash";
                                    ws->send(error.dump(), uWS::OpCode::TEXT);
                                    return;
                                }
                            }
                        }
                        
                        if (needBroadcastEvent) {
                            std::cout << "[Server] ===== Broadcasting event to room " << roomId << " =====" << std::endl;
                            std::cout << "[Server] ===== Event JSON: " << event.dump(2) << " =====" << std::endl;
                            g_serverState.broadcastToRoom(roomId, "event", event);
                        } else {
                            std::cout << "[Server] WARNING: needBroadcastEvent is false, event not broadcast!" << std::endl;
                        }
                        
                        g_serverState.broadcastStateUpdate(roomId);
                        
                    } else if (type == "chat") {
                        std::string message = msg.value("message", "");
                        int targetPlayerId = msg.value("targetPlayerId", -1); 
                        
                        std::cout << "[Server] Received chat message from player " << playerId 
                                  << ": " << message;
                        if (targetPlayerId >= 0) {
                            std::cout << " (private to player " << targetPlayerId << ")";
                        } else {
                            std::cout << " (group chat)";
                        }
                        std::cout << std::endl;
                        
                        std::string roomId;
                        {
                            std::lock_guard<std::mutex> lock(g_serverState.mutex);
                            auto roomIt = g_serverState.playerToRoom.find(playerId);
                            if (roomIt == g_serverState.playerToRoom.end()) {
                                return;
                            }
                            roomId = roomIt->second;
                        }
                        
                        json chatMsg;
                        chatMsg["type"] = "chat";
                        chatMsg["playerId"] = playerId;
                        chatMsg["playerName"] = "";
                        chatMsg["message"] = message;
                        chatMsg["isPrivate"] = (targetPlayerId >= 0);
                        if (targetPlayerId >= 0) {
                            chatMsg["targetPlayerId"] = targetPlayerId;
                        }
                        
                        {
                            std::lock_guard<std::mutex> lock(g_serverState.mutex);
                            auto roomIt = g_serverState.rooms.find(roomId);
                            if (roomIt != g_serverState.rooms.end()) {
                                auto room = roomIt->second;
                                std::lock_guard<std::mutex> roomLock(room->mutex);
                                auto playerIt = room->players.find(playerId);
                                if (playerIt != room->players.end()) {
                                    std::string displayName = playerIt->second.displayName;
                                    if (!displayName.empty()) {
                                        chatMsg["playerName"] = displayName;
                                    } else {
                                        chatMsg["playerName"] = "Player" + std::to_string(playerId);
                                    }
                                    std::cout << "[Server] Chat message from player " << playerId 
                                              << " (" << chatMsg["playerName"].get<std::string>() << ")" << std::endl;
                                } else {
                                    std::cout << "[Server] WARNING: Player " << playerId << " not found in room " << roomId << std::endl;
                                    chatMsg["playerName"] = "Player" + std::to_string(playerId);
                                }
                            } else {
                                std::cout << "[Server] WARNING: Room " << roomId << " not found" << std::endl;
                                chatMsg["playerName"] = "Player" + std::to_string(playerId);
                            }
                        }
                        
                        if (targetPlayerId >= 0) {
                            auto senderIt = g_serverState.connections.find(playerId);
                            if (senderIt != g_serverState.connections.end()) {
                                senderIt->second->send(chatMsg.dump(), uWS::OpCode::TEXT);
                                std::cout << "[Server] Sent private chat to sender (player " << playerId << ")" << std::endl;
                            }

                            auto targetIt = g_serverState.connections.find(targetPlayerId);
                            if (targetIt != g_serverState.connections.end()) {
                                targetIt->second->send(chatMsg.dump(), uWS::OpCode::TEXT);
                                std::cout << "[Server] Sent private chat to target (player " << targetPlayerId << ")" << std::endl;
                            } else {
                                std::cout << "[Server] WARNING: Target player " << targetPlayerId << " not found" << std::endl;
                            }
                        } else {

                            std::cout << "[Server] Broadcasting group chat to room " << roomId << std::endl;
                            g_serverState.broadcastToRoom(roomId, "chat", chatMsg);
                        }
                        
                    } else if (type == "heartbeat") {

                    }
                    
                } catch (const std::exception& e) {
                    std::cerr << "[Server] Error processing message: " << e.what() << std::endl;
                    json error;
                    error["type"] = "error";
                    error["message"] = std::string("Server error: ") + e.what();
                    ws->send(error.dump(), uWS::OpCode::TEXT);
                }
            },
            
            .close = [](auto* ws, int code, std::string_view message) {
                auto* data = static_cast<PerSocketData*>(ws->getUserData());
                int playerId = data->playerId;
                
                std::string roomId;
                {
                    std::lock_guard<std::mutex> lock(g_serverState.mutex);
                    

                    auto roomIt = g_serverState.playerToRoom.find(playerId);
                    if (roomIt != g_serverState.playerToRoom.end()) {
                        roomId = roomIt->second;
                        auto room = g_serverState.rooms[roomId];
                        std::lock_guard<std::mutex> roomLock(room->mutex);
                        room->players.erase(playerId);
                        std::cout << "[Server] Player " << playerId << " left room " << roomId << std::endl;
                        
                        g_serverState.playerToRoom.erase(playerId);
                    }
                    
                    g_serverState.connections.erase(playerId);
                }
                

                if (!roomId.empty()) {
                    g_serverState.broadcastStateUpdate(roomId);
                }
                
                std::cout << "[Server] Player #" << playerId << " disconnected" << std::endl;
            }
        })
        .listen(9002, [](auto* listenSocket) {
            if (listenSocket) {
                std::cout << "[Server] ========================================" << std::endl;
                std::cout << "[Server] WebSocket server listening on port 9002" << std::endl;
                std::cout << "[Server] Server is accessible from all network interfaces (0.0.0.0)" << std::endl;
                std::cout << "[Server] Clients can connect using: ws://<your-ip>:9002" << std::endl;
                std::cout << "[Server] Example: ws://10.30.198.46:9002" << std::endl;
                std::cout << "[Server] ========================================" << std::endl;
            } else {
                std::cerr << "[Server] ERROR: Failed to listen on port 9002" << std::endl;
                std::cerr << "[Server] Check if port 9002 is already in use or firewall is blocking" << std::endl;
            }
        })
        .run();
    
    return 0;
}

