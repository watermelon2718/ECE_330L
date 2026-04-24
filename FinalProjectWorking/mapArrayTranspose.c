//updates cursor to move from vertical to horizontal and vice versa

void mapArrayTranspose(Cursor *cursorToModify) {

	if(cursorToModify->cursorMode == HORIZONTAL){
		cursorToModify->cursorMode = VERTICAL; //toggle cursorMode value

		if(cursorToModify->yPos >= V_HEIGHT) { // cap the y index at 1
			cursorToModify->yPos = (V_HEIGHT -1);
		}

		cursorToModify->xPos = (cursorToModify->xPos)*2 -1; // map the x index to the appropriate location



	} else if(cursorToModify->cursorMode == VERTICAL) {
		cursorToModify->cursorMode = HORIZONTAL; //toggle cursorMode value

		//y index is unchanged

		//map the x position by dividing by 2 (integer division drops the decimal)
		cursorToModify->xPos /= 2;
	}
}
