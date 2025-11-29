#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <memory>

#include "Tile.h"

class Board {
private:
    std::vector<std::unique_ptr<Tile>> tiles;
    static const int size = 40;

public:
    Board();

    // Accessors
    Tile& getTile(int index) const;
    int getSize() const;
};

#endif
