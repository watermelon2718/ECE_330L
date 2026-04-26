#include "Potentiometer.h"
#include "GameConstAndEnums.h"

#define MAX_VALUE 4100 // cap value for potentiometer maximum (not intended to be precise, tweak to fix deadzones)

//struct init
void InitPot(Potentiometer *p, Cursor *c, int channel, Axis a, ADC_TypeDef *adc) {
    p->adc = adc; // set the adc for this pot
    p->cursor = c; // set the cursor that this pot controls
    p->raw = 0; // set raw reading value to 0
    p->channel = channel; // set the channel of this pot
    p->axis = a; // set the axis of this pot
}

//function to poll the potentiometer
void UpdatePot(Potentiometer *p) {
    ReadADC(p); // read raw value
    UpdateCursor(p); // update cursor based on new raw value
}

//reads raw adc value for potentiometer
void ReadADC(Potentiometer *p) {
    p->adc->SQR1 = 0;                // one conversion in sequence
    p->adc->SQR3 = p->channel;       // select which ADC channel to convert
    p->adc->SR = 0;                  // clear ADC status flags
    p->adc->CR2 |= (1 << 30);        // start conversion using SWSTART

    while ((p->adc->SR & (1 << 1)) == 0){
        // wait until conversion is done
    }
    p->raw = (p->adc->DR); // return result
}

//function to update cursor based upon raw values
void UpdateCursor(Potentiometer *p) {
    Cursor *c = p->cursor; // for convenient referencing

    //depending on cursor mode
    switch(c->cursorMode) {
        case VERTICAL:
            if(p->axis == X_AXIS) {
                c->xPos = ((p->raw)*(V_WIDTH))/MAX_VALUE; // set cursor x based on raw
            } else if(p->axis == Y_AXIS) {
                c->yPos = ((p->raw)*(V_HEIGHT))/MAX_VALUE; // set cursor y based on raw
            }
            //bound cursor according to map dimensions and cursor length
            if(c->yPos > (V_HEIGHT-c->cursorLength)){c->yPos = V_HEIGHT-c->cursorLength;}
            if(c->xPos > (V_WIDTH-1)){c->xPos = V_WIDTH-1;}
            break;

        case HORIZONTAL:
            if(p->axis == X_AXIS) {
                c->xPos = ((p->raw)*(H_WIDTH))/MAX_VALUE; // set cursor x based on raw
            } else if(p->axis == Y_AXIS) {
                c->yPos = ((p->raw)*(H_HEIGHT))/MAX_VALUE; // set cursor y based on raw
            }
            //bound cursor according to map dimensions and cursor length
            if(c->xPos > (H_WIDTH-c->cursorLength)){c->xPos = H_WIDTH-c->cursorLength;}
            if(c->yPos > (H_HEIGHT-1)){c->yPos = H_HEIGHT-1;}
            break;    
    }
}



