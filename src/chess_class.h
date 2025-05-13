//includes
#include <string>
#include <iostream>

//retrieve the std
using namespace std;

class Chesspiece {
    public:
    //constructor
    Chesspiece () : name (""), points(0), sprite(""), position_x(0), position_y(0){}
    Chesspiece( string n, int p, string s, int x, int y)
    : name(n), points(p), sprite(s), position_x(x), position_y(y) {}

    // Getters
    string getName() const { return name; }
    int getPoint() const {return points;}
    string getSprite() const { return sprite; }
    int getX() const { return position_x; }
    int getY() const { return position_y; }

    // Setters
    void setName(const  string& n) { name = n; }
    void setPoint(int p) { points = p; }
    void setSprite(const  string& s) { sprite = s; }
    void setPosition(int x, int y) {
        position_x = x;
        position_y = y;
    }

    // display a piece
    void displaypiece(){
        cout << sprite << flush;
    }

    private:
    string name;
    int points;
    string sprite;
    int position_x;
    int position_y;

    
};