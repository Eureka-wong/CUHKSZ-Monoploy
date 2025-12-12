#include <string>

#include "Board.h"
#include "Tile.h"

using namespace std;

Board::Board() {
    vector<TileInfo> tileInfoVector = {
        TileInfo("Property", "Research Hall (Brown 1)", 1, 60, 50, {{2, 10, 30, 90, 160, 250}}, "Brown"),
        TileInfo("Community Chest", "Community Chest 1", 2),
        TileInfo("Property", "Sports Hall (Brown 2)", 3, 60, 50, {{4, 20, 60, 180, 320, 450}}, "Brown"),
        TileInfo("Tax", "Income Tax", 4, 200),
        TileInfo("Property", "Bus Stop 1 (Station 1)", 5, 200, 100, {{25, 50, 100, 200, 0, 0}}, "Station"),
        TileInfo("Property", "Administration (Light Blue 1)", 6, 100, 50, {{6, 30, 90, 270, 400, 550}}, "Light Blue"),
        TileInfo("Chance", "Chance 1", 7),
        TileInfo("Property", "TA (Light Blue 2)", 8, 100, 50, {{6, 30, 90, 270, 400, 550}}, "Light Blue"),
        TileInfo("Property", "TB (Light Blue 3)", 9, 120, 50, {{8, 40, 100, 300, 450, 600}}, "Light Blue"),

        TileInfo("Free Parking", "Presidential Office", 10),
        TileInfo("Property", "Shaw College (Pink 1)", 11, 140, 100, {{10, 50, 150, 450, 625, 750}}, "Pink"),
        TileInfo("Property", "Electric Company (Utilities 1)", 12, 150, 0, {{0, 0, 0, 0, 0, 0}}, "Utility"),
        TileInfo("Property", "Student Center (Pink 2)", 13, 140, 100, {{10, 50, 150, 450, 625, 750}}, "Pink"),
        TileInfo("Property", "Zhixin (Pink 3)", 14, 160, 100, {{12, 60, 180, 500, 700, 900}}, "Pink"),
        TileInfo("Property", "Bus Stop 2 (Station 2)", 15, 200, 0, {{25, 50, 100, 200, 0, 0}}, "Station"),
        TileInfo("Property", "TD (Orange 1)", 16, 180, 100, {{14, 70, 200, 550, 750, 950}}, "Orange"),
        TileInfo("Community Chest", "Community Chest 2", 17),
        TileInfo("Property", "TC (Orange 2)", 18, 180, 100, {{14, 70, 200, 550, 750, 950}}, "Orange"),
        TileInfo("Property", "Library (Orange 3)", 19, 200, 100, {{16, 80, 220, 600, 800, 1000}}, "Orange"),

        TileInfo("Free Parking", "Canteen (Free Parking)", 20),
        TileInfo("Property", "Ling College (Red 1)", 21, 220, 150, {{18, 90, 250, 700, 875, 1050}}, "Red"),
        TileInfo("Chance", "Chance 2", 22),
        TileInfo("Property", "Harmonia (Red 2)", 23, 220, 150, {{18, 90, 250, 700, 875, 1050}}, "Red"),
        TileInfo("Property", "(Red 3)", 24, 240, 150, {{20, 100, 300, 750, 925, 1100}}, "Red"),
        TileInfo("Property", "Bus Stop 3 (Station 3)", 25, 200, 0, {{25, 50, 100, 200, 0, 0}}, "Station"),
        TileInfo("Property", "Minerva (Yellow 1)", 26, 260, 150, {{22, 110, 330, 800, 975, 1150}}, "Yellow"),
        TileInfo("Property", "Duan (Yellow 2)", 27, 260, 150, {{22 ,110 ,330, 800, 975, 1150}}, "Yellow"),
        TileInfo("Property", "Water Works (Utilities 2)", 28, 150, 0, {{0, 0, 0, 0, 0, 0}}, "Utility"),
        TileInfo("Property", "Muse (Yellow 3)", 29, 280, 150, {{24, 120, 360, 850, 1025, 1200}}, "Yellow"),

        TileInfo("Go to Jail", "Go to Presidential's Office", 30),
        TileInfo("Property", "Eighth College (Green 1)", 31, 300, 200, {{26, 130, 390, 900, 1100, 1275}}, "Green"),
        TileInfo("Property", "School of Music (Green 2)", 32, 300, 200, {{26, 130, 390, 900, 1100, 1275}}, "Green"),
        TileInfo("Community Chest", "Community Chest 3", 33),
        TileInfo("Property", "Fairy Lake (Green 3)", 34, 320, 200, {{28, 150, 450, 1000, 1200, 1400}}, "Green"),
        TileInfo("Property", "Bus Stop 4 (Station 4)", 35, 200, 0, {{25, 50, 100, 200, 0, 0}}, "Station"),
        TileInfo("Chance", "Chance 3", 36),
        TileInfo("Property", "TxC (Dark Blue 1)", 37, 350, 200, {{35, 175, 500, 1100, 1300, 1500}}, "Dark Blue"),
        TileInfo("Tax", "Tax 2", 38, 100),
        TileInfo("Property", "Liwen Hall (Dark Blue 2)", 39, 400, 200, {{50, 200, 600, 1400, 1700, 2000}}, "Dark Blue")
    };

    tiles.push_back(make_unique<GoTile>());
    for (const auto& info : tileInfoVector) {
        if (info.tileType == "Property") {
            tiles.push_back(make_unique<PropertyTile>(info));
        } else if (info.tileType == "Free Parking") {
            tiles.push_back(make_unique<FreeParkingTile>(info));
        } else if (info.tileType == "Tax") {
            tiles.push_back(make_unique<TaxTile>(info));
        } else if (info.tileType == "Go to Jail") {
            tiles.push_back(make_unique<GoToJailTile>(info));
        } else if (info.tileType == "Chance") {
            tiles.push_back(make_unique<ChanceTile>(info));
        } else if (info.tileType == "Community Chest") {
            tiles.push_back(make_unique<CommunityChestTile>(info));
        }
    }
}

Tile& Board::getTile(int index) const {
    return *tiles[index % size];
}

int Board::getSize() const {
    return size;
}
