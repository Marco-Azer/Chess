#include "human.h"
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
using namespace std;

bool HumanPlayer::move(int r_from,int c_from,int r_to,int c_to, char type){
    

    if (color!=b->getPieceColor(r_from,c_from )){
        cout<<" Illegal move: your colour is "<<color<<endl;
        cout << b->getPieceColor(r_from,c_from);
        return false;
    }
   
    updatePieces();
    return b->move(r_from, c_from, r_to, c_to, type);
        
}
    

