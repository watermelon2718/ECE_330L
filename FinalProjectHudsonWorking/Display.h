	#ifndef DISPLAY_H_ // header guard
	#define DISPLAY_H_

	#include "GameConstAndEnums.h" // include constants
	typedef struct Game Game; // inform compiler that Game is a struct

	//Display data structure
	typedef struct Display {
		TileState horizDisplayArray[H_WIDTH][H_HEIGHT]; //arrays for display contents
		TileState vertDisplayArray[V_WIDTH][V_HEIGHT];

		TileState horizFrameArray[H_WIDTH][H_HEIGHT]; //arrays for individual frame contents
		TileState vertFrameArray[V_WIDTH][V_HEIGHT];

	} Display;

	// master function, calculates all display and frame data and outputs to displays
	void DisplayBoard(Game *game); 

	// map current appropriate display data into the arrays, add cursor on top
	void UpdateDisplayArrays(Game *game);

	// generates current frame for display from display arrays
	void UpdateFrameArrays(Game *game);

	// outputs frameArray through GPIO to the 7seg display
	void OutputFrame(Display *d);

	// Utility function to convert array data into 7-segment bitmasks
	int Get7SegBitMask(Display *d, int displayIndex);

	// helper function to overlay the cursor position onto the display arrays
	void AddCursor(Game *game);



	#endif /* DISPLAY_H_ */
