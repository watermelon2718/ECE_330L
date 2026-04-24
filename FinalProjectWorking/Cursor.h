#ifndef CURSOR_H_ // header guard
#define CURSOR_H_

typedef enum CursorMode {HORIZONTAL, VERTICAL} CursorMode; // horizontal or vertical mode
typedef enum CursorLength {SINGLE_SEG, DOUBLE_SEG}; // single or double segment mode

typedef struct Cursor {
	int xPos; // cursor x position
	int yPos; // cursor y position

	CursorMode cursorMode; // cursor mode
	CursorLength cursorLength; // cursor length

} Cursor;

void CursorHorizVertToggle(Cursor *c);

void CursorSingleDoubleToggle(Cursor *c);

#endif