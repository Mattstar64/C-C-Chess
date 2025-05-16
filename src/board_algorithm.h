#include "chess_class.h"
#include <vector>


using namespace std;

//definitions
#ifndef PIECE_MANAGER_H
#define PIECE_MANAGER_H

class PieceManager {
private:
    vector<Chesspiece*> pieces;

public:
    //add a ppiece
    void addpiece(Chesspiece* piece) {
        pieces.push_back(piece);
    }
    //remove a piece
    bool removepiece(const string& name) {
        for (auto it = pieces.begin(); it != pieces.end(); ++it) {
            if ((*it)->getName() == name) {
                delete *it;
                pieces.erase(it);
                return true;
            }
        }
        return false;
    }

    ~PieceManager() {
        for (Chesspiece* piece : pieces) {
            delete piece;
        }
    }
};

#endif
