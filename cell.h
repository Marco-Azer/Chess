#ifndef __CELL_H__
#define __CELL_H__
#include <vector>
#include "textdisplay.h"

class Board;

struct pos{
int r,c;
pos(int r, int c): r(r), c(c) {}
   
};

class Cell{
std::vector<pos> v;
char type; // Type of piece
char color; // Colour of piece: 'B'- black, 'W'- white, 'E'- empty
char cellColor;// Colour of cell: 'B' or 'W'
int r, x; // Row
int c, y; // Column
int attack_w;// Number of white pieces attaking the cell
int attack_b;// Number of black pieces attaking the cell

public:
    
Cell();
    //Pre: True
    //Post: Construct a cell
void setType(char);
    //Pre: Char
    //Post: Set the type of the cell equal to the passed in char
void setCellColor(char);
    //Pre: Char
    //Posot: Set the color of the cell to the passed in char
void setColor(char);
    //Pre: Char
    //Post: Set the color of the piece in the cell to the passed in char
    void setCoords(int, int,int,int, Xwindow*);
    //Pre: Two Integers
    //Post: Set the coordinates of the cell to the passed in Integers
void notifyDisplay(TextDisplay&);
    //Pre: TextDisplay Address
    //Post: Notify the text display with the changes
void notifyNoAttack();
    //Pre: True
    //Post: The cell is not attacked
void notifyAttack(char);
    //Pre: Char
    //Post: Notify the cell that it is under attack by 'W' or 'B'
    char getColor();
    //Pre: True
    //Post: Return the color of the piece in the cell, 'E' if the the cell is empty
    char getType();
    //Pre: True
    //Post: Return the type of the piece in the cell, 'E' if the cell is empty
    int getAttack(char);
    //Pre: Char
    //Post: Return the number of attacks on the cell.
    //Example: char = 'B', it returns attack_b
    std::vector<pos> getMove();
    //Pre: True
    //Post: Return a vector of the available moves of the cell
void addMove(int,int);
    //Pre: Two Integers
    //Post: Add the passed in coordinates to the vector of the available moves of the cell
void clearMove();
    //Pre: True
    //Post: Clear the vector of the available moves of the cell
    void draw();
    //Pre: True
    //Post: Draw the cell with piece/empty


};

#endif
