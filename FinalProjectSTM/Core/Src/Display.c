#include "Display.h"
#include "Game.h"

void DisplayInit(Display *d) {
	memset(d, 0, sizeof(Display)); //init to 0
}

//used by systick to update the representational array to be displayed
//(based upon game phase and state)
void DisplayBoard(Game *game) {
	UpdateDisplayArrays(game);
	UpdateFrameArrays(game);
	OutputFrame(&(game->display));
}

void UpdateDisplayArrays(Game *game) {
	switch (game->gamePhase) {
	case PHASE_TITLE:
		break; // no arrays are displayed, ignore
	case PHASE_PLACE:
		//placement phase needs to display ships already placed and the cursor
		for(int i = 0; i < H_WIDTH; i++) {
			for(int j = 0; j < H_HEIGHT; j++) {
				game->display.horizDisplayArray[i][j] = game->players[game->activePlayer].horizPlacementArray[i][j];
			}
		}

		for(int i = 0; i < V_WIDTH; i++) {
			for(int j = 0; j < V_HEIGHT; j++) {
				game->display.vertDisplayArray[i][j] = game->players[game->activePlayer].vertPlacementArray[i][j];
			}
		}

		//here, cursor overrides whatever would be in the slot
		AddCursor(game);

		break;

	case PHASE_TURN:
		//placement phase needs to display ships already placed and the cursor
		for(int i = 0; i < H_WIDTH; i++) {
			for(int j = 0; j < H_HEIGHT; j++) {
				game->display.horizDisplayArray[i][j] = game->players[game->activePlayer].horizGuessArray[i][j];
			}
		}

		for(int i = 0; i < V_WIDTH; i++) {
			for(int j = 0; j < V_HEIGHT; j++) {
				game->display.vertDisplayArray[i][j] = game->players[game->activePlayer].vertPlacementArray[i][j];
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
void UpdateFrameArrays(Game *game) {
    // Static counters persist between function calls
    static int displayTimer = 0;
    displayTimer++;

	//determine duty cycles for each tileState
	int noneBright = 0; // none will always be off
	int hitBright = 1; //hit ships are at 100% brightness
	int missBright =(displayTimer % 5 == 0); // miss gets 20% duty cycle
    int placedBright = 1; // placed ships are displayed at 100% brightness
	int cursorBright = (displayTimer % 1500 < 750); //cursor blinks, toggle the blink state every 750 counts

    //Process Horizontal Array
    for (int i = 0; i < H_WIDTH; i++) {
        for (int j = 0; j < H_HEIGHT; j++) {
            TileState tileState = game->display.horizDisplayArray[i][j];
            
            switch (tileState) {
                case NONE:
                    game->display.horizFrameArray[i][j] = noneBright;
                    break;
                case HIT:
					game->display.horizFrameArray[i][j] = hitBright;
                    break;
                case PLACED:
                    game->display.horizFrameArray[i][j] = placedBright;
                    break;
                case MISS:
                    game->display.horizFrameArray[i][j] = missBright; 
                    break;
                case CURSOR:
                    game->display.horizFrameArray[i][j] = cursorBright;
                    break;
            }
        }
    }

    // Process Vertical Array
    for (int i = 0; i < V_WIDTH; i++) {
        for (int j = 0; j < V_HEIGHT; j++) {
            TileState currentState = game->display.vertDisplayArray[i][j];
            
            switch (currentState) {
                case NONE:
                    game->display.vertFrameArray[i][j] = noneBright;
                    break;
                case HIT:
					game->display.vertFrameArray[i][j] = hitBright;
                    break;
                case PLACED:
                    game->display.vertFrameArray[i][j] = placedBright;
                    break;
                case MISS:
                    game->display.vertFrameArray[i][j] = missBright;
                    break;
                case CURSOR:
                    game->display.vertFrameArray[i][j] = cursorBright;
                    break;
            }
        }
    }
    
    // Reset displayTimer eventually to prevent overflo
    if (displayTimer >= 15000) displayTimer = 0; 
}

void OutputFrame(Display *d) {
	for(int i = 0; i < H_WIDTH; i++) {
		int bitMask = !Get7SegBitMask(d,i); // get the mask for the character
		GPIOE->ODR = (0xFF00 | bitMask) & ~(1<<(i+8));
		// Set all selects high to latch-in character
		GPIOE->ODR |= 0xFF00;
	}
}

int Get7SegBitMask(Display *d, int displayIndex) {
	    int bitMask = 0x0; // initialize seven segment code to 0
	    int horizontalXShift = (H_WIDTH-1)-displayIndex; // find offset for x value for indexing horizontal matrix
	    int verticalXShift = ((H_WIDTH-1)-displayIndex)*2; // find offset for x value for indexing vertical matrix
	    bitMask |= ((d->horizFrameArray[horizontalXShift][2]) << 0); //set segment A to the value in horizontal at 'a' position for the appropriate display
	    bitMask |= ((d->vertFrameArray[1 + verticalXShift][1]) << 1); //segment b
	    bitMask |= ((d->vertFrameArray[1 + verticalXShift][0]) << 2); // segment c
	    bitMask |= ((d->horizFrameArray[horizontalXShift][0]) << 3); // segment d
	    bitMask |= ((d->vertFrameArray[verticalXShift][0]) << 4); // segment e
	    bitMask |= ((d->vertFrameArray[verticalXShift][1]) << 5); // segment f
	    bitMask |= ((d->horizFrameArray[horizontalXShift][1]) << 6); // segment g
	    return bitMask; // return code for seven segment value, now with all of the segments set appropriately, and correct select line on
}
void AddCursor (Game *game) {
	//add cursor if horizontal
	if(game->cursor.cursorMode == HORIZONTAL) {
		game->display.horizDisplayArray[game->cursor.xPos][game->cursor.yPos] = CURSOR;
		if(game->cursor.cursorLength == DOUBLE_SEG) {
			game->display.horizDisplayArray[game->cursor.xPos + 1][game->cursor.yPos] = CURSOR;
		}
	}
	//add vertical cursor if present
	else if(game->cursor.cursorMode == VERTICAL) {
		game->display.vertDisplayArray[game->cursor.xPos][game->cursor.yPos] = CURSOR;
		if(game->cursor.cursorLength == DOUBLE_SEG) {
			game->display.vertDisplayArray[game->cursor.xPos][game->cursor.yPos+1] = CURSOR;
		}
	}
}
