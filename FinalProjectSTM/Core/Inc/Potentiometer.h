#ifndef POTENTIOMETER_H_
#define POTENTIOMETER_H_

//includes
#include "Cursor.h"
#include "math.h"
#include "GameConstAndEnums.h"
#include "stm32f4xx.h"
typedef enum Axis {X_AXIS, Y_AXIS} Axis; 

//struct
typedef struct Potentiometer {
    ADC_TypeDef *adc;
    Cursor *cursor;
    int channel;
    int raw;
    Axis axis;

} Potentiometer;

void InitPot(Potentiometer *p, Cursor *c, int channel, Axis a, ADC_TypeDef adc);
void UpdatePot(Potentiometer *p);
void ReadADC(Potentiometer *p);
void UpdateCursor(Potentiometer *p);


#endif