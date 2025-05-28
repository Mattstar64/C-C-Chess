#ifndef AIOPPONENT
#define AIOPPONENT

#include "movement_rules.h"
#include "chess_board.h"
#include <vector>
#include <limits>

using namespace std;

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
    bool isCastle = false;
};

int minimax(Chesspiece* board[8][8], int depth, bool maximizingPlayer, int alpha, int beta) {
    if (depth == 0 || Movrules::ischeckmate(0, board) || Movrules::ischeckmate(1, board)) {
        return evaluateBoard(board);
    }

    int bestValue = maximizingPlayer ? INT_MIN : INT_MAX;

    for (int y1 = 0; y1 < 8; ++y1) {
        for (int x1 = 0; x1 < 8; ++x1) {
            Chesspiece* piece = board[y1][x1];
            if (!piece || piece->getTeam() != (maximizingPlayer ? 0 : 1)) continue;
            bool m = piece->hasmoved();

            for (int y2 = 0; y2 < 8; ++y2) {
                for (int x2 = 0; x2 < 8; ++x2) {
                    if (!Movrules::isvalidmove(piece, x1, y1, x2, y2, board, piece->getTeam(), true, m)) continue;

                    Chesspiece* captured = board[y2][x2];
                    board[y2][x2] = piece;
                    board[y1][x1] = nullptr;
                    int oldX = piece->getX(), oldY = piece->getY();
                    piece->setPosition(x2, y2);

                    bool promote = piece->getName() == "Pawn" && ((piece->getTeam() == 0 && y2 == 0) || (piece->getTeam() == 1 && y2 == 7));
                    Chesspiece* promoted = nullptr;
                    Chesspiece* originalPawn = nullptr;

                    if (promote) {
                        originalPawn = piece;
                        if (board[y2][x2] != piece) {
                            delete board[y2][x2];
                        }
                        promoted = createPiece(5, x2, y2, piece->getTeam(), true);
                        board[y2][x2] = promoted;
                        piece = promoted;
                    }

                    int eval = minimax(board, depth - 1, !maximizingPlayer, alpha, beta);

                    if (promoted) {
                        delete board[y2][x2];
                        board[y2][x2] = captured;
                        board[y1][x1] = originalPawn;
                        originalPawn->setPosition(oldX, oldY);
                    } else {
                        board[y1][x1] = piece;
                        board[y2][x2] = captured;
                        piece->setPosition(oldX, oldY);
                    }

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
    Move bestMove = {-1, -1, -1, -1};

    for (int y1 = 0; y1 < 8; ++y1) {
        for (int x1 = 0; x1 < 8; ++x1) {
            Chesspiece* piece = board[y1][x1];
            if (!piece || piece->getTeam() != 0) continue;

            bool m = piece->hasmoved();

            for (int y2 = 0; y2 < 8; ++y2) {
                for (int x2 = 0; x2 < 8; ++x2) {
                    if (!Movrules::isvalidmove(piece, x1, y1, x2, y2, board, 0, true, m)) continue;

                    Chesspiece* captured = board[y2][x2];
                    board[y2][x2] = piece;
                    board[y1][x1] = nullptr;
                    int oldX = piece->getX(), oldY = piece->getY();
                    piece->setPosition(x2, y2);

                    bool promote = piece->getName() == "Pawn" && y2 == 0;
                    Chesspiece* promoted = nullptr;
                    Chesspiece* originalPawn = nullptr;

                    if (promote) {
                        originalPawn = piece;
                        if (board[y2][x2] != piece) {
                            delete board[y2][x2];
                        }
                        promoted = createPiece(5, x2, y2, piece->getTeam(), true);
                        board[y2][x2] = promoted;
                        piece = promoted;
                    }

                    int score = minimax(board, 6, false, INT_MIN, INT_MAX);

                    if (promoted) {
                        delete board[y2][x2];
                        board[y2][x2] = captured;
                        board[y1][x1] = originalPawn;
                        originalPawn->setPosition(oldX, oldY);
                    } else {
                        board[y1][x1] = piece;
                        board[y2][x2] = captured;
                        piece->setPosition(oldX, oldY);
                    }

                    if (score > bestScore) {
                        bestScore = score;
                        bestMove = {x1, y1, x2, y2};
                    }
                }
            }

            if (piece->getName() == "King" && !m) {
                for (bool kingside : {true, false}) {
                    int x2 = kingside ? 6 : 2;
                    if (Movrules::cancastle(board, x1, y1, x2, y1, kingside, 0)) {
                        Movrules::performcastle(board, x1, y1, x2, y1, kingside);
                        int score = minimax(board, 3, false, INT_MIN, INT_MAX);

                        board[y1][x1] = board[y1][x2];
                        board[y1][x1]->setPosition(x1, y1);
                        board[y1][x1]->setmoved(false);
                        int rookX = kingside ? 5 : 3;
                        int originalRookX = kingside ? 7 : 0;
                        board[y1][originalRookX] = board[y1][rookX];
                        board[y1][originalRookX]->setPosition(originalRookX, y1);
                        board[y1][originalRookX]->setmoved(false);
                        board[y1][x2] = nullptr;
                        board[y1][rookX] = nullptr;

                        if (score > bestScore) {
                            bestScore = score;
                            bestMove = {x1, y1, x2, y1, true};
                        }
                    }
                }
            }
        }
    }

    if (bestMove.fromX != -1) {
        if (bestMove.isCastle) {
            bool kingside = Movrules::lookkingside(bestMove.fromX, bestMove.toX);
            Movrules::performcastle(board, bestMove.fromX, bestMove.fromY, bestMove.toX, bestMove.toY, kingside);
            cout << "AI performs castling\n";
        } else {
            cout << "AI moves from (" << bestMove.fromX << ", " << bestMove.fromY << ") to (" << bestMove.toX << ", " << bestMove.toY << ")\n";
            changeposition(bestMove.fromX, bestMove.fromY, bestMove.toX, bestMove.toY, 0, true);
        }
    } else {
        cout << "AI has no legal moves (Checkmate or Stalemate).\n";
    }
}

#endif