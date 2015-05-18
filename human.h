#ifndef __HUMANPLAYER_H__
#define __HUMANPLAYER_H__

#include "player.h"
#include <string>



class HumanPlayer:public Player{
    
public:
    bool move(int,int,int,int,char type ='Q');
    
};

#endif
