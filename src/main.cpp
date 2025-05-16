#include "chess_board.h"
#include <iostream>

int main() {
    setboard();
    printboard();

    int x1, y1, x2, y2;
    while (true) {
        cout << "\nEnter move (x1 y1 x2 y2): ";
        cin >> x1 >> y1 >> x2 >> y2;
        if (x1 < 0) break;

        changeposition(x1, y1, x2, y2);
        printboard();
    }

    return 0;
}