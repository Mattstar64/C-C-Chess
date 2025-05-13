#include "board_algorithm.h"
#include <iostream>
using namespace std;

int board[] {4,2,3,5,6,3,2,4,
             1,1,1,1,1,1,1,1,
             0,0,0,0,0,0,0,0,
             0,0,0,0,0,0,0,0,
             0,0,0,0,0,0,0,0,
             0,0,0,0,0,0,0,0,
             1,1,1,1,1,1,1,1,
             4,2,3,5,6,3,2,4,};


// b n + 1  = pos p 
// a n + 1 = 1 N + 1 
// t n + 1 i+ 1 k 
// inverse pos n pos  couleur change 
//create the different pieces on the board
void setpiece(int index, int i, int y){
    PieceManager manager;
    Chesspiece piece;
    switch (index){
        case 0:
            piece =Chesspiece("None",0,"   ", i, y);
            break;
        case 1:
            
             piece =Chesspiece("Pawn",1," P ", i, y);
            break;
        case 2:
             piece = Chesspiece("Knight",3," C ", i, y);
            break;
        case 3:
             piece =Chesspiece("Bishop",3," B ", i, y);
            break;
        case 4:
             piece = Chesspiece("Rook",5," R ", i, y);
            break;
        case 5:
             piece =Chesspiece("Queen",9," Q ", i, y);
            break;
        case 6:
             piece =Chesspiece("King",0," K ", i, y);
            break;
    }

    
    manager.addpiece(piece);
    
    piece.displaypiece();
}



void setboard(){
    cout << "setboard is running!" << endl;
    int index = 0;
    int x=0;
    int y=0;
    int i = 0;
    string sprite;

    for (y = 0; y <= 7;y++){
        for (x = 0; x <= 7;x++){
            index = board[i];
            setpiece(index,i,y);
            i++;    
        }
        cout<< "\n" << flush;
        
    }
};


