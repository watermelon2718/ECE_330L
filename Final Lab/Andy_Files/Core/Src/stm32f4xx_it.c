/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
#include "seg7.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static void Refresh_One_Display_Digit(void)
{
    uint8_t digit = display_scan_digit;

    if (display_brightness[digit] > pwm_step)
    {
        Seven_Segment_Digit(digit, display_chars[digit], display_dots[digit]);
    }
    else
    {
        Seven_Segment_Digit(digit, SPACE, 0);
    }

    display_scan_digit++;
    if (display_scan_digit >= 8)
    {
        display_scan_digit = 0;
    }
}

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern HCD_HandleTypeDef hhcd_USB_OTG_FS;
extern TIM_HandleTypeDef htim7;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/

void NMI_Handler(void)
{
  while (1)
  {
  }
}

void HardFault_Handler(void)
{
  while (1)
  {
  }
}

void MemManage_Handler(void)
{
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  while (1)
  {
  }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

    /* Lab 6 / Lab 11 note timing support */
    COUNT++;
    Vibrato_Count++;

    /* Vibrato effect */
    if (Vibrato_Count >= Vibrato_Rate)
    {
        Vibrato_Count = 0;
        if (Song[INDEX].note > 0)
        {
            Song[INDEX].note += Vibrato_Depth;
            if (Song[INDEX].note > (Save_Note + Vibrato_Depth))
            {
                Song[INDEX].note = Save_Note - Vibrato_Depth;
            }
        }
    }

    /* Marquee pointer update */
    if (Animate_On > 0)
    {
        Delay_counter++;
        if (Delay_counter > Delay_msec)
        {
            Delay_counter = 0;
            Message_Pointer++;

            if ((Message_Pointer - Save_Pointer) >= (Message_Length - 8))
            {
                Message_Pointer = Save_Pointer;
            }
        }
    }

    /* Cursor blink timing */
    blink_counter++;
    if (blink_counter >= 250)
    {
        blink_counter = 0;
        cursor.blink_on ^= 1;
    }

    /* 10-step software PWM for display brightness */
    pwm_step++;
    if (pwm_step >= 10)
    {
        pwm_step = 0;
    }

    /* Refresh one 7-seg digit every SysTick */
    Refresh_One_Display_Digit();

    /* Tell main loop it is time to update UI/game display state */
    ui_tick_flag = 1;

  /* USER CODE END SysTick_IRQn 0 */
    HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/******************************************************************************/

/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */

    /* Increment tone counter and PWM ramp counter */
    TONE++;
    ramp++;

    /* Song playback */
    if ((Music_ON > 0) && (Song[INDEX].note > 0) &&
        ((Song[INDEX].tempo / Song[INDEX].size - Song[INDEX].space) > COUNT))
    {
        if (Song[INDEX].note <= TONE)
        {
            GPIOD->ODR ^= 1;
            TONE = 0;
        }
    }
    else if ((Music_ON > 0) && (Song[INDEX].tempo / Song[INDEX].size > COUNT))
    {
        TONE = 0;
    }
    else if ((Music_ON > 0) && (Song[INDEX].tempo / Song[INDEX].size == COUNT))
    {
        COUNT = 0;
        TONE = 0;

        if (!(Song[INDEX].end))
        {
            INDEX++;
            Save_Note = Song[INDEX].note;
        }
        else
        {
            Music_ON = 0;
            INDEX = 0;
            Save_Note = Song[0].note;
        }
    }
    else if (Music_ON == 0)
    {
        TONE = 0;
        COUNT = 0;
    }

    /* Optional RGB LED PWM dimming */
    if (DIM_Enable > 0)
    {
        if (RED_BRT <= ramp)
        {
            GPIOD->ODR |= (1 << 15);
        }
        else
        {
            GPIOD->ODR &= ~(1 << 15);
        }

        if (BLUE_BRT <= ramp)
        {
            GPIOD->ODR |= (1 << 14);
        }
        else
        {
            GPIOD->ODR &= ~(1 << 14);
        }

        if (GREEN_BRT <= ramp)
        {
            GPIOD->ODR |= (1 << 13);
        }
        else
        {
            GPIOD->ODR &= ~(1 << 13);
        }
    }

  /* USER CODE END TIM7_IRQn 0 */
    HAL_TIM_IRQHandler(&htim7);
  /* USER CODE BEGIN TIM7_IRQn 1 */

  /* USER CODE END TIM7_IRQn 1 */
}

/**
  * @brief This function handles USB On The Go FS global interrupt.
  */
void OTG_FS_IRQHandler(void)
{
  HAL_HCD_IRQHandler(&hhcd_USB_OTG_FS);
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
