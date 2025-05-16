#include "chess_class.h"
#include <iostream>

using namespace std;

#ifndef MOVEMENTRULES
#define MOVEMENTRULES

class Movrules {
    public:
        static bool isvalidmove(Chesspiece* piece, int x1, int y1, int x2, int y2, Chesspiece* board[8][8]){

        if(!piece) return false;
        string name = piece->getName();
        if (name == "Pawn")   return isvalidpawnmove(piece, x1, y1, x2, y2, board);

        return true;
        };
    private:
    static bool isPathClear(int x1, int y1, int x2, int y2, Chesspiece* board[8][8]) {
        int dx = (x2 - x1) == 0 ? 0 : (x2 - x1) / abs(x2 - x1);
        int dy = (y2 - y1) == 0 ? 0 : (y2 - y1) / abs(y2 - y1);

        x1 += dx;
        y1 += dy;

        while (x1 != x2 || y1 != y2) {
            if (board[y1][x1] != nullptr) return false;
            x1 += dx;
            y1 += dy;
        }

        return true;
    }
    static bool isvalidpawnmove(Chesspiece* piece, int x1, int y1, int x2, int y2, Chesspiece* board[8][8]){
    int direction = 1;
    bool startRow = (direction == 1 && y1 == 1) || (direction == -1 && y1 == 6);

        if (x1 == x2) {
            // Move forward
            if (y2 == y1 + direction && board[y2][x2] == nullptr) return true;
            if (startRow && y2 == y1 + 2 * direction && board[y1 + direction][x2] == nullptr && board[y2][x2] == nullptr)
                return true;
        } else if (abs(x2 - x1) == 1 && y2 == y1 + direction) {
            // Capture
            if (board[y2][x2] != nullptr) return true;
        }

        return false;
}
};





#endif