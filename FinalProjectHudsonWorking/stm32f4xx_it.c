//includes
#include "main.h"
#include "stm32f4xx_it.h"

extern HCD_HandleTypeDef hhcd_USB_OTG_FS;
extern TIM_HandleTypeDef htim7;

void NMI_Handler(void){while (1){}}
void HardFault_Handler(void){while (1)}
void MemManage_Handler(void){while(1)}
void BusFault_Handler(void){while(1)}
void UsageFault_Handler(void){while(1)}
void SVC_Handler(void){}
void DebugMon_Handler(void){}
void PendSV_Handler(void){}

void SysTick_Handler(void){
  HAL_IncTick();
  MusicSystick(&(game.music));
  RefreshDisplay(&game);

  if (Animate_On > 0)
  {
    Delay_counter++;
    if (Delay_counter > Delay_msec)
    {
      Delay_counter = 0;
      Seven_Segment_Digit(7,*(Message_Pointer),0);
      Seven_Segment_Digit(6,*(Message_Pointer+1),0);
      Seven_Segment_Digit(5,*(Message_Pointer+2),0);
      Seven_Segment_Digit(4,*(Message_Pointer+3),0);
      Seven_Segment_Digit(3,*(Message_Pointer+4),0);
      Seven_Segment_Digit(2,*(Message_Pointer+5),0);
      Seven_Segment_Digit(1,*(Message_Pointer+6),0);
      Seven_Segment_Digit(0,*(Message_Pointer+7),0);
      Message_Pointer++;
      if ((Message_Pointer - Save_Pointer) >= (Message_Length-8)) Message_Pointer = Save_Pointer;

    }
  }
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
