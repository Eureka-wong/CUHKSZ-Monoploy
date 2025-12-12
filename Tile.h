#ifndef TILE_H
#define TILE_H

#include <string>
#include <array>
#include <QObject>

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
             std::array<int, 6> rent = std::array<int, 6>{0, 0, 0, 0, 0, 0}, std::string colorGroup = "None");
};

class Tile:public QObject{
    Q_OBJECT
protected:
    std::string name;
    int index;
public:
    Tile(const std::string& name, int index, QObject* parent = nullptr); // Constructor
    virtual ~Tile() {}; // Destructor

    // Event triggered when a player lands on the tile
    virtual void onLand(Player& player, Game& game, int step = 0) = 0;

    // Main accessors
    virtual std::string getName() const;
    virtual int getIndex() const;
};

class GoTile : public Tile {
    Q_OBJECT
public:
    GoTile(QObject* parent = nullptr);
    void onLand(Player& player, Game& game, int step) override;
signals:
    void passedGo(const QString& playerName, int reward);
};


class PropertyTile : public Tile {
    Q_OBJECT

    // Fixed variables
    const int price;
    const int housePrice;
    const std::array<int, 6> rent;
    const std::string group;

    Player* owner = nullptr;
    int houses = 0;
    bool mortgaged = false;
public:
    PropertyTile(const TileInfo& info, QObject* parent = nullptr);
    void onLand(Player& player, Game& game, int step) override;

    // Property management
    void buyProperty(Player& player);
    void setOwnerWithoutPayment(Player& player);  // 设置所有权但不扣钱（用于服务器同步）

    bool sellProperty(Player& player, Game& game);
    bool buyBuilding(Player& player, Game& game);
    bool sellBuilding(Player& player, Game& game);
    bool mortgageProperty(Player& player, Game& game);

    void liquidateBuildings(Game& game, Player* creditor = nullptr);
    void transferOwnership(Player& previousOwner, Player* newOwner, Game& game);

    // Accessors
    int getPrice() const;
    int getHouses() const;
    Player* getOwner() const;
    bool isMortgaged() const;
    int getHousePrice() const;
    std::array<int, 6> getRent() const;
    std::string getGroup() const;

    // Helpers
    int countOwnedPropertiesInGroup() const;
    bool ownColorGroup() const;
    bool allowHouseTransactions(bool buy) const;
    bool allPropertyInGroupHasNoHouses() const;

    int calculateRent(int step) const;
    bool ownedByPlayer(Player& player) const;
    bool isStationOrUtility() const;
    int calculateValue() const;

signals:
    //void purchaseOpportunity(const QString& propertyName, int price, const QString& playerName);
    void propertyAlreadyOwned(const QString& propertyName, const QString& ownerName);
    void propertyMortgaged(const QString& propertyName, const QString& ownerName);
    void buildingOpportunity(const QString& propertyName, int housePrice, const QString& playerName);
};

class FreeParkingTile : public Tile {
    Q_OBJECT
public:
    FreeParkingTile(const TileInfo& info, QObject* parent = nullptr);
    void onLand(Player& player, Game& game, int step) override;
signals:
    void freeParkingLanded(const QString& playerName);
};

class TaxTile : public Tile {
    Q_OBJECT
    int tax;
public:
    TaxTile(const TileInfo& info,QObject* parent = nullptr);
    void onLand(Player& player, Game& game, int step) override;
signals:
    void taxDue(const QString& tileName, int taxAmount, const QString& playerName);
};

class GoToJailTile : public Tile {
    Q_OBJECT
public:
    GoToJailTile(const TileInfo& info, QObject* parent = nullptr);
    void onLand(Player& player, Game& game, int step) override;
signals:
    void passedGo(const QString& playerName, int reward);
};

class ChanceTile: public Tile {
    Q_OBJECT
public:
    ChanceTile(const TileInfo& info, QObject* parent = nullptr);
    void onLand(Player& player, Game& game, int step) override;
signals:
    void cardDrawn(const QString& cardMessage, const QString& playerName);
};

class CommunityChestTile: public Tile {
    Q_OBJECT
public:
    CommunityChestTile(const TileInfo& info, QObject* parent = nullptr);
    void onLand(Player& player, Game& game, int step) override;
signals:
    void cardDrawn(const QString& cardMessage, const QString& playerName);
};

#endif
