#ifndef Player_H
#define Player_H

#include <vector>
#include <string>
#include <algorithm>

class PropertyTile;
class Game;

class Player {
private:
    std::string name;
    int position;
    int balance = 0;

    std::vector<PropertyTile*> properties;
public:
    Player(const std::string& name, int position, int balance);

    // Accessors
    std::string getName() const;
    int getPosition() const;
    int getCash() const;
    std::vector<PropertyTile*> getProperties() const;

    // Helpers
    void showPlayer() const;
    void setPosition(int newPosition);
    void addMoney(int amount);
    void deductMoney(int amount);
    void addProperty(PropertyTile* property);
    void removeProperty(PropertyTile* property);
    void manageProperties(Game& game);
};

#endif
