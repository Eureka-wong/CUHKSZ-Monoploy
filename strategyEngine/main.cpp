#include <iostream>
#include "hint.h"
#include "../src/Player.h"
#include "../src/Tile.h"
#include "../src/Game.h"
#include "../src/Board.h"

using namespace std;

int main(){ 

    Game game = Game();
    Board board = Board();

    // add test players
   

    std::vector<Player>& players = game.getPlayers();
    Player& player1 = players[0];

    Hint hint(&game);


    vector<possibleActions*> actions = hint.getActions(player1);
    cout << "=== Possible Actions ===" << endl;
    if (actions.empty()) {
    cout << "Warning: No actions generated!" << endl;
    } else {
    cout << "Total actions: " << actions.size() << endl;
    }
    for (auto a : actions) {
        if (a && a->property) {  // 添加空指针检查
            cout << "- " << a->action << " on property " 
                 << a->property->getName() << endl;
        } else {
            cout << "- Invalid action or null property" << endl;
        }
    }

    // ====== 5. 计算评分 ======
    playerstate* state = hint.playerGameinfo(player1);
    vector<possibleActions*> scored = hint.scoreActions(actions, state);

    cout << "\n=== Scored Actions ===" << endl;
    for (auto a : scored) {
        cout << a->action << " on " << a->property->getName()
             << " → score = " << a->score << endl;
    }

    return 0;

}

