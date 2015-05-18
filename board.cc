#include <iostream>
#include <vector>
#include "board.h"
#include "cell.h"
#include "textdisplay.h"

using namespace std;

//---------------ctor--------------------------------------------
Board::Board(): theBoard(NULL),checkW(false), checkB(false), sCastlingW(true), bCastlingW(true), sCastlingB(true), bCastlingB(true), checkmateW(false), checkmateB(false), history(-1,-1), en_passantW(false), en_passantB(false)) {}

//---------------dtor---------------------------------------------
Board::~Board(){
    for(int i=0; i<8; i++){
        delete []theBoard[i];
    }
    delete [] theBoard;
    delete td;
    delete w;
}

//--------------initTheBoard-------------------------------------------------
// Set the coordinates of the board
// Set the color of the cell
// Set the chess pieces to default

void Board::initTheBoard() {
	td = new TextDisplay();
	theBoard = new Cell*[8];
	for(int i = 0; i < 8; i++) {
		theBoard[i] = new Cell[8];
		for (int j = 0; j < 8; j++) {
			theBoard[i][j].setCoords(i,j,j*50 + 50, i*50 +50,w); // Set the coords for each cell
 			theBoard[i][j].setColor('E'); // Set the color of the piece at that cell, which is empty for now
            
		}
	}
	char startC = 'W'; // The color to start with
	for (int i = 0; i < 8; i++) { // Set the cell colour
		if (i%2!=0) {
			startC = 'B';
		}
		else {
			startC = 'W';
		}
		char color = startC;
		for (int j = 0; j < 8; j++) {
			theBoard[i][j].setCellColor(color);
            theBoard[i][j].setType('E'); // Set the type of the cell, Empty fo r now
			if (color == 'B'){
				color = 'W';
			}
			else {
				color = 'B';
			}
		}
	}
    for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			theBoard[i][j].notifyDisplay(*td);
            theBoard[i][j].draw();
		}
	}
    
}


//--------------------init-----------------------------------
// Set the chess pieces on the board

void Board::init(char color = 'W') {
    string row_name[8]={"a","b","c","d","e","f","g","h"};
    string col_name[8]={"8","7","6","5","4","3","2","1"};
    
    currentPlayer = color;
	for (int j = 0; j < 8; j++) { // Set the Black Pawns
		theBoard[1][j].setType('p');
		theBoard[1][j].setColor('B');
	}
	for (int j = 0; j < 8; j++) { // Set the White Pawns
		theBoard[6][j].setType('P');
		theBoard[6][j].setColor('W');
	}
    
	// Set the Black Rook
	theBoard[0][0].setType('r');
	theBoard[0][0].setColor('B');
	theBoard[0][7].setType('r');
	theBoard[0][7].setColor('B');
    
	//Set the White Rook
	theBoard[7][0].setType('R');
    theBoard[7][0].setColor('W');
    theBoard[7][7].setType('R');
    theBoard[7][7].setColor('W');
    
	// Set the Black Knight
	theBoard[0][1].setType('n');
    theBoard[0][1].setColor('B');
    theBoard[0][6].setType('n');
    theBoard[0][6].setColor('B');
    
	// Set the White Knight
	theBoard[7][1].setType('N');
    theBoard[7][1].setColor('W');
    theBoard[7][6].setType('N');
    theBoard[7][6].setColor('W');
	
	// Set the Black Bishop
	theBoard[0][2].setType('b');
    theBoard[0][2].setColor('B');
    theBoard[0][5].setType('b');
    theBoard[0][5].setColor('B');
    
	// Set the White Bishop
	theBoard[7][2].setType('B');
    theBoard[7][2].setColor('W');
    theBoard[7][5].setType('B');
    theBoard[7][5].setColor('W');
    
	// Set the Black Queen
	theBoard[0][3].setType('q');
    theBoard[0][3].setColor('B');
    
	// Set the White Queen
	theBoard[7][3].setType('Q');
    theBoard[7][3].setColor('W');
    
	// Set the Black King
	theBoard[0][4].setType('k');
    theBoard[0][4].setColor('B');
    BlackKingCol = 4;
    BlackKingRow = 0;
    
	// Set the White King
	theBoard[7][4].setType('K');
    theBoard[7][4].setColor('W');
    WhiteKingCol = 4;
    WhiteKingRow = 7;
	
    attackNotification();
    
    
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			theBoard[i][j].notifyDisplay(*td);
            theBoard[i][j].draw();
		}
	}
    
}


//--------------------operator<<-----------------------------------------
//Print the board's configuration

ostream &operator<<(ostream &out, const Board &b){
    out<< *(b.td);
    return out;
}

//-----------------------cmpClr----------------------------------
// Compare colors

bool cmpClr(char clr1, char clr2) {
	return clr1 == clr2;
}

//--------------------------moveKing---------------------------------------
// Saves the possible moves of the King in a vector

void Board::moveKing(int r, int c) {
	char color = theBoard[r][c].getColor();
    if (r-1 >= 0) {
        theBoard[r-1][c].notifyAttack(color);
        if (!cmpClr(color,theBoard[r-1][c].getColor()) && theBoard[r-1][c].getAttack(color) == 0) {
            theBoard[r][c].addMove(r-1,c);
        }
    }
	if (r-1 >= 0 && c+1 < 8) {
        theBoard[r-1][c+1].notifyAttack(color);
        if (!cmpClr(color,theBoard[r-1][c+1].getColor()) && theBoard[r-1][c+1].getAttack(color) == 0) {
            theBoard[r][c].addMove(r-1,c+1);
        }
    }
	if(c+1 < 8) {
        theBoard[r][c+1].notifyAttack(color);
        if (!cmpClr(color,theBoard[r][c+1].getColor()) && theBoard[r][c+1].getAttack(color) == 0) {
            theBoard[r][c].addMove(r,c+1);
        }
	}
	if (r+1 < 8 && c+1 < 8) {
        theBoard[r+1][c+1].notifyAttack(color);
        if (!cmpClr(color,theBoard[r+1][c+1].getColor()) && theBoard[r+1][c+1].getAttack(color) == 0) {
            theBoard[r][c].addMove(r+1,c+1);
        }
    }
	if (r+1 < 8) {
		theBoard[r+1][c].notifyAttack(color);
        if (!cmpClr(color,theBoard[r+1][c].getColor()) && theBoard[r+1][c].getAttack(color) == 0) {
            theBoard[r][c].addMove(r+1,c);
        }
	}
	if (r+1 < 8 && c-1 >= 0) {
        theBoard[r+1][c-1].notifyAttack(color);
        if (!cmpClr(color,theBoard[r+1][c-1].getColor()) && theBoard[r+1][c-1].getAttack(color) == 0) {
            theBoard[r][c].addMove(r+1,c-1);
        }
    }
	if (c-1 >= 0) {
		theBoard[r][c-1].notifyAttack(color);
        if (!cmpClr(color,theBoard[r][c-1].getColor()) && theBoard[r][c-1].getAttack(color) == 0) {
            theBoard[r][c].addMove(r,c-1);
        }
	}
	if (r-1 >= 0 && c-1 >= 0 ) {
        theBoard[r-1][c-1].notifyAttack(color);
        if (!cmpClr(color,theBoard[r-1][c-1].getColor()) && theBoard[r-1][c-1].getAttack(color) == 0) {
            theBoard[r][c].addMove(r-1,c-1);
        }

    }
}

//------------------moveKnight------------------------------------------
// Saves the possible moves for the Knight in a vector

void Board::moveKnight(int r, int c) {
	char color = theBoard[r][c].getColor();
	if(r-2 >= 0 && c+1 < 8) {
        theBoard[r-2][c+1].notifyAttack(color);
        if (!cmpClr(color,theBoard[r-2][c+1].getColor())) {
            theBoard[r][c].addMove(r-2,c+1);
        }
    }
	if(r-2 >= 0 && c-1 >= 0) {
        theBoard[r-2][c-1].notifyAttack(color);
        if (!cmpClr(color,theBoard[r-2][c-1].getColor())) {
            theBoard[r][c].addMove(r-2,c-1);
        }
    }
	if(r+2 < 8 && c+1 < 8) {
        theBoard[r+2][c+1].notifyAttack(color);
        if (!cmpClr(color,theBoard[r+2][c+1].getColor())) {
            theBoard[r][c].addMove(r+2,c+1);
        }
    }
	if(r+2 < 8 && c-1 >= 0) {
        theBoard[r+2][c-1].notifyAttack(color);
        if (!cmpClr(color,theBoard[r+2][c-1].getColor())) {
            theBoard[r][c].addMove(r+2,c-1);
        }
    }
	if(r+1 < 8 && c+2 < 8) {
        theBoard[r+1][c+2].notifyAttack(color);
        if (!cmpClr(color,theBoard[r+1][c+2].getColor())) {
            theBoard[r][c].addMove(r+1,c+2);
        }
    }
	if(r-1 >= 0 && c+2 < 8) {
        theBoard[r-1][c+2].notifyAttack(color);
        if (!cmpClr(color,theBoard[r-1][c+2].getColor())) {
            theBoard[r][c].addMove(r-1,c+2);
        }
    }
	if(r+1 < 8 && c-2 >= 0) {
        theBoard[r+1][c-2].notifyAttack(color);
        if (!cmpClr(color,theBoard[r+1][c-2].getColor())) {
            theBoard[r][c].addMove(r+1,c-2);
        }
    }
	if(r-1 >= 0 && c-2 >= 0) {
        theBoard[r-1][c-2].notifyAttack(color);
        if (!cmpClr(color,theBoard[r-1][c-2].getColor())) {
            theBoard[r][c].addMove(r-1,c-2);
        }
    }
}

//-----------------------moveRook------------------------------------------------
// Saves the possible moves for the Rook in a vector

void Board::moveRook(int r, int c) {
	char color = theBoard[r][c].getColor();
	// Add moves horizontally to the right
	for (int i = c+1 ; i < 8; i++) {
		if (cmpClr(color,theBoard[r][i].getColor())) { // If same color, it doesn't add the cell, it break out of the loop
			theBoard[r][i].notifyAttack(color);
            break;
		}
		else if (theBoard[r][i].getType() != 'E'  && !cmpClr(color,theBoard[r][i].getColor())) { // If different color, it adds the cell and break out of the loop
			theBoard[r][c].addMove(r,i);
            theBoard[r][i].notifyAttack(color);
			break;
		}
		else { // Otherwise, it adds the cell
			theBoard[r][c].addMove(r,i);
            theBoard[r][i].notifyAttack(color);
        }
	}
	// Add moves horizontally to the left
	for (int i = c-1; i >= 0; i--) {
		if (cmpClr(color,theBoard[r][i].getColor())) { // If same color, it doesn't add the cell, it break out of the loop
            theBoard[r][i].notifyAttack(color);
            break;
        }
        else if (theBoard[r][i].getType() != 'E'  && !cmpClr(color,theBoard[r][i].getColor())) { // If different color, it adds the cell and break out of the loop
            theBoard[r][c].addMove(r,i);
            theBoard[r][i].notifyAttack(color);
            break;
        }
        else { // Otherwise, it adds the cell
            theBoard[r][c].addMove(r,i);
            theBoard[r][i].notifyAttack(color);
        }
	}
	// Add moves vertically downward
	for (int i = r+1; i < 8; i++) {
        if (cmpClr(color,theBoard[i][c].getColor())) { // If same color, it doesn't add the cell, it break out of the loop
            theBoard[i][c].notifyAttack(color);
            break;
        }
        else if (theBoard[i][c].getType() != 'E'  && !cmpClr(color,theBoard[i][c].getColor())) { // If different color, it adds the cell and break out of the loop
            theBoard[r][c].addMove(i,c);
            theBoard[i][c].notifyAttack(color);
            break;
        }
        else { // Otherwise, it adds the cell
            theBoard[r][c].addMove(i,c);
            theBoard[i][c].notifyAttack(color);
        }
    }
	// Add moves vertically upward
	for (int i = r-1; i >= 0; i--) {
        if (cmpClr(color,theBoard[i][c].getColor())) { // If same color, it doesn't add the cell, it break out of the loop
            theBoard[i][c].notifyAttack(color);
            break;
        }
        else if (theBoard[i][c].getType() != 'E'  && !cmpClr(color,theBoard[i][c].getColor())) { // If different color, it adds the cell and break out of the loop
            theBoard[r][c].addMove(i,c);
            theBoard[i][c].notifyAttack(color);
            break;
        }
        else { // Otherwise, it adds the cell
            theBoard[r][c].addMove(i,c);
            theBoard[i][c].notifyAttack(color);
        }
    }
}

//---------------------moveBishop-----------------------------------------------
// Saves the possible moves for the Bishop in a vector

void Board::moveBishop(int r, int c) {
	char color = theBoard[r][c].getColor();
	// Add moves diagonally right upward
	for (int i = r-1, j = c+1; i >= 0 && j < 8; i--, j++) {
		if (cmpClr(color,theBoard[i][j].getColor())) { // If same color, it doesn't add the cell, it break out of the loop
            theBoard[i][j].notifyAttack(color);
            break;
        }
        else if (theBoard[i][j].getType() != 'E'  && !cmpClr(color,theBoard[i][j].getColor())) { // If different color, it adds the cell and break out of the loop
            theBoard[r][c].addMove(i,j);
            theBoard[i][j].notifyAttack(color);
            break;
        }
        else { // Otherwise, it adds the cell
            theBoard[r][c].addMove(i,j);
            theBoard[i][j].notifyAttack(color);
        }
	}
	// Add moves diagonally left downward
	for (int i = r+1, j = c-1; i < 8 && j >= 0; i++, j--) {
        if (cmpClr(color,theBoard[i][j].getColor())) { // If same color, it doesn't add the cell, it break out of the loop
            theBoard[i][j].notifyAttack(color);
            break;
        }
        else if (theBoard[i][j].getType() != 'E'  && !cmpClr(color,theBoard[i][j].getColor())) { // If different color, it adds the cell and break out of the loop
            theBoard[r][c].addMove(i,j);
            theBoard[i][j].notifyAttack(color);
            break;
        }
        else { // Otherwise, it adds the cell
            theBoard[r][c].addMove(i,j);
            theBoard[i][j].notifyAttack(color);
        }
    }
	// Add moves diagonally right downward
	for (int i = r+1,  j = c+1; i < 8 && j < 8; i++, j++) {
        if (cmpClr(color,theBoard[i][j].getColor())) { // If same color, it doesn't add the cell, it break out of the loop
            theBoard[i][j].notifyAttack(color);
            break;
        }
        else if (theBoard[i][j].getType() != 'E'  && !cmpClr(color,theBoard[i][j].getColor())) { // If different color, it adds the cell and break out of the loop
            theBoard[r][c].addMove(i,j);
            theBoard[i][j].notifyAttack(color);
            break;
        }
        else { // Otherwise, it adds the cell
            theBoard[r][c].addMove(i,j);
            theBoard[i][j].notifyAttack(color);
        }
    }
	// Add moves diagonally left upward
	for (int i = r-1, j = c-1; i >= 0 && j >= 0; i--, j--) {
        if (cmpClr(color,theBoard[i][j].getColor())) { // If same color, it doesn't add the cell, it break out of the loop
            theBoard[i][j].notifyAttack(color);
            break;
        }
        else if (theBoard[i][j].getType() != 'E'  && !cmpClr(color,theBoard[i][j].getColor())) { // If different color, it adds the cell and break out of the loop
            theBoard[r][c].addMove(i,j);
            theBoard[i][j].notifyAttack(color);
            break;
        }
        else { // Otherwise, it adds the cell
            theBoard[r][c].addMove(i,j);
            theBoard[i][j].notifyAttack(color);
        }
    }
}

//-------------------moveQueen----------------------------------------
// Saves the possible moves for the Queen in a vector
// Queen's moves is a mix of Rook's moves and Bishop's moves

void Board::moveQueen(int r, int c) {
	moveRook(r,c);
	moveBishop(r,c);
}

//-------------------movePawn----------------------------------------
// Saves the possible moves for the Pawn in a vector

void Board::movePawn(int r, int c) {
	char color = theBoard[r][c].getColor();
	// Moves for Black Pawn
	if (color == 'B') {
		if(r == 1 && theBoard[r+1][c].getColor() == 'E' && theBoard[r+2][c].getColor() == 'E') {
			theBoard[r][c].addMove(r+1,c);
			theBoard[r][c].addMove(r+2,c);
		}
		else if (r == 1 && theBoard[r+1][c].getColor() == 'E' && theBoard[r+2][c].getColor() != 'E') {
			theBoard[r][c].addMove(r+1,c);
		}
		else if (r+1 < 8 && theBoard[r+1][c].getColor() == 'E') {
			theBoard[r][c].addMove(r+1,c);
		}
		// Capturing diagonally
		if (r+1 < 8 && c+1 < 8) {
            theBoard[r+1][c+1].notifyAttack(color);
			if(theBoard[r+1][c+1].getType() != 'E') {
				if(!cmpClr(color,theBoard[r+1][c+1].getColor())) {
					theBoard[r][c].addMove(r+1,c+1);
                }
			}
		}
		if (r+1 < 8 && c-1 >= 0) {
            theBoard[r+1][c-1].notifyAttack(color);
			if(theBoard[r+1][c-1].getType() != 'E') {
                if(!cmpClr(color,theBoard[r+1][c-1].getColor())) {
                    theBoard[r][c].addMove(r+1,c-1);
                }
            }
		}
        // en-pasant for Black Pawns
        if (r == 4) { // If the pawn is in position to do en-pasant
            if (history.r == r && c+1 == history.c && theBoard[r][c+1].getType() == 'P') {
                theBoard[r][c].addMove(r+1,c+1);
            }
            if (history.r == r && c-1 == history.c && theBoard[r][c-1].getType() == 'P') {
                theBoard[r][c].addMove(r+1,c-1);
            }
        }
	}
	// Moves for White Pawns
	else if (color == 'W') {
		if(r == 6 && theBoard[r-1][c].getColor() == 'E' && theBoard[r-2][c].getColor() == 'E') {
            theBoard[r][c].addMove(r-1,c);
            theBoard[r][c].addMove(r-2,c);
        }
        else if (r == 6 && theBoard[r-1][c].getColor() == 'E' && theBoard[r-2][c].getColor() != 'E') {
            theBoard[r][c].addMove(r-1,c);
        }
        else if (r-1 >= 0 && theBoard[r-1][c].getColor() == 'E') {
            theBoard[r][c].addMove(r-1,c);
        }
        // Capturing diagonally
        if (r-1 >= 0 && c+1 < 8) {
            theBoard[r-1][c+1].notifyAttack(color);
            if(theBoard[r-1][c+1].getType() != 'E') {
                if(!cmpClr(color,theBoard[r-1][c+1].getColor())) {
                    theBoard[r][c].addMove(r-1,c+1);
                }
            }
        }
        if (r-1 >= 0 && c-1 >= 0) {
            theBoard[r-1][c-1].notifyAttack(color);
            if(theBoard[r-1][c-1].getType() != 'E') {
                if(!cmpClr(color,theBoard[r-1][c-1].getColor())) {
                    theBoard[r][c].addMove(r-1,c-1);                 
                }
            }
        }
        if ( r == 3) {
            if ( history.r == r && c+1 == history.c && theBoard[r][c+1].getType() == 'p') {
                theBoard[r][c].addMove(r-1,c+1);
            }
            if (history.r == r && c-1 == history.c && theBoard [r][c-1].getType() == 'p') {
                theBoard[r][c].addMove(r-1,c-1);
            }
        }
	}
    
}

//-----------------addMoves-------------------------------

void Board::addMoves(char type, int r1, int c1) {
	if (type == 'K' || type == 'k') {
        moveKing(r1,c1);
	}
	else if (type == 'Q' || type == 'q') {
        moveQueen(r1,c1);
	}
	else if (type == 'B' || type == 'b') {
        moveBishop(r1,c1);
	}
	else if (type == 'R' || type == 'r') {
        moveRook(r1,c1);
	}
	else if (type == 'N' || type == 'n') {
        moveKnight(r1,c1);
	}
	else if (type == 'P' || type == 'p') {
        movePawn(r1,c1);
	}
}

//-------------------setMoves---------------------------

void Board::setMoves(){
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            char type = theBoard[i][j].getType();
            if (type != 'E') {
                addMoves(type,i,j);
            }
        }
    }
}

//-----------------attackNotification---------------------

void Board::attackNotification() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            theBoard[i][j].notifyNoAttack();
        }
    }
    setMoves();
}

//------------------emptyBoard----------------------------------

void Board::emptyBoard() {
    string row_name[8]={"a","b","c","d","e","f","g","h"};
    string col_name[8]={"8","7","6","5","4","3","2","1"};
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            theBoard[i][j].setType('E');
            theBoard[i][j].setColor('E');
            theBoard[i][j].clearMove();
            theBoard[i][j].notifyDisplay(*td);
            theBoard[i][j].draw();
        }
    }
    
    checkW=false;
    checkB=false;
    sCastlingW=true;
    bCastlingW=true;
    sCastlingB=true;
    bCastlingB=true;
    checkmateW=false;
    checkmateB=false;
}

//-----------------setPiece--------------------------------------------

void Board::setPiece(int r, int c, char type, char color) {
    theBoard[r][c].setType(type);
    theBoard[r][c].setColor(color);
    theBoard[r][c].notifyDisplay(*td);
    theBoard[r][c].draw();
    if (type == 'K'){
        WhiteKingRow=r;
        WhiteKingCol=c;
    }
    if (type == 'k'){
        BlackKingRow=r;
        BlackKingCol=c;
    }
}

//------------------removePiece--------------------------------------

void Board::removePiece(int r, int c, char &t) {
    t=theBoard[r][c].getType();
    theBoard[r][c].setType('E');
    theBoard[r][c].setColor('E');
    theBoard[r][c].notifyDisplay(*td);
    theBoard[r][c].draw();
}

//------------------------getPieceMove------------------------------------------

vector<pos> Board::getPieceMove(int r, int c) {
    char type = theBoard[r][c].getType();
    addMoves(type,r,c);
    return theBoard[r][c].getMove();
}

//------------------------setKingsPos------------------------------------------

void Board::setKingsPos() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (theBoard[i][j].getType() == 'K') {
                WhiteKingRow = i;
                WhiteKingCol = j;
            }
            else if (theBoard[i][j].getType() == 'k') {
                BlackKingRow = i;
                BlackKingCol = j;
            }
        }
    }
}

//------------------------testMove--------------------------------------------

void Board::testMove(int r1, int c1, int r2, int c2, bool& myCheck, bool& enemyCheck) {
    setKingsPos();
    clearAllMoves();
    attackNotification();
    char type1 = theBoard[r1][c1].getType();
    char type2 = theBoard[r2][c2].getType();
    char color1 = theBoard[r1][c1].getColor();
    char color2 = theBoard[r2][c2].getColor();
    vector<pos> vec = theBoard[r1][c1].getMove();
    
    // Check if the go-to cell is valid for the piece
    for (unsigned int i = 0; i < vec.size(); i++) {
        // Looping through the vector of the availble moves of the piece
        if(r2 == vec[i].r && c2 == vec[i].c) {
            theBoard[r2][c2].setType(type1); // Swtich types
            theBoard[r1][c1].setType('E'); // Set the type of the old cell to empty
            theBoard[r2][c2].setColor(color1); // Switch colors
            theBoard[r1][c1].setColor('E'); // Set the color of the old cell to empty
            clearAllMoves();
            attackNotification();
            setKingsPos();
        }
    }
    if (color1 == 'W' && theBoard[WhiteKingRow][WhiteKingCol].getAttack('W') != 0) {
        myCheck = true;
    }
    // Check if this move put the black king in check
    else if (color1 == 'B' && theBoard[BlackKingRow][BlackKingCol].getAttack('B') != 0) {
        myCheck = true;
    }
    else {
        myCheck = false;
    }
    if (color1 == 'W' && theBoard[BlackKingRow][BlackKingCol].getAttack('B') != 0) {
        enemyCheck = true;
    }
    // Check if this move put the black king in check
    else if (color1 == 'B' && theBoard[WhiteKingRow][WhiteKingCol].getAttack('W') != 0) {
        enemyCheck = true;
    }
    else {
        enemyCheck = false;
    }
    theBoard[r1][c1].setType(type1);
    theBoard[r2][c2].setType(type2); // Set the type of the old cell to empty
    theBoard[r1][c1].setColor(color1); // Switch colors
    theBoard[r2][c2].setColor(color2); // Set the color of the old cell to empty
    clearAllMoves();
    attackNotification();
    setKingsPos();
}

//------------------------clearAllMoves----------------------------------------

void Board::clearAllMoves(){
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            theBoard[i][j].clearMove();
        }
    }
}

//-----------------------checkmate---------------------------------------------

bool Board::checkmate(char color) {
    if (color == 'W') {
        return checkmateW;
    }
    else {
        return checkmateB;
    }
}

//------------------------move-------------------------------------------------

bool Board::move(int r1, int c1, int r2, int c2, char type) {
    setKingsPos(); // Set the current kings position
    bool success = false;
	// Check for Valid input
    if (r1 < 0 || r1 > 7 || c1 < 0 || c1 > 7 || r2 < 0 || r2 > 7 || c2 < 0 || c2 > 7){
		return success;
	}
    
    // Initialize types and colors
	char type1 = theBoard[r1][c1].getType();
    char type2 = theBoard[r2][c2].getType();
	char color1 = theBoard[r1][c1].getColor();
	char color2 = theBoard[r2][c2].getColor();
    
    // Check if trying to move an empty cell
	if (type1 == 'E') {
		return success;
	}
    
    // Check if trying to replace a piece by another piece of the same color
	if (color1 == color2) {
		return success;
	}
    // Check if the rules of castling are true, then the player can do castling
    // Caslting the White king with the Rook on its right
    else if (type1 == 'K' && theBoard[7][7].getType() == 'R' && r2 == 7 && c2 == 6 && theBoard[7][5].getType() == 'E' && theBoard[7][6].getType() == 'E' && sCastlingW) {
        if (theBoard[7][5].getAttack('W') != 0){
            cout << "The king can't move through a square that is attacked by a piece of the opponent" << endl;
            return success;
        }
        else if (theBoard[r1][c1].getAttack(color1) != 0){
            cout << "Invalid Move: Can't Castle if the king is in check" << endl;
            clearAllMoves();
            attackNotification();
            return success;
        }
        else if (theBoard[r2][c2].getAttack(color1) != 0) {
            cout << "Invalid Move: White King is in danger" << endl;
            clearAllMoves();
            attackNotification();
            return success;
        }
        else {
            // Moving the king
            theBoard[r2][c2].setType(type1); // Swtich types
            theBoard[r1][c1].setType('E'); // Set the type of the old cell to empty
            theBoard[r2][c2].setColor(color1); // Switch colors
            theBoard[r1][c1].setColor('E'); // Set the color of the old cell to empty
            theBoard[r1][c1].notifyDisplay(*td); // Notify display for changes
            theBoard[r2][c2].notifyDisplay(*td); // Notify display for changes
        

                  
            //Moving the Rook
            theBoard[7][5].setType('R');
            theBoard[7][7].setType('E');
            theBoard[7][5].setColor('W');
            theBoard[7][7].setColor('E');
            theBoard[7][5].notifyDisplay(*td); // Notify display for changes
            theBoard[7][7].notifyDisplay(*td); // Notify display for changes
            theBoard[7][5].draw();
            theBoard[7][7].draw();
             
            success = true;
        }
    }
    // Castling the White king with the Rook on its left
    else if (type1 == 'K' && theBoard[7][0].getType() == 'R' && r2 == 7 && c2 == 2 && theBoard[7][3].getType() == 'E' && theBoard[7][2].getType() == 'E' && theBoard[7][1].getType() == 'E' && bCastlingW) {
        if (theBoard[7][3].getAttack('W') != 0){
            cout << "The king can't move through a square that is attacked by a piece of the opponent" << endl;
            return success;
        }
        else if (theBoard[r1][c1].getAttack(color1) != 0){
            cout << "Invalid Move: Can't Castle if the king is in check" << endl;
            clearAllMoves();
            attackNotification();
            return success;
        }
        else if (theBoard[r2][c2].getAttack(color1) != 0) {
            cout << "Invalid Move: White King is in danger" << endl;
            clearAllMoves();
            attackNotification();
            return success;
        }
        else {
            // Moving the king
            theBoard[r2][c2].setType(type1); // Swtich types
            theBoard[r1][c1].setType('E'); // Set the type of the old cell to empty
            theBoard[r2][c2].setColor(color1); // Switch colors
            theBoard[r1][c1].setColor('E'); // Set the color of the old cell to empty
            theBoard[r1][c1].notifyDisplay(*td); // Notify display for changes
            theBoard[r2][c2].notifyDisplay(*td); // Notify display for changes
            theBoard[r1][c1].draw();
            theBoard[r2][c2].draw();
    
        
            //Moving the Rook
            theBoard[7][3].setType('R');
            theBoard[7][0].setType('E');
            theBoard[7][3].setColor('W');
            theBoard[7][0].setColor('E');
            theBoard[7][3].notifyDisplay(*td); // Notify display for changes
            theBoard[7][0].notifyDisplay(*td); // Notify display for changes
            theBoard[7][3].draw();
            theBoard[7][0].draw();
        
    
            success = true;
        }
    }
    // Caslting the Black king with the Rook on its left
    else if (type1 == 'k' && theBoard[0][7].getType() == 'r' && r2 == 0 && c2 == 6 && theBoard[0][5].getType() == 'E' && theBoard[0][6].getType() == 'E' && sCastlingB) {
        if (theBoard[0][5].getAttack('B') != 0){
            cout << "The king can't move through a square that is attacked by a piece of the opponent" << endl;
            return success;
        }
        else if (theBoard[r1][c1].getAttack(color1) != 0){
            cout << "Invalid Move: Can't Castle if the king is in check" << endl;
            clearAllMoves();
            attackNotification();
            return success;
        }
        else if (theBoard[r2][c2].getAttack(color1) != 0) {
            cout << "Invalid Move: Black King is in danger" << endl;
            clearAllMoves();
            attackNotification();
            return success;
        }
        else {
            // Moving the king
            theBoard[r2][c2].setType(type1); // Swtich types
            theBoard[r1][c1].setType('E'); // Set the type of the old cell to empty
            theBoard[r2][c2].setColor(color1); // Switch colors
            theBoard[r1][c1].setColor('E'); // Set the color of the old cell to empty
            theBoard[r1][c1].notifyDisplay(*td); // Notify display for changes
            theBoard[r2][c2].notifyDisplay(*td); // Notify display for changes
            theBoard[r1][c1].draw();
            theBoard[r2][c2].draw();
        
            //Moving the Rook
            theBoard[0][5].setType('r');
            theBoard[0][7].setType('E');
            theBoard[0][5].setColor('B');
            theBoard[0][7].setColor('E');
            theBoard[0][5].notifyDisplay(*td); // Notify display for changes
            theBoard[0][7].notifyDisplay(*td); // Notify display for changes
            theBoard[0][5].draw();
            theBoard[0][7].draw();


            
            success = true;
        }
    }
    // Castling the Black king with the Rook on its right
    else if (type1 == 'k' && theBoard[0][0].getType() == 'r' && r2 == 0 && c2 == 2 && theBoard[0][3].getType() == 'E' && theBoard[0][2].getType() == 'E' && theBoard[0][1].getType() == 'E' && bCastlingB) {
        if (theBoard[0][3].getAttack('B') != 0){
            cout << "The king can't move through a square that is attacked by a piece of the opponent" << endl;
            return success;
        }
        else if (theBoard[r1][c1].getAttack(color1) != 0){
            cout << "Invalid Move: Can't Castle if the king is in check" << endl;
            clearAllMoves();
            attackNotification();
            return success;
        }
        else if (theBoard[r2][c2].getAttack(color1) != 0) {
            cout << "Invalid Move: Black King is in danger" << endl;
            clearAllMoves();
            attackNotification();
            return success;
        }
        else {
            // Moving the king
            theBoard[r2][c2].setType(type1); // Swtich types
            theBoard[r1][c1].setType('E'); // Set the type of the old cell to empty
            theBoard[r2][c2].setColor(color1); // Switch colors
            theBoard[r1][c1].setColor('E'); // Set the color of the old cell to empty
            theBoard[r1][c1].notifyDisplay(*td); // Notify display for changes
            theBoard[r2][c2].notifyDisplay(*td); // Notify display for changes
            theBoard[r1][c1].draw();
            theBoard[r2][c2].draw();
        
            //Moving the Rook
            theBoard[0][3].setType('r');
            theBoard[0][0].setType('E');
            theBoard[0][3].setColor('B');
            theBoard[0][0].setColor('E');
            theBoard[0][3].notifyDisplay(*td); // Notify display for changes
            theBoard[0][0].notifyDisplay(*td); // Notify display for changes
            theBoard[0][3].draw();
            theBoard[0][0].draw();
        
            success = true;
        }
    }
    // Any other move
    else {
        // Get the available moves for the piece that is going to move
		vector<pos> vec = theBoard[r1][c1].getMove();
        
        // Check if the go-to cell is valid for the piece
		for (unsigned int i = 0; i < vec.size(); i++) {
            // Looping through the vector of the availble moves of the piece
			if(r2 == vec[i].r && c2 == vec[i].c) {
                success = true;
                // White Pawn promotion
                if (type1 == 'P' && r2 == 0) { // added by elena
                    theBoard[r2][c2].setType(type); // Swtich types
                    theBoard[r1][c1].setType('E'); // Set the type of the old cell to empty
                    theBoard[r2][c2].setColor(color1); // Switch colors
                    theBoard[r1][c1].setColor('E'); // Set the color of the old cell to empty
                    
                }
                // Black Pawn promotion
                else if (type1 == 'p' && r2 == 7) {// added by elena
                    theBoard[r2][c2].setType(type); // Swtich types
                    theBoard[r1][c1].setType('E'); // Set the type of the old cell to empty
                    theBoard[r2][c2].setColor(color1); // Switch colors
                    theBoard[r1][c1].setColor('E'); // Set the color of the old cell to empty
                }
                // White pawn en-passant
                else if (type1 == 'P' && r2 == r1-1 && (c2 == c1-1 || c2 == c1+1) && type2 == 'E') {
                    theBoard[r2][c2].setType(type1); // Move the pawn to its destination
                    theBoard[r2][c2].setColor(color1); // Set the color
                    theBoard[r1][c1].setType('E'); // Empty the old cell
                    theBoard[r1][c1].setColor('E'); // Set the color of the old cell to 'E'
                    theBoard[r1][c2].setType('E'); // Capture the oponent pawn en-passant
                    theBoard[r1][c2].setColor('E'); // Capture the oponent pawn en-passant
                    theBoard[r1][c2].notifyDisplay(*td); // Notify display for changes
                    theBoard[r1][c2].draw();
                    
                    en_passantW = true;
                }
                // Black pawn en-passant
                else if (type1 == 'p' && r2 == r1+1 && (c2 == c1-1 || c2 == c1+1) && type2 == 'E') {
                    theBoard[r2][c2].setType(type1); // Move the pawn to its destination
                    theBoard[r2][c2].setColor(color1); // Set the color
                    theBoard[r1][c1].setType('E'); // Empty the old cell
                    theBoard[r1][c1].setColor('E'); // Set the color of the old cell to 'E'
                    theBoard[r1][c2].setType('E'); // Capture the oponent pawn en-passant
                    theBoard[r1][c2].setColor('E'); // Capture the oponent pawn en-passant
                    theBoard[r1][c2].notifyDisplay(*td); // Notify display for changes
                    theBoard[r1][c2].draw();
                    en_passantB = true;
                }
                else {
                    theBoard[r2][c2].setType(type1); // Swtich types
                    theBoard[r1][c1].setType('E'); // Set the type of the old cell to empty
                    theBoard[r2][c2].setColor(color1); // Switch colors
                    theBoard[r1][c1].setColor('E'); // Set the color of the old cell to empty
                }
                
		clearAllMoves();
		attackNotification();
                setKingsPos();
                
			}
		}
        // Check if this move put the white king in check
        if (color1 == 'W' && theBoard[WhiteKingRow][WhiteKingCol].getAttack('W') != 0) {
            cout << "Invalid Move: White king is in danger" << endl;
            // Undo the move
            theBoard[r1][c1].setType(type1);
            theBoard[r2][c2].setType(type2); // Set the type of the old cell
            theBoard[r1][c1].setColor(color1); // Switch colors
            theBoard[r2][c2].setColor(color2); // Set the color of the old cell
            if (en_passantW) {
                theBoard[r1][c2].setType('p');
                theBoard[r1][c2].setColor('B');
                theBoard[r1][c2].notifyDisplay(*td); // Notify display for changes
                theBoard[r1][c2].draw();
                
                en_passantW = false;
            }
            clearAllMoves();
            attackNotification();
            success = false;
            return success;
        }
        
        // Check if this move put the black king in check
		else if (color1 == 'B' && theBoard[BlackKingRow][BlackKingCol].getAttack('B') != 0) {
            cout << "Invalid Move: Black king is in danger" << endl;
            // Undo the move
            theBoard[r1][c1].setType(type1);
            theBoard[r2][c2].setType(type2); // Set the type of the old cell to empty
            theBoard[r1][c1].setColor(color1); // Switch colors
            theBoard[r2][c2].setColor(color2); // Set the color of the old cell to empty
            if (en_passantB) {
                theBoard[r1][c2].setType('P');
                theBoard[r1][c2].setColor('W');
                theBoard[r1][c2].notifyDisplay(*td); // Notify display for changes
                theBoard[r1][c2].draw();
                
                en_passantB = false;
            }
            clearAllMoves();
            attackNotification();
            success = false;
            return success;
        }
        
    }
    theBoard[r1][c1].notifyDisplay(*td); // Notify display for changes
    theBoard[r2][c2].notifyDisplay(*td); // Notify display for changes
    theBoard[r1][c1].draw();
    theBoard[r2][c2].draw();
    
    // Check if the White king is checked
    if (theBoard[WhiteKingRow][WhiteKingCol].getAttack('W') != 0) {
        bool escape = false;
        vector<pos> v = theBoard[WhiteKingRow][WhiteKingCol].getMove();
        for (int i = 0; i < v.size(); i++) {
            int row = v[i].r;
            int col = v[i].c;
            if (theBoard[row][col].getAttack('W') == 0) {
                escape = true;
            }
        }
        if (escape) {
           cout << "White is in check." << endl;
        }
        else if (!escape) {
            //cout << "Checkmate! White King" << endl;
            checkmateW = true;
        }
    }
     
    // Check if the Black king is in Check
    else if (theBoard[BlackKingRow][BlackKingCol].getAttack('B') != 0) {
        bool escape = false;
        vector<pos> v = theBoard[BlackKingRow][BlackKingCol].getMove();
        for (int i = 0; i < v.size(); i++) {
            int row = v[i].r;
            int col = v[i].c;
            if (theBoard[row][col].getAttack('B') == 0) {
                escape = true;
            }
        }
        if (escape) {
           cout << "Black is in check." << endl;
        }
        else if (!escape) {
            //cout << "Checkmate! Black King" << endl;
            checkmateB = true;
        }

    }
     
    // If the king moves, then castling is not availble
    if (type1 == 'K' && success) {
        sCastlingW = false;
        bCastlingW = false;
    }
    else if (type1 == 'k' && success) {
        sCastlingB = false;
        bCastlingB = false;
    }
    
    // If a Rook moves, then castling is not availble with that Rook
    if ( success && type1 == 'R' && r1 == 7 && c1 == 0) {
        bCastlingW = false;
    }
    else if ( success && type1 == 'R' && r1 == 7 && c1 == 7) {
        sCastlingW = false;
    }
    else if ( success && type1 == 'r' && r1 == 0 && c1 == 0) {
        bCastlingB = false;
    }
    else if ( success && type1 == 'r' && r1 == 0 && c1 == 7) {
        sCastlingB = false;
    }
    history.r = r2;
    history.c = c2;
    clearAllMoves();
    attackNotification();
    return success;
}

//-----------------getKingPos----------------------------------------------

void Board::getKingPos(char color, int &r, int &c){
    if (color =='W'){
        r=WhiteKingRow;
        c=WhiteKingCol;
    }
    else{
        r=BlackKingRow;
        c=BlackKingCol;
    }
}

//-----------------getCellType----------------------------------------------

char Board::getCellType(int r ,int c){
    return theBoard[r][c].getType();
}


//-----------------getPieceColor-----------------------------------------

char Board::getPieceColor(int r,int c){
    return theBoard[r][c].getColor();
}

//-----------------getPieceAttack-----------------------------------------

int Board::getPieceAttack(int r ,int c, char color){
    return theBoard[r][c].getAttack(color);
}

//-----------------getPieces--------------------------------------------
vector<pos> Board::getPieces(char color){
    vector<pos> pieces;
    for (int i=0; i<8; i++){
        for (int j=0; j<8; j++){
            if (theBoard[i][j].getColor()==color){
                pieces.push_back(pos(i,j));
            }
        }
    }
    return pieces;
}

//------------------validSetup--------------------------------------------------

bool Board::validSetup(int K, int k){
    char piece;
    
    for (int j=0; j<8; j++){//Condition on pawns at first/last row
        if ((theBoard[0][j].getType()=='p')||(theBoard[0][j].getType()=='P')){
            cout<<"no pawns should be at the last row of the board"<<endl;
            return false;
        }
        if ((theBoard[7][j].getType()=='p')||(theBoard[7][j].getType()=='P')){
            cout<<"no pawns should be at the first row of the board"<<endl;
            return false;
        }
    }
    
    
    if ((K!=1)||(k!=1)){//Condition on # of kings
        cout<<"the board should contatin exactly one white and one black king"<<endl;
        return false;
    }
    
    attackNotification();
    for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
            piece=theBoard[i][j].getType();
            if (piece=='K'){
                WhiteKingRow = i;
                WhiteKingCol = j;
                if (theBoard[i][j].getAttack('W')>0){
                    cout<<"a king should not be in check"<<endl;
                    return false;
                }
                
            }
		if (piece=='k'){// Condition on kings' checks
                BlackKingRow = i;
                BlackKingCol = j;
                if (theBoard[i][j].getAttack('B')>0){
                    cout<<"a king should not be in check"<<endl;
                    return false;
                }
                
            }
		}
	}
    return true;
}
