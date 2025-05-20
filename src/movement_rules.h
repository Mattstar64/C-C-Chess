#include "chess_class.h"
#include <iostream>

using namespace std;

#ifndef MOVEMENTRULES
#define MOVEMENTRULES

class Movrules {
    public:
        static bool isvalidmove(Chesspiece* piece, int x1, int y1, int x2, int y2, Chesspiece* board[8][8], bool t, bool silent) {
    
    if (x1 < 0 || x1 > 7 || y1 < 0 || y1 > 7 || x2 < 0 || x2 > 7 || y2 < 0 || y2 > 7)
        return false;

    
    if (!piece) return false;

    Chesspiece* target = board[y2][x2];
    if (target && target->getTeam() == t) {
        if (!silent) std::cout << "Cannot capture own piece!" << std::endl;
        return false;
    }

    // 4. Validate based on piece type
    string name = piece->getName();
    bool valid = false;

    if (name == "Pawn")   valid = isvalidpawnmove(piece, x1, y1, x2, y2, board, t);
    else if (name == "Rook")   valid = isvalidrookmove(x1, y1, x2, y2, board, t);
    else if (name == "Knight") valid = isvalidknightmove(x1, y1, x2, y2, board, t);
    else if (name == "Bishop") valid = isvalidbishopmove(x1, y1, x2, y2, board, t);
    else if (name == "Queen")  valid = isvalidqueenmove(x1, y1, x2, y2, board, t);
    else if (name == "King")   valid = isvalidkingmove(x1, y1, x2, y2, board, t);

    if (!valid) return false;

    Chesspiece* captured = board[y2][x2];
    board[y2][x2] = piece;
    board[y1][x1] = nullptr;
    int oldX = piece->getX(), oldY = piece->getY();
    piece->setPosition(x2, y2);

    bool inCheck = iskingincheck(t, board);

    board[y1][x1] = piece;
    board[y2][x2] = captured;
    piece->setPosition(oldX, oldY);

    if (inCheck) {
        if (!silent) std::cout << "Move would put king in check!" << std::endl;
        return false;
    }

    return true;
}

        static bool iskingincheck(bool team, Chesspiece* board[8][8]) {
            int kingX, kingY;

            for (int y = 0; y < 8; y++) {
                for (int x = 0; x < 8; x++) {
                    Chesspiece* piece = board[y][x];
                    if (piece && piece->getName() == "King" && piece->getTeam() == team) {
                        kingX = x;
                        kingY = y;
                    }
                }
            }

            for (int y = 0; y < 8; y++) {
                for (int x = 0; x < 8; x++) {
                    Chesspiece* attacker = board[y][x];
                    if (attacker && attacker->getTeam() != team) {
                        if (Movrules::isvalidmove(attacker, x, y, kingX, kingY, board, attacker->getTeam(), true)) {
                            return true;
                        }
                    }
                }
            }
            
        return false;
        };
        static bool ischeckmate(bool team, Chesspiece* board[8][8]) {
            if (!iskingincheck(team, board)) return false;

            for (int y1 = 0; y1 < 8; ++y1) {
                for (int x1 = 0; x1 < 8; ++x1) {
                    Chesspiece* piece = board[y1][x1];
                    if (!piece || piece->getTeam() != team) continue;

                    for (int y2 = 0; y2 < 8; ++y2) {
                        for (int x2 = 0; x2 < 8; ++x2) {
                            if (!isvalidmove(piece, x1, y1, x2, y2, board, team, true)) continue;

                            Chesspiece* captured = board[y2][x2];
                            board[y2][x2] = piece;
                            board[y1][x1] = nullptr;
                            int oldX = piece->getX(), oldY = piece->getY();
                            piece->setPosition(x2, y2);

                            bool stillInCheck = iskingincheck(team, board);

                            board[y1][x1] = piece;
                            board[y2][x2] = captured;
                            piece->setPosition(oldX, oldY);

                            if (!stillInCheck) return false;
                        }
                    }
                }
            }

            return true; // No escape
        }

    private:
    static bool ispathclear(int x1, int y1, int x2, int y2, Chesspiece* board[8][8], bool t) {
    int dx = (x2 - x1) == 0 ? 0 : (x2 - x1) / abs(x2 - x1);
    int dy = (y2 - y1) == 0 ? 0 : (y2 - y1) / abs(y2 - y1);
    x1 += dx;
    y1 += dy;

    while (x1 != x2 || y1 != y2) {
        if (board[y1][x1] != nullptr) {
            return false;
        }
        x1 += dx;
        y1 += dy;
    }

    return true;
}

    static bool isvalidpawnmove(Chesspiece* piece, int x1, int y1, int x2, int y2, Chesspiece* board[8][8], bool t) {
    int direction = t ? -1 : 1;
    bool startRow = (direction == 1 && y1 == 1) || (direction == -1 && y1 == 6);

    if (x1 == x2) {
        if (y2 == y1 + direction && board[y2][x2] == nullptr)
            return true;
        if (startRow && y2 == y1 + 2 * direction && board[y1 + direction][x2] == nullptr && board[y2][x2] == nullptr)
            return true;
    } 
    
    else if (abs(x2 - x1) == 1 && y2 == y1 + direction) {
        Chesspiece* target = board[y2][x2];
        return target != nullptr && target->getTeam() != t;
    }
    
    return false;
}

    static bool isvalidrookmove(int x1, int y1, int x2, int y2, Chesspiece* board[8][8], bool t) {
        if (x1 != x2 && y1 != y2) return false;
        if (!ispathclear(x1, y1, x2, y2, board, t)) return false;

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
        if (!ispathclear(x1, y1, x2, y2, board, t)) return false;

        Chesspiece* target = board[y2][x2];
        return target == nullptr || target->getTeam() != t;
    }


    static bool isvalidqueenmove(int x1, int y1, int x2, int y2, Chesspiece* board[8][8], bool t) {
        bool valid = false;
        if (x1 == x2 || y1 == y2) valid = isvalidrookmove(x1, y1, x2, y2, board, t);
        else if (abs(x2 - x1) == abs(y2 - y1)) valid = isvalidbishopmove(x1, y1, x2, y2, board, t);
        return valid;
    }


    static bool isvalidkingmove(int x1, int y1, int x2, int y2, Chesspiece* board[8][8], bool t) {

       if (abs(x2 - x1) > 1 || abs(y2 - y1) > 1) return false;
        
        Chesspiece* target = board[y2][x2];
        
        return target == nullptr || target->getTeam() != t;
    }
};





#endif