#ifndef BUTTON_H_
#define BUTTON_H_

#include "stm32f4xx_hal.h" // Replace with your specific HAL if different

#define DEBOUNCE_THRESHOLD 20 // Number of consecutive polls required to confirm state

typedef struct Button {
    GPIO_TypeDef* port;     // e.g., GPIOA
    uint16_t pin;           // e.g., GPIO_PIN_0
    
    int currentState;       // The confirmed debounced state (0 or 1)
    int rawState;           // The current reading from the pin
    int counter;            // Counter for debouncing
    int hasBeenPressed;     // Flag: set to 1 when a "falling/rising edge" is detected
} Button;

/**
 * @brief Initializes the button struct.
 */
void ButtonInit(Button *b, GPIO_TypeDef* port, uint16_t pin);

/**
 * @brief Polls the hardware and updates the button's internal state machine.
 * Call this in your main while(1) loop.
 */
void ButtonUpdate(Button *b);

/**
 * @brief Checks if the button was pressed. 
 * Returns 1 once per physical press, then resets itself.
 */
int WasPressed(Button *b);

#endif