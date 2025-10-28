#ifndef TILE_H
#define TILE_H

#include <iostream>
#include <string>
#include <array>
#include <memory>

class Player;
class Game;

// For initializing tiles on the board.
struct TileInfo {
    std::string tileType;
    std::string name;
    int index;

    // Property-specific attributes
    int price; // Becomes tax for TaxTile
    int housePrice;
    std::array<int, 6> rent;
    std::string group;

    TileInfo(std::string tileType, std::string name, int index, int price = 0, int housePrice = 0,
             std::array<int, 6> rent = {0, 0, 0, 0, 0, 0}, std::string colorGroup = "None");
};

class Tile {
protected: 
    std::string name;
    int index;
public: 
    Tile(const std::string& name, int index); // Constructor
    virtual ~Tile() {}; // Destructor

    // Event triggered when a player lands on the tile
    virtual void onLand(Player& player, Game& game, int step = 0) = 0;

    // Main accessors
    virtual std::string getName() const;
    virtual int getIndex() const;
};

class GoTile : public Tile {
public:
    GoTile();
    void onLand(Player& player, Game& game, int step) override;
};


class PropertyTile : public Tile {
    // Fixed variables
    const int price;
    const int housePrice;
    const std::array<int, 6> rent;
    const std::string group;

    Player* owner = nullptr;
    int houses = 0;
    bool mortgaged = false;
public:
    PropertyTile(const TileInfo& info);
    void onLand(Player& player, Game& game, int step) override;

    // Property management
    void buyProperty(Player& player);
    void sellProperty(Player& player);
    void buyBuilding(Player& player, Game& game);
    void sellBuilding(Player& player, Game& game);
    void mortgageProperty(Player& player);

    // Additional accessors
    int getHouses() const;

    // Helpers
    int countOwnedPropertiesInGroup() const;
    bool ownColorGroup() const;
    bool allowHouseTransactions(bool buy) const;
    void calculateRent(int step, int& rentDue) const;
    bool ownedByPlayer(Player& player) const;
    bool isStationOrUtility() const;
};

class FreeParkingTile : public Tile {
public:
    FreeParkingTile(const TileInfo& info);
    void onLand(Player& player, Game& game, int step) override;
};

class TaxTile : public Tile {
    int tax;
public:
    TaxTile(const TileInfo& info);
    void onLand(Player& player, Game& game, int step) override;
};

class ChanceTile: public Tile {
    
};

class CommunityChestTile: public Tile {

};

class JailTile : public Tile {

};

#endif
