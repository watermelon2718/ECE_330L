
//used by systick to update the representational array to be displayed
//(based upon game phase and state)
void UpdateDisplayArrays(Game *game) {
	switch (game->gamePhase) {
	case PHASE_TITLE:
		break; // no arrays are displayed, ignore
	case PHASE_PLACE:
		//placement phase needs to display ships already placed and the cursor
		for(int i = 0; i < H_WIDTH; i++) {
			for(int j = 0; j < H_HEIGHT; j++) {
				game->display->horizDisplayArray[i][j] = game->players[game->activePlayer]->horizPlacementArray[i][j];
			}
		}

		for(int i = 0; i < V_WIDTH; i++) {
			for(int j = 0; j < V_HEIGHT; j++) {
				game->display->vertDisplayArray[i][j] = game->players[game->activePlayer]->vertPlacementArray[i][j];
			}
		}

		//here, cursor overrides whatever would be in the slot
		AddCursor(game);

		break;

	case PHASE_TURN:
		//placement phase needs to display ships already placed and the cursor
		for(int i = 0; i < H_WIDTH; i++) {
			for(int j = 0; j < H_HEIGHT; j++) {
				game->display->horizDisplayArray[i][j] = game->players[game->activePlayer]->horizGuessArray[i][j];
			}
		}

		for(int i = 0; i < V_WIDTH; i++) {
			for(int j = 0; j < V_HEIGHT; j++) {
				game->display->vertDisplayArray[i][j] = game->players[game->activePlayer]->vertPlacementArray[i][j];
			}
		}
		//here, cursor overrides whatever would be in the slot
		AddCursor(game);

		break;

	case PHASE_END:

		break; // no arrays are displayed, ignore
	}
}


//used by the systick to get the data that will be output to the segments on a given 'frame'
//(includes dimming and effects)
void UpdateFrameArray () {
	static int dutyCycleCounter = 0;


}

void RefreshDisplay() {
	for(int i = 0; i < H_WIDTH; i++) {
		int bitMask = Get7SegBitMask(i);

		GPIOE->ODR = (0xFF00 | bitMask) & ~(1<<(i+8));

		#if COMMON_ANODE == 1
		if (dot > 0) GPIOE->ODR &= ~(1<<7); // Set dot segment to 0 if dot is on
		#else
		if (dot > 0) GPIOE->ODR |= (1<<7); // Set dot segment to 1 if dot is on
		#endif

		// Set all selects high to latch-in character
		GPIOE->ODR |= 0xFF00;
		return;
	}
}

int Get7SegBitMask(Display *d, int displayIndex) {

	    int bitMask = 0x0; // initialize seven segment code to 0

	    int horizontalXShift = (H_WIDTH-1)-displayIndex; // find offset for x value for indexing horizontal matrix

	    int verticalXShift = ((H_WIDTH-1)-displayIndex)*2; // find offset for x value for indexing vertical matrix

	    bitMask |= ((d->horizDisplayArray[horizontalXShift][2]) << 0); //set segment A to the value in horizontal at 'a' position for the appropriate display
	    bitMask |= ((d->vertDisplayArray[1 + verticalXShift][1]) << 1); //segment b
	    bitMask |= ((d->vertDisplayArray[1 + verticalXShift][0]) << 2); // segment c
	    bitMask |= ((d->horizDisplayArray[horizontalXShift][0]) << 3); // segment d
	    bitMask |= ((d->vertDisplayArray[verticalXShift][0]) << 4); // segment e
	    bitMask |= ((d->vertDisplayArray[verticalXShift][1]) << 5); // segment f
	    bitMask |= ((d->horizDisplayArray[horizontalXShift][1]) << 6); // segment g

	    return bitMask; // return code for seven segment value, now with all of the segments set appropriately, and correct select line on
}



void AddCursor (Game *game) {
	//add cursor if horizontal
	if(game->cursor->cursorMode == HORIZONTAL) {
		game->display->horizDisplayArray[game->cursor->xPos][game->cursor->yPos] = CURSOR;
		if(game->cursor->cursorLength == DOUBLE_SEG) {
			game->display->horizDisplayArray[game->cursor->xPos + 1][game->cursor->yPos] = CURSOR;
		}
	}
	//add vertical cursor if present
	else if(game->cursor->cursorMode == VERTICAL) {
		game->display->vertDisplayArray[game->cursor->xPos][game->cursor->yPos] = CURSOR;
		if(game->cursor->cursorLength == DOUBLE_SEG) {
			game->display->vertDisplayArray[game->cursor->xPos][game->cursor->yPos+1] = CURSOR;
		}
	}
}
