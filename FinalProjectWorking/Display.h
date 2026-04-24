#ifndef DISPLAY_H_ // header guard
#define DISPLAY_H_

#include "Game.h" // include game struct

//Display data structure
typedef struct Display {
	int frameMask; // bitmask that will be latched into the seven segment display each frame

	SquareState horizDisplayArray[H_WIDTH][H_HEIGHT]; //arrays for display contents
	SquareState vertDisplayArray[V_WIDTH][V_HEIGHT];

} Display;

// map current appropriate display data into the arrays, add cursor on top
void UpdateDisplayArrays(Game *game);

// generates current frame for display from display arrays
void UpdateFrameArray();

// outputs frameArray through GPIO to the 7seg display
void RefreshDisplay(Game *g);

// Utility function to convert array data into 7-segment bitmasks
int Get7SegBitMask(Display *d, int displayIndex);

// helper function to overlay the cursor position onto the display arrays
void AddCursor(Game *game);



#endif /* DISPLAY_H_ */
