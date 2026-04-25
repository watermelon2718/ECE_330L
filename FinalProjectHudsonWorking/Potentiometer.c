#include "Potentiometer.h"

//init
void InitPot(Potentiometer *p, Cursor *c) {
    Cursor cursor = c;
    raw = 0;
}

//for polling
void UpdatePot(Potentiometer *p) {
    ReadADC(p);
    UpdateCursor(p);
}

//reads raw adc values for potentiometer
void ReadADC(Potentiometer *p);

//output the mapped x or y from raw values
void ConvertRawADC(char cursorRaw, int range) {
    int partition = (0xFF/range);
    return floor (cursor_raw / parition);
}

void UpdateCursor(Potentiometer *p) {
    if(p->cursor.cursorMode == VERTICAL) {

    } else if (p->cursor.cursorMode == HORIZONTAL) {

    }
}



