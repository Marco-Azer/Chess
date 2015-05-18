#include "textdisplay.h"
#include <iostream>
#include <sstream>
using namespace std;

//----------ctor---------------------
TextDisplay::TextDisplay(){
    
    theDisplay=new char*[8];//visual repres. of board
    for(int i=0; i<8; i++){
        theDisplay[i]=new char[8];
        for (int j=0; j<8; j++) {
            theDisplay[i][j]=' ';  //by default each cell is black
        }
    }
}

//-------------dtor------------------

TextDisplay::~TextDisplay(){
    for(int i=0; i<8; i++){
        delete []theDisplay[i];
    }
    delete [] theDisplay;

}

//---------update repres. based on cell's type---------
void TextDisplay::notify(int r, int c, char type, char cellColour ){
    if (type=='E'){ 
	    if (cellColour=='B')
		     theDisplay[r][c]='_';
        else
		     theDisplay[r][c]=' ';
    }
    else {
	theDisplay[r][c]=type;
    }	
}


//---------------overload <<--------------------
ostream &operator<<(ostream &out, const TextDisplay &td){
    cout<<endl;
    int row=8;
    for(int i=0; i<8; i++){
        cout<<" "<<row<<" ";
        row=row-1;
        for(int j=0; j<8; j++){
            out<<td.theDisplay[i][j];//"output" repres. of each cell
        }
        out<<endl;
    }
    cout<<"   abcdefgh"<<endl;
    cout<<endl;
    return out;
}
