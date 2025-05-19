#include "chess_class.h"
#include <iostream>

using namespace std;

#ifndef MOVEMENTRULES
#define MOVEMENTRULES

class Movrules {
    public:
        static bool isvalidmove(Chesspiece* piece, int x1, int y1, int x2, int y2, Chesspiece* board[8][8], bool t){

        if(!piece) return false;
        Chesspiece* target = board[y2][x2];
        if (target && target->getTeam() == t) {
            cout << "Cannot capture own piece!\n";
            return false;
        }
        string name = piece->getName();
        if (name == "Pawn")   return isvalidpawnmove(piece, x1, y1, x2, y2, board, t);
        if (name == "Rook")   return isvalidrookmove(x1, y1, x2, y2, board, t);
        if (name == "Knight") return isvalidknightmove(x1, y1, x2, y2, board, t);
        if (name == "Bishop") return isvalidbishopmove(x1, y1, x2, y2, board, t);
        if (name == "Queen")  return isvalidqueenmove(x1, y1, x2, y2, board, t);
        if (name == "King")   return isvalidkingmove(x1, y1, x2, y2, board, t);

        return true;
        };
    private:
    static bool ispathclear(int x1, int y1, int x2, int y2, Chesspiece* board[8][8], bool t) {
    int dx = (x2 - x1) == 0 ? 0 : (x2 - x1) / abs(x2 - x1);
    int dy = (y2 - y1) == 0 ? 0 : (y2 - y1) / abs(y2 - y1);
    x1 += dx;
    y1 += dy;

    while (x1 != x2 || y1 != y2) {
        cout << "Checking square (" << x1 << ", " << y1 << ")\n";
        if (board[y1][x1] != nullptr) {
            cout << "Blocked by: " << board[y1][x1]->getName() << " at (" << x1 << ", " << y1 << ")\n";
            return false;
        }
        x1 += dx;
        y1 += dy;
    }

    return true;
}

    static bool isvalidpawnmove(Chesspiece* piece, int x1, int y1, int x2, int y2, Chesspiece* board[8][8], bool t){
    int direction = t ? -1 : 1;
    bool startRow = (direction == 1 && y1 == 1) || (direction == -1 && y1 == 6);

        if (x1 == x2) {
            if (y2 == y1 + direction && board[y2][x2] == nullptr) return true;
            if (startRow && y2 == y1 + 2 * direction && board[y1 + direction][x2] == nullptr && board[y2][x2] == nullptr)
                return true;
        } 
        else if (abs(x2 - x1) == 1 && y2 == y1 + direction) {
            Chesspiece* target = board[y2][x2];
            cout << "Diagonal move check: from (" << x1 << "," << y1 << ") to (" << x2 << "," << y2 << ") ";
            if (target) {
            cout << "| Target: " << target->getName() << ", Team: " << target->getTeam() << ", Own Team: " << t << endl;
            } else {
                cout << "| No piece to capture.\n";
    }
    return target != nullptr && target->getTeam() != t;
}
        else if (abs(x2 - x1) == 1 && y2 == y1 + direction) {
            if (board[y2][x2] != nullptr) return true;
        }

        return false;
}
static bool isvalidrookmove(int x1, int y1, int x2, int y2, Chesspiece* board[8][8], bool t) {
        if (x1 != x2 && y1 != y2) return false;
        return ispathclear(x1, y1, x2, y2, board, t);

        Chesspiece* target = board[y2][x2];
        return target == nullptr || target->getTeam() != t;
    }

    static bool isvalidknightmove(int x1, int y1, int x2, int y2, Chesspiece* board[8][8], bool t) {
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        if (!((dx == 1 && dy == 2) || (dx == 2 && dy == 1))) return false;

        Chesspiece* target = board[y2][x2];
        return target == nullptr || target->getTeam() != t;
    }

    static bool isvalidbishopmove(int x1, int y1, int x2, int y2, Chesspiece* board[8][8], bool t) {
        if (abs(x2 - x1) != abs(y2 - y1)) return false;
        return ispathclear(x1, y1, x2, y2, board, t);

        Chesspiece* target = board[y2][x2];
        return target == nullptr || target->getTeam() != t;
    }

    static bool isvalidqueenmove(int x1, int y1, int x2, int y2, Chesspiece* board[8][8], bool t) {
        return isvalidrookmove(x1, y1, x2, y2, board, t) || isvalidbishopmove(x1, y1, x2, y2, board, t);

        Chesspiece* target = board[y2][x2];
        return target == nullptr || target->getTeam() != t;
    }

    static bool isvalidkingmove(int x1, int y1, int x2, int y2,Chesspiece* board[8][8], bool t) {
        return abs(x2 - x1) <= 1 && abs(y2 - y1) <= 1;

        Chesspiece* target = board[y2][x2];
        return target == nullptr || target->getTeam() != t;
    }
};





#endif