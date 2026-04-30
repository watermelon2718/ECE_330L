#include "Button.h"

//initializer
void ButtonInit(Button *b, GPIO_TypeDef* port, uint16_t pin) {
    b->port = port; // init to specified port
    b->pin = pin; // init to specified pin
    b->currentState = 0; // current state is 0
    b->rawState = 0; // raw state is 0
    b->counter = 0; // counter is 0 
    b->hasBeenPressed = 0; // has not been pressed
}

//poll this
void ButtonUpdate(Button *b) {
    // Read current physical state (assuming Active High)
    // If your buttons are Active Low (Internal Pull-up), use !HAL_GPIO_ReadPin
    int reading = !HAL_GPIO_ReadPin(b->port, b->pin);

    if (reading != b->rawState) {
        // State is changing, reset the debounce counter
        b->rawState = reading;
        b->counter = 0;
    } else {
        // State is steady, increment counter
        if (b->counter < DEBOUNCE_THRESHOLD) {
            b->counter++;
            
            // If we hit the threshold, we have a confirmed state change
            if (b->counter == DEBOUNCE_THRESHOLD) {
                // If it transitioned from 0 to 1, mark as a "Press Event"
                if (reading == 1 && b->currentState == 0) {
                    b->hasBeenPressed = 1;
                }
                b->currentState = reading;
            }
        }
    }
}

//tells you if the button was pressed
int WasPressed(Button *b) {
    if (b->hasBeenPressed) {
        b->hasBeenPressed = 0; // Reset the flag (consume the event)
        return 1; // return 1 if has been
    }
    return 0; // otherwise return 0
}