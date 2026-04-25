#include "Potentiometer.h"
#include "GameConstAndEnums.h"
#include "main.h"

//init
void InitPot(Potentiometer *p, Cursor *c, int channel, Axis a) {
    p->cursor = c;
    p->raw = 0;
    p->channel = channel;
    p->axis = a;
}

//for polling
void UpdatePot(Potentiometer *p) {
    ReadADC(p);
    UpdateCursor(p);
}

//reads raw adc values for potentiometer
void ReadADC(Potentiometer *p) {
    ADC1->SQR1 = 0;                // one conversion in sequence
    ADC1->SQR3 = p->channel;          // select which ADC channel to convert
    ADC1->SR = 0;                  // clear ADC status flags
    ADC1->CR2 |= (1 << 30);        // start conversion using SWSTART

    while ((ADC1->SR & (1 << 1)) == 0){
        // wait until conversion is done
    }
    p->raw = (uint8_t)(ADC1->DR & 0xFF); // return 8-bit result [betwe
}

void UpdateCursor(Potentiometer *p) {
    switch(p->cursor->cursorMode) {
        case VERTICAL:
            if(p->axis == X_AXIS) {
                p->cursor->xPos = ((p->raw)*(H_WIDTH -(p->cursor->cursorLength)))/256;
            } else if(p->axis == Y_AXIS) {
                p->cursor->yPos = ((p->raw)*(H_HEIGHT -(p->cursor->cursorLength)))/256;
            }
            break;
        case HORIZONTAL:
            if(p->axis == X_AXIS) {
                p->cursor->xPos = ((p->raw)*(V_WIDTH -(p->cursor->cursorLength)))/256;
            } else if(p->axis == Y_AXIS) {
                p->cursor->yPos = ((p->raw)*(V_HEIGHT -(p->cursor->cursorLength)))/256;
            }
            break;    
    }
        
}



