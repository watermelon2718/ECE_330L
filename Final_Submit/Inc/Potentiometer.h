#ifndef POTENTIOMETER_H_
#define POTENTIOMETER_H_

//includes
#include "Cursor.h" // include cursor struct
#include "GameConstAndEnums.h" // include game constants for position mapping functions
#include "stm32f4xx.h" // allow hardware interfacing
typedef enum Axis {X_AXIS, Y_AXIS} Axis; // enum for which axis a potentiometer acts on 

//struct
typedef struct Potentiometer {
    ADC_TypeDef *adc; // pointer to the ADC that a potentiometer reads from
    Cursor *cursor; // pointer to cursor modified by the potentiometer
    int channel; // indicates which ADC channel is used
    int raw; // keeps raw value read from ADC
    Axis axis; // denotes axis of operation

} Potentiometer;

void InitPot(Potentiometer *p, Cursor *c, int channel, Axis a, ADC_TypeDef *adc); // initialize struct
void UpdatePot(Potentiometer *p); // updates potentiometer input, polled
void ReadADC(Potentiometer *p); // helper function which reads from the ADC of the pot
void UpdateCursor(Potentiometer *p); // updates cursor position based on raw ADC values

#endif