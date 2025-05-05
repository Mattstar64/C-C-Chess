//includes
#include <string>
#include <iostream>

//retrieve the std
using namespace std;

class Chesspiece {
    public:
    //constructor
    Chesspiece () : name (""), points(0), sprite(""), position_x(0), position_y(0),{}
    Chesspiece( string n, int points, string s, int x, int y){}

    private:
    string name;
    int points;
    string sprite;
    int position_x;
    int position_y;

    void displaypiece(){
        cout << sprite;
    }
};