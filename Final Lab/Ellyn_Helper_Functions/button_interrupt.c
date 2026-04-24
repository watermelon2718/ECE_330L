// Logic to make button press generate interrupt + interrupt handler
//TODO: change from PC13 to PC10 or PC11

#include "main.h"

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

int main(void) {
  // Initialize HAL, clock, and GPIOs
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  // Main program loop
  while (1) {
    // The main loop can do other tasks or sleep
    // The LED toggling happens in the interrupt handler
    HAL_Delay(100);
  }
}

static void MX_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // Enable GPIO clocks
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  // Configure LED pin (PA5 on many Nucleo boards)
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Configure button pin (PC13 on many STM32 boards)
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  // Enable EXTI interrupt for button
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

// --------------------------------------------------- ISR for Button Press --------------------------------------- //
// Interrupt callback using HAL library
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if (GPIO_Pin == GPIO_PIN_13) {
    // TODO: add ISR code 
    // Simple debounce
    HAL_Delay(50);
  }
}

// ---------------------------------------- alternate ISR for Button Press -----------------------------//
// This function is automatically called when the EXTI15_10 interrupt occurs
void EXTI15_10_IRQHandler(void) {
  // Check if the interrupt is from PC13
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13) != RESET) {
    // Clear the interrupt flag
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
    
    // Your interrupt handling code goes here
    // For example, toggle an LED
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    
    // Add debounce delay if this is a button press
    HAL_Delay(50);
  }
}