#include "board_algorithm.h"
#include "movement_rules.h"
#include <iostream>
using namespace std;

int board_init[] = {
    4,2,3,5,6,3,2,4,
    1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,
    4,2,3,5,6,3,2,4
};

Chesspiece* board[8][8] = {nullptr};
PieceManager manager;

Chesspiece* createPiece(int index, int x, int y) {
    switch (index){
        case 1: return new Chesspiece("Pawn", 1, " P ", x, y);
        case 2: return new Chesspiece("Knight", 3, " C ", x, y);
        case 3: return new Chesspiece("Bishop", 3, " B ", x, y);
        case 4: return new Chesspiece("Rook", 5, " R ", x, y);
        case 5: return new Chesspiece("Queen", 9, " Q ", x, y);
        case 6: return new Chesspiece("King", 0, " K ", x, y);
        default: return nullptr;
    }
}

void setboard() {
    cout << "Setting up board:\n";
    int i = 0;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            int pieceCode = board_init[i++];
            if (pieceCode != 0) {
                Chesspiece* piece = createPiece(pieceCode, x, y);
                board[y][x] = piece;
                manager.addpiece(piece);
            }
        }
    }
}

void printboard() {
    cout << "\nCurrent Board:\n";
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (board[y][x]) {
                board[y][x]->displaypiece();
            } else {
                cout << " . ";
            }
        }
        cout << endl;
    }
}

void changeposition(int x1, int y1, int x2, int y2) {
    Chesspiece* piece = board[y1][x1];
    if (!piece) {
        cout << "No piece at that position.\n";
        return;
    }

    if (!Movrules::isvalidmove(piece, x1, y1, x2, y2, board)) {
        cout << "Invalid move for " << piece->getName() << endl;
        return;
    }

    if (board[y2][x2]) {
        cout << "Captured: " << board[y2][x2]->getName() << endl;
        delete board[y2][x2];
    }

    board[y2][x2] = piece;
    board[y1][x1] = nullptr;
    piece->setPosition(x2, y2);
    cout << "Moved to (" << x2 << "," << y2 << ")\n";
}
