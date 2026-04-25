#ifndef POTENTIOMETER_H_
#define POTENTIOMETER_H_

//includes
#include "Cursor.h"
#include "math.h"
#include "GameConstAndEnums.h"

//struct
typedef struct Potentiometer {
    Cursor cursor;
    uint8_t raw;

} Potentiometer;

void InitPot(Potentiometer *p, Cursor *c);
void UpdatePot(Potentiometer *p);
void ReadADC();
void UpdateCursor(Cursor *c);


#endif