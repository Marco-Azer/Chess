#ifndef __TEXTDISPLAY_H__
#define __TEXTDISPLAY_H__  
#include <iostream>
class TextDisplay{
	char **theDisplay;
public: 
	TextDisplay();       
	void notify(int r, int c, char type, char colour);
	~TextDisplay();
	friend std::ostream & operator<<(std::ostream& out, const TextDisplay& td);
};
#endif
