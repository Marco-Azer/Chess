#include "player.h"
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

void Player::setStatus(string status){
    this->status=status;
    
}
string Player::getStatus(){
    
    return status;
}

void Player::setBoard(Board* b){
    this->b=b;
}

void Player::setColor(char color){
    this->color=color;
}

void Player::setPieces(std::vector<pos> pieces){
    this->pieces=pieces;
}

void Player::updatePieces(){
    pieces=b->getPieces(color);
}

std::string Player::int2str(int i){
    std::stringstream ss;
    ss << i;
    return ss.str();
}

void Player::extractMove(std::string move,int level, int &r_from,int &c_from, int &r_to, int &c_to){
    if (level==1){
        
        r_from = move[0]-48;
        c_from = move[1]-48;
        r_to = move[2]-48;
        c_to = move[3]-48;
    }
    else if (level==2){
        r_from = move[2]-48;
        c_from = move[3]-48;
        r_to =  move[4]-48;
        c_to = move[5]-48;
        
    }
    else{
        r_from = move[3]-48;
        c_from = move[4]-48;
        r_to = move[5]-48;
        c_to = move[6]-48;
    }
    
}


bool inMap(string move,int val, map<string,int>  &Map){
    
    pair<map<string,int>::iterator,bool> ret;
    ret= Map.insert(pair<string, int>(move,val));
    
    
    return ret.second;
    
}

vector<string> Player::genMoves(int level){
    map<string,int> Map;
    string move, r_from,c_from, r_to, c_to;
    vector<string> moves;
    vector<pos> pieceMoves;
    char type;
    string safeCode,attackCode,checkCode;
    bool myCheck, enemyCheck;
    
    updatePieces();
    for (unsigned int i=0; i<pieces.size(); i++){
        r_from = int2str(pieces[i].r);
        c_from = int2str(pieces[i].c);
        
        
        pieceMoves = b->getPieceMove(pieces[i].r,pieces[i].c);

        for (unsigned int j=0; j< pieceMoves.size(); j++){
            b->testMove(pieces[i].r,pieces[i].c,pieceMoves[j].r,pieceMoves[j].c,myCheck, enemyCheck);
            

            if (myCheck!=true){
                
                /* L2+ */       if (enemyCheck){
                    checkCode="0"; // performing check
                }
                else{
                    checkCode="1";// not performing check
                }
                
                type = b->getCellType(pieceMoves[j].r,pieceMoves[j].c);
                /* L2+ */       if(type!='E'){
                    if ((type=='Q')||(type=='q')){
                        attackCode = "0"; // performong capture of queen
                    }
                    if ((type=='R')||(type=='r')){
                        attackCode = "1"; // performong capture of rook
                    }
                    if ((type=='B')||(type=='b')){
                        attackCode = "2"; // performong capture of bishop
                    }
                    if ((type=='N')||(type=='n')){
                        attackCode = "3"; // performong capture of knight
                    }
                    if ((type=='P')||(type=='p')){
                        attackCode = "4"; // performong capture of pawn
                    }
                    
                }
                else{
                    attackCode = "5"; // not performing capture
                }
                
                
                /* L3+ */       if(b->getPieceAttack(pieceMoves[j].r,pieceMoves[j].c,color)>0){
                    safeCode="1"; // under attack
                    
                }
                else{
                    safeCode="0";// not under attack
                }
                
                if (level == 1){
                    safeCode="";
                    checkCode="";
                    attackCode="";
                }
                if (level == 2){
                    safeCode="";
                    
                }
                if (level==3){
                    if (attackCode!="0"){
                        attackCode="1";
                    }
                }
                
                r_to = int2str(pieceMoves[j].r);
                c_to = int2str(pieceMoves[j].c);
                move = safeCode + checkCode + attackCode + r_from + c_from + r_to + c_to;
                
                if (!inMap(move,0,Map)){
                    moves.push_back(move);
                }
            }
            
        }
        
        
    }
    return moves;
    
    
}

