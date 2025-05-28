#ifndef AIOPPONENT
#define AIOPPONENT

#include "movement_rules.h"
#include "chess_board.h"
#include <vector>
#include <limits>
#include <algorithm>

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

struct ScoredMove {
    Move move;
    int score;
};


vector<ScoredMove> getAllMoves(Chesspiece* board[8][8], bool team) {
    vector<ScoredMove> moves;

    for (int y1 = 0; y1 < 8; ++y1) {
        for (int x1 = 0; x1 < 8; ++x1) {
            Chesspiece* piece = board[y1][x1];
            if (!piece || piece->getTeam() != team) continue;

            for (int y2 = 0; y2 < 8; ++y2) {
                for (int x2 = 0; x2 < 8; ++x2) {
                    if (!Movrules::isvalidmove(piece, x1, y1, x2, y2, board, team, true, piece->hasmoved())) continue;

                    int moveScore = 0;
                    if (board[y2][x2]) {
                        moveScore += board[y2][x2]->getPoint(); // Capturing is good
                    }
                    if (piece->getName() == "Pawn" && (y2 == 0 || y2 == 7)) {
                        moveScore += 900; // Promotion
                    }

                    moves.push_back({Move{x1, y1, x2, y2}, moveScore});
                }
            }
        }
    }

    sort(moves.begin(), moves.end(), [](const ScoredMove& a, const ScoredMove& b) {
        return a.score > b.score;
    });

    return moves;
}

int minimax(Chesspiece* board[8][8], int depth, bool maximizingPlayer, int alpha, int beta) {
    if (depth == 0 || Movrules::ischeckmate(0, board) || Movrules::ischeckmate(1, board)) {
        return evaluateBoard(board);
    }

    vector<ScoredMove> moves = getAllMoves(board, maximizingPlayer ? 0 : 1);

    int bestValue = maximizingPlayer ? INT_MIN : INT_MAX;

    for (const ScoredMove& sm : moves) {
        const Move& move = sm.move;

        Chesspiece* piece = board[move.fromY][move.fromX];
        Chesspiece* captured = board[move.toY][move.toX];
        bool hasMoved = piece->hasmoved();

        board[move.toY][move.toX] = piece;
        board[move.fromY][move.fromX] = nullptr;
        int oldX = piece->getX(), oldY = piece->getY();
        piece->setPosition(move.toX, move.toY);

        bool promote = piece->getName() == "Pawn" && ((piece->getTeam() == 0 && move.toY == 0) || (piece->getTeam() == 1 && move.toY == 7));
        Chesspiece* promoted = nullptr;
        Chesspiece* originalPawn = nullptr;

        if (promote) {
            originalPawn = piece;
            if (board[move.toY][move.toX] != piece) delete board[move.toY][move.toX];
            promoted = createPiece(5, move.toX, move.toY, piece->getTeam(), true);
            board[move.toY][move.toX] = promoted;
            piece = promoted;
        }

        int eval = minimax(board, depth - 1, !maximizingPlayer, alpha, beta);

        if (promoted) {
            delete board[move.toY][move.toX];
            board[move.toY][move.toX] = captured;
            board[move.fromY][move.fromX] = originalPawn;
            originalPawn->setPosition(oldX, oldY);
        } else {
            board[move.fromY][move.fromX] = piece;
            board[move.toY][move.toX] = captured;
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
            break;
    }

    return bestValue;
}

void bestAImove(Chesspiece* board[8][8]) {
    int bestScore = INT_MIN;
    Move bestMove = {-1, -1, -1, -1};

    vector<ScoredMove> moves = getAllMoves(board, 0);

    for (const ScoredMove& sm : moves) {
        const Move& move = sm.move;
        Chesspiece* piece = board[move.fromY][move.fromX];
        Chesspiece* captured = board[move.toY][move.toX];
        bool hasMoved = piece->hasmoved();

        board[move.toY][move.toX] = piece;
        board[move.fromY][move.fromX] = nullptr;
        int oldX = piece->getX(), oldY = piece->getY();
        piece->setPosition(move.toX, move.toY);

        bool promote = piece->getName() == "Pawn" && move.toY == 0;
        Chesspiece* promoted = nullptr;
        Chesspiece* originalPawn = nullptr;

        if (promote) {
            originalPawn = piece;
            if (board[move.toY][move.toX] != piece) delete board[move.toY][move.toX];
            promoted = createPiece(5, move.toX, move.toY, piece->getTeam(), true);
            board[move.toY][move.toX] = promoted;
            piece = promoted;
        }

        int score = minimax(board, 6, false, INT_MIN, INT_MAX);

        if (promoted) {
            delete board[move.toY][move.toX];
            board[move.toY][move.toX] = captured;
            board[move.fromY][move.fromX] = originalPawn;
            originalPawn->setPosition(oldX, oldY);
        } else {
            board[move.fromY][move.fromX] = piece;
            board[move.toY][move.toX] = captured;
            piece->setPosition(oldX, oldY);
        }

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }

    if (bestMove.fromX != -1) {
        cout << "AI moves from (" << bestMove.fromX << ", " << bestMove.fromY << ") to (" << bestMove.toX << ", " << bestMove.toY << ")\n";
        changeposition(bestMove.fromX, bestMove.fromY, bestMove.toX, bestMove.toY, 0, true);
    } else {
        cout << "AI has no legal moves (Checkmate or Stalemate).\n";
    }
}

#endif