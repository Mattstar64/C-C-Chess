#include "chess_class.h"
#include <vector>


using namespace std;

//definitions
#ifndef PIECE_MANAGER_H
#define PIECE_MANAGER_H


//
class PieceManager {
private:
    vector<Chesspiece> pieces;

public:
    // Add piece
    void addpiece(const Chesspiece& piece) {
        pieces.push_back(piece);
    }


    // Remove piece by name
    bool removepiece(const string& name) {
        for (auto it = pieces.begin(); it != pieces.end(); ++it) { //"auto" lets the compiler deduce the type of the element in the vector, which is piece
            if (it->getName() == name) {
                pieces.erase(it);
                return true;
            }
        }
        return false; // //Will return an error message if the piece is not found
    }

    
};

#endif