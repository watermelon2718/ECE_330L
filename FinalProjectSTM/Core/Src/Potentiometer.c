#include "Potentiometer.h"
#include "GameConstAndEnums.h"

#define MAX_VALUE 2400

//init
void InitPot(Potentiometer *p, Cursor *c, int channel, Axis a, ADC_TypeDef *adc) {
    p->adc = adc;
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
    p->adc->SQR1 = 0;                // one conversion in sequence
    p->adc->SQR3 = p->channel;          // select which ADC channel to convert
    p->adc->SR = 0;                  // clear ADC status flags
    p->adc->CR2 |= (1 << 30);        // start conversion using SWSTART

    while ((p->adc->SR & (1 << 1)) == 0){
        // wait until conversion is done
    }
    p->raw = (p->adc->DR); // return 8-bit result [betwe
}

void UpdateCursor(Potentiometer *p) {
    Cursor *c = p->cursor;
    switch(c->cursorMode) {
        case VERTICAL:
            if(p->axis == X_AXIS) {
                c->xPos = ((p->raw)*(H_WIDTH))/MAX_VALUE;
            } else if(p->axis == Y_AXIS) {
                c->yPos = ((p->raw)*(H_HEIGHT))/MAX_VALUE;
            }
            //bound cursor
            if(c->yPos > (V_HEIGHT-c->cursorLength-1)){c->yPos = V_HEIGHT-c->cursorLength;}
            
            break;

        case HORIZONTAL:
            if(p->axis == X_AXIS) {
                c->xPos = ((p->raw)*(V_WIDTH))/MAX_VALUE;
            } else if(p->axis == Y_AXIS) {
                c->yPos = ((p->raw)*(V_HEIGHT))/MAX_VALUE;
            }
            //bound cursor
            if(c->xPos > (H_WIDTH-c->cursorLength-1)){c->xPos = H_WIDTH-c->cursorLength;}
            break;    
    }

    //bound x and y
    if(p->cursor->xPos >= (H_WIDTH-p->cursor->cursorLength)) {

    }
        
}



