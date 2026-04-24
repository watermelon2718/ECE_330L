#include "Cursor.h"
#include "GameConstAndEnums.h" // include constants from Game header

//updates cursor to move from vertical to horizontal and vice versa
void CursorHorizVertToggle(Cursor *c) {

	if(c->cursorMode == HORIZONTAL){
		c->cursorMode = VERTICAL; //toggle cursorMode value

		if(c->yPos >= V_HEIGHT) { // cap the y index at 1
			c->yPos = (V_HEIGHT -1);
		}

		c->xPos = (c->xPos)*2; // map the x index to the appropriate location



	} else if(c->cursorMode == VERTICAL) {
		c->cursorMode = HORIZONTAL; //toggle cursorMode value

		//y index is unchanged

		//map the x position by dividing by 2 (integer division drops the decimal)
		c->xPos /= 2;
	}
}

//switches cursor between single and double segment modes
void CursorSingleDoubleToggle(Cursor *c) {
	if(c->cursorLength == SINGLE_SEG){
		c->cursorLength = DOUBLE_SEG; //toggle cursorLength value
	} else if(c->cursorLength == DOUBLE_SEG) {
		c->cursorLength = SINGLE_SEG; //toggle cursorLength value
	}
}
