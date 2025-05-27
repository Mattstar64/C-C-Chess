//includes
#include <string>
#include <iostream>

//retrieve the std
using namespace std;

#ifndef CHESSCLASS
#define CHESSCLASS

class Chesspiece {
    public:
    //constructor
    Chesspiece () : name (""), points(0), sprite(""), position_x(0), position_y(0), team(0), moved(0){}
    Chesspiece( string n, int p, string s, int x, int y, bool t, bool m)
    : name(n), points(p), sprite(s), position_x(x), position_y(y), team(t), moved(m){}

    // Getters
    string getName() const { return name; }
    int getPoint() const {return points;}
    string getSprite() const { return sprite; }
    int getX() const { return position_x; }
    int getY() const { return position_y; }
    bool getTeam() const {return team;}
    bool hasmoved() const { return moved; }

    // Setters
    void setName(const  string& n) { name = n; }
    void setPoint(int p) { points = p; }
    void setSprite(const  string& s) { sprite = s; }
    void setPosition(int x, int y) {
        position_x = x;
        position_y = y;
    }
    void setTeam(bool t) {team = t;}
    void setmoved(bool m) { moved = m; }

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
    bool team;
    bool moved;

    
};

#endif