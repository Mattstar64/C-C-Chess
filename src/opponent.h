#include "movement_rules.h"
#include "chess_board.h"
#include <vector>
#include <limits>

#ifndef AIOPPONENT
#define AIOPPONENT


int positional_weights[8][8] = {
    {1, 2, 3, 4, 4, 3, 2, 1},
    {2, 3, 4, 5, 5, 4, 3, 2},
    {3, 4, 5, 6, 6, 5, 4, 3},
    {4, 5, 6, 7, 7, 6, 5, 4},
    {4, 5, 6, 7, 7, 6, 5, 4},
    {3, 4, 5, 6, 6, 5, 4, 3},
    {2, 3, 4, 5, 5, 4, 3, 2},
    {1, 2, 3, 4, 4, 3, 2, 1}
};

int evaluateBoard(Chesspiece* board[8][8]) {
    int score = 0;
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Chesspiece* piece = board[y][x];
            if (!piece) continue;

            int base = piece->getPoint();
            int posBonus = positional_weights[y][x];
            int total = base + posBonus;

            score += (piece->getTeam() == 0 ? total : -total);
        }
    }
    return score;
}


struct Move {
    int fromX, fromY;
    int toX, toY;
};

vector<Move> getlegalmoves(bool team, Chesspiece* board[8][8]) {
    vector<Move> moves;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Chesspiece* piece = board[y][x];
            if (piece && piece->getTeam() == team) {
                for (int ty = 0; ty < 8; ty++) {
                    for (int tx = 0; tx < 8; tx++) {
                        if (Movrules::isvalidmove(piece, x, y, tx, ty, board, team, true)) {
                            moves.push_back({x, y, tx, ty});
                        }
                    }
                }
            }
        }
    }
    return moves;
}

int minimax(Chesspiece* board[8][8], int depth, bool maximizingPlayer, int alpha, int beta) {
    if (depth == 0 || Movrules::ischeckmate(0, board) || Movrules::ischeckmate(1, board)) {
        return evaluateBoard(board);
    }

    int bestValue = maximizingPlayer ? INT_MIN : INT_MAX;

    for (int y1 = 0; y1 < 8; ++y1) {
        for (int x1 = 0; x1 < 8; ++x1) {
            Chesspiece* piece = board[y1][x1];
            if (!piece) continue;
            if (piece->getTeam() != (maximizingPlayer ? 0 : 1)) continue;

            for (int y2 = 0; y2 < 8; ++y2) {
                for (int x2 = 0; x2 < 8; ++x2) {
                    if (!Movrules::isvalidmove(piece, x1, y1, x2, y2, board, piece->getTeam(), true)) continue;

                    Chesspiece* captured = board[y2][x2];
                    board[y2][x2] = piece;
                    board[y1][x1] = nullptr;
                    int oldX = piece->getX(), oldY = piece->getY();
                    piece->setPosition(x2, y2);

                    int eval = minimax(board, depth - 1, !maximizingPlayer, alpha, beta);

                    board[y1][x1] = piece;
                    board[y2][x2] = captured;
                    piece->setPosition(oldX, oldY);

                    if (maximizingPlayer) {
                        bestValue = max(bestValue, eval);
                        alpha = max(alpha, eval);
                    } else {
                        bestValue = min(bestValue, eval);
                        beta = min(beta, eval);
                    }

                    if (beta <= alpha)
                        return bestValue;
                }
            }
        }
    }

    return bestValue;
}

void bestAImove(Chesspiece* board[8][8]) {
    int bestScore = INT_MIN;
    int bestX1 = -1, bestY1 = -1, bestX2 = -1, bestY2 = -1;
    bool foundMove = false;

    for (int y1 = 0; y1 < 8; ++y1) {
        for (int x1 = 0; x1 < 8; ++x1) {
            Chesspiece* piece = board[y1][x1];
            if (!piece || piece->getTeam() != 0) continue;

            for (int y2 = 0; y2 < 8; ++y2) {
                for (int x2 = 0; x2 < 8; ++x2) {
                    if (!Movrules::isvalidmove(piece, x1, y1, x2, y2, board, 0, true)) continue;

                    // Simulate move
                    Chesspiece* captured = board[y2][x2];
                    board[y2][x2] = piece;
                    board[y1][x1] = nullptr;
                    int oldX = piece->getX(), oldY = piece->getY();
                    piece->setPosition(x2, y2);

                    int score = minimax(board, 4, false, INT_MIN, INT_MAX);

                    // Undo move
                    board[y1][x1] = piece;
                    board[y2][x2] = captured;
                    piece->setPosition(oldX, oldY);

                    if (score > bestScore) {
                        bestScore = score;
                        bestX1 = x1; bestY1 = y1;
                        bestX2 = x2; bestY2 = y2;
                        foundMove = true;
                    }
                }
            }
        }
    }

    if (foundMove) {
        cout << "AI moves from (" << bestX1 << ", " << bestY1 << ") to (" << bestX2 << ", " << bestY2 << ")\n";
        changeposition(bestX1, bestY1, bestX2, bestY2, 0);
    } else {
        cout << "AI has no legal moves (Checkmate or Stalemate).\n";
    }
}


#endif