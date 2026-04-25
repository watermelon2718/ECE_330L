#ifndef CURSOR_H_ // header guard
#define CURSOR_H_

typedef enum CursorMode {HORIZONTAL, VERTICAL} CursorMode; // horizontal or vertical mode
typedef enum CursorLength {ZERO_SEG, SINGLE_SEG, DOUBLE_SEG} CursorLength; // single or double segment mode

typedef struct Cursor {
	int xPos; // cursor x position
	int yPos; // cursor y position

	CursorMode cursorMode; // cursor mode
	CursorLength cursorLength; // cursor length

} Cursor;

void CursorInit(Cursor *c);

void CursorHorizVertToggle(Cursor *c);

void CursorSingleDoubleToggle(Cursor *c);

#endif