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

Chesspiece* createPiece(int index, int x, int y, bool t, bool m) {
    switch (index){
        case 1: return new Chesspiece("Pawn", 1, " P ", x, y, t, m);
        case 2: return new Chesspiece("Knight", 3, " C ", x, y, t, m);
        case 3: return new Chesspiece("Bishop", 3, " B ", x, y, t, m);
        case 4: return new Chesspiece("Rook", 5, " R ", x, y, t, m);
        case 5: return new Chesspiece("Queen", 9, " Q ", x, y, t, m);
        case 6: return new Chesspiece("King", 0, " K ", x, y, t, m);
        default: return nullptr;
    }
}

void setboard() {
    cout << "Setting up board:\n";
    int i = 0;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            int pieceCode = board_init[i++];
            bool t = false;
            if (pieceCode != 0) {
                if (y >= 6){
                    t = true;
                }
                Chesspiece* piece = createPiece(pieceCode, x, y, t, 0);
                board[y][x] = piece;
                cout << t << " ," << flush;
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
    cout << flush;
}

void changeposition(int x1, int y1, int x2, int y2, bool t, bool m) {
    cout << m << endl;
    Chesspiece* piece = board[y1][x1];

    if ((piece->getName()=="Pawn") && (y2 == (t ? 0 : 7))){
        int newindex;
        if((piece->getTeam()==1)){
            cout << "PROMOTION! Choose (2=Knight, 3=Bishop, 4=Rook, 5=Queen): ";
            cin >> newindex;
        }
        else{
            newindex = 5;//TODO: opponent promotion
        }

        if (board[y2][x2]) {
            cout << "Captured: " << board[y2][x2]->getName() << endl;
            delete board[y2][x2];
        }

        delete board[y1][x1];
        piece = createPiece(newindex, x2, y2, t, 1);
        board[y2][x2] = piece;
        board[y1][x1] = nullptr;
        manager.addpiece(piece);
        cout << "Pawn promoted and moved to (" << x2 << "," << y2 << ")\n";
        return;
    }
    
    if (piece->getName()=="King"){
        if (Movrules::cancastle(board,x1,y1,x2, y2, true, t)){
            Movrules::performcastle(board,x1,y1,x2, y2, true);
            cout<<"castle success"<<endl;
            return;
        }
        cout<<"no castle"<<endl;
    }

    if (!Movrules::isvalidmove(piece, x1, y1, x2, y2, board, piece->getTeam(), false, m)) {
    cout << "Invalid move for " << piece->getName() << endl;
    return;
}

    if (board[y2][x2]) {
        cout << "Captured: " << board[y2][x2]->getName() << endl;
        delete board[y2][x2];
        board[y2][x2] = nullptr;
    }
    


    board[y2][x2] = piece;
    board[y1][x1] = nullptr;
    piece->setmoved(true);
    piece->setPosition(x2, y2);
    cout << "Moved to (" << x2 << "," << y2 << ")\n";
}
