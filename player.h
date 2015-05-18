#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "board.h"
#include "cell.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Board;

class Player{
protected:
    
    Board *b;
    char color;
    std::string status;
    std::vector<pos> pieces;
    
public:
    void updatePieces();
    virtual std::vector<std::string> genMoves(int level);
    void extractMove(std::string ,int, int&,int&, int&, int&);
    std::string int2str(int);
    virtual bool move(int rf=0,int cf=0,int rt=0,int ct=0, char type = 'Q') = 0;
    void setBoard(Board*);
    void setColor(char);
    void setPieces(std::vector<pos>);
    void setStatus(std::string);
    std::string getStatus();
    
    


    
    
};

#endif
