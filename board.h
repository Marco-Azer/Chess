#ifndef __BOARD_H__
#define __BOARD_H__
#include <iostream>
#include "cell.h"
#include "textdisplay.h"


class Board{
	Cell **theBoard; // Two dimensional array of Cells
	TextDisplay *td; // Pointer to text display
	bool checkW;
    	bool checkB;
	bool sCastlingW; // True if castling is available for white king with the rook on its right
	bool bCastlingW; //True if castling is available for white king with the rook on its left
    bool sCastlingB; // True if castling is available for the black king with the rook on its left
    bool bCastlingB; // True if castling is available for black king with the rook on its right
	char currentPlayer; // B for Black, W for White
    int WhiteKingCol; // Column of the white king
    int WhiteKingRow; // Row of the white king
    int BlackKingCol; // Column of the black king
    int BlackKingRow; // Row of the black king
    bool checkmateW; // True if the White king is in checkmate
    bool checkmateB; // True if the Black Kings is in checkmate
    pos history; // Keeps track of the last move that was performed, used for en-passant
    bool en_passantW; // True if the last move was a white pawn en-passant
    bool en_passantB; // True if the last move was a black pawn en-passant
        Xwindow *w; // window ptr
    
private:
	void moveKing(int,int);
	//Pre: Two Integers
	//Post: True if the King can move, false otherwise
	void moveQueen(int,int);
	//Pre: Two Integers
    //Post: True if the Queen can move, false otherwise
	void moveBishop(int,int);
	//Pre: Two Integers
    //Post: True if the Bishop can move, false otherwise
	void moveRook(int,int);
	//Pre: Two Integers
    //Post: True if the Rook can move, false otherwise
	void moveKnight(int,int);
	//Pre: Two Integers
    //Post: True if the Knight can move, false otherwise
	void movePawn(int,int);
	//Pre: Two Integers
    //Post: True if the Pawn can move, false otherwise
	void castling();
	//Pre: True
	//Post: return castling-w if the current player is white, castling-b otherwise
    void addMoves(char, int, int);
    //Pre: Char and two Integers
    //Post: Call move function depends on the char
    //Example: char K or k, will call moveKing etc.
    void setMoves();
    //Pre: True
    //Post: Set the available moves for each cell and notify all atacked cells
    void setKingsPos();
    //Pre: True
    //Post: Set the WhiteKingCol/Row and BlackKingCol/Row
    void clearAllMoves();
    //Pre: True
    //Post: Clears all the available moves of each cell
public:
    Board();
    //Pre: True
    //Post: Construct a Board
    ~Board();
    //Pre: True
    //Post: Destruct the Board
    void initTheBoard();
    //Pre: True
    //Post: Construct the board
    void init(char);
    //Pre: Char
    //Post: Initialize the game with all pieces in their place, by default its the white player turn first, if char is black, then black starts first
    bool move(int,int,int,int,char type='Q');
    //Pre: Four Integers and char
    //Post: Move the piece from one place to another if the move the available and return true. If the move is not available, then nothing change and return false
    void emptyBoard();
    //Pre: True
    //Post: Emptyt the board from all pieces
    void setPiece(int,int,char,char);
    //Pre: Two integers and two chars
    //Post: Set the cell at the given coordinate to the given type and given color
    bool checkmate(char);
    //Pre: Char
    //Post: Returns true if the king of color char is in checkmate, false otherwise
    char getCellType(int,int);
    //Pre: Two integers
    //Post: Return the cell type at the given coordinate
    char getPieceColor(int,int);
    //Pre: Two integers
    //Post: Return the cell type at the given coordinate
    int getPieceAttack(int,int,char);
    //Pre: Two integers
    //Post: Return the # attacks against the piece at the given coordinate
    std::vector<pos> getPieces(char);
    //Pre: Char
    //Post: Return the set of positions of pieces of the given colour
    void getKingPos(char color, int &r, int &c);
    //Pre: Char
    //Post: Return the positions of king of the given colour
    void removePiece(int,int,char &);
    //Pre: Two integers and char&
    //Post: Remove the piece at the given coordinate
    void attackNotification();
    //Pre: True
    //Post: Recalculate all attack for each cell
    bool validSetup(int K, int k);
    //Pre: True
    //Post: Check validility of setup
    void testMove(int, int, int ,int ,bool& , bool&);
    //Pre: four integers, two booleans
    //Post This method is used in the computer player, to check how effective is the move
    std::vector<pos> getPieceMove(int,int);
    //Pre: Two integers
    //Post: Return a vector of all the available moves for this piece
    Xwindow* getWindow();
    //Pre: True
    //Post: Return a ptr to window
    friend std::ostream &operator<<(std::ostream&, const Board&);
    //Pre: ostream and board
    //Post: Print the Board


};

#endif
