//includes
#include "main.h"
#include "stm32f4xx_it.h"

extern HCD_HandleTypeDef hhcd_USB_OTG_FS;
extern TIM_HandleTypeDef htim7;

void NMI_Handler(void){while(1);}
void HardFault_Handler(void){while(1);}
void MemManage_Handler(void){while(1);}
void BusFault_Handler(void){while(1);}
void UsageFault_Handler(void){while(1);}
void SVC_Handler(void){}
void DebugMon_Handler(void){}
void PendSV_Handler(void){}

void SysTick_Handler(void){
    HAL_IncTick();
    MusicSystick(&(game.music));
    DisplayOutput(&game);
}
void TIM7_IRQHandler(void) {
  HAL_TIM_IRQHandler(&htim7);
  MusicTIM7Tick(&(game.music));
}



/**
  * @brief This function handles USB On The Go FS global interrupt.
  */
void OTG_FS_IRQHandler(void) {
  /* USER CODE BEGIN OTG_FS_IRQn 0 */

  /* USER CODE END OTG_FS_IRQn 0 */
  HAL_HCD_IRQHandler(&hhcd_USB_OTG_FS);
  /* USER CODE BEGIN OTG_FS_IRQn 1 */

  /* USER CODE END OTG_FS_IRQn 1 */
}
