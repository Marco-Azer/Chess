#include <iostream>
#include <vector>
#include "cell.h"
#include "board.h"
#include <sstream>
#include <string>
using namespace std;

Cell::Cell(): attack_w(0),attack_b(0){}

void Cell::setType(char c){
    type = c;
    
}

void Cell::setCellColor(char cl) {
    cellColor = cl;

}

void Cell::setColor(char cl) {
    color = cl;
}

void Cell::setCoords(int r, int c, int x, int y, Xwindow *w) {
    this->r = r;
    this->c = c;
    this->x = x;
    this->y = y;
    this->w = w;
    
}

char Cell::getColor() {
    return color;
}

char Cell::getType() {
    return type;
    
}

void Cell::notifyDisplay(TextDisplay& td) {
    td.notify(r,c,type,cellColor);
}

void Cell::addMove(int r, int c) {
v.push_back(pos(r,c));
}

vector<pos> Cell::getMove() {
return v;
}

void Cell::clearMove() {
v.clear();
}

void Cell::notifyAttack(char col) {
    if (col == 'W') {
        attack_w++;
    }
   if (col =='B') {
        attack_b++;
    }
 
}

void Cell::notifyNoAttack() {
        attack_b = 0;
        attack_w = 0;
}

int Cell::getAttack(char col){
    if (col == 'W') {
        return attack_b;
    }
    else {
       return attack_w;
    }
}

