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

    Chesspiece* selected = board[y1][x1];
    if (!selected) {
        cout << "No piece at that position." << endl;
        continue;
    }

    changeposition(x1, y1, x2, y2, team);
    printboard();

    bool opponentTeam = !team;
    if (Movrules::iskingincheck(opponentTeam, board)) {
        if (Movrules::ischeckmate(opponentTeam, board)) {
            cout << "Checkmate! Team " << (opponentTeam ? "White" : "Black") << " loses.\n";
            break;
        } else {
            cout << "Check to " << (opponentTeam ? "White" : "Black") << "!\n";
        }
    }
}
    return 0;
}