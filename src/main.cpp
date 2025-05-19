#include "chess_board.h"
#include <iostream>

int main() {
    setboard();
    printboard();

    int x1, y1, x2, y2;
    bool t;
    while (true) {
        cout << "\nEnter move (x1 y1 x2 y2): ";
        cin >> x1 >> y1 >> x2 >> y2;
        if (x1 < 0) break;
        if (!board[y1][x1]) {
            cout << "No piece at selected position.\n";
        continue;
        }
        t = board[y1][x1]->getTeam();
        changeposition(x1, y1, x2, y2, t);
        printboard();
    }

    return 0;
}