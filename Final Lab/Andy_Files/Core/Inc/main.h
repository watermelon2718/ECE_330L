/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  ******************************************************************************
  */
/* USER CODE END Header */

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

/* ---------- Music / sound structure ---------- */
typedef struct music {
    int note;
    int size;
    int tempo;
    int space;
    char end;
} Music;

/* ---------- Game phases ---------- */
typedef enum {
    PHASE_P1_PLACE = 0,
    PHASE_P2_PLACE,
    PHASE_P1_TURN,
    PHASE_P2_TURN,
    PHASE_P1_WIN,
    PHASE_P2_WIN
} GamePhase;

/* ---------- Map type ---------- */
typedef enum {
    MAP_HORIZONTAL = 0,
    MAP_VERTICAL
} MapType;

/* ---------- Segment positions inside one digit ---------- */
typedef enum {
    SEG_OVER = 0,
    SEG_UPPER_LEFT,
    SEG_UPPER_RIGHT,
    SEG_DASH,
    SEG_LOWER_LEFT,
    SEG_LOWER_RIGHT,
    SEG_UNDER
} SegmentPos;

/* ---------- Segment-level cursor ---------- */
typedef struct {
    uint8_t map_type;   /* MAP_HORIZONTAL or MAP_VERTICAL */
    uint8_t row;        /* H: 0..2, V: 0..1 */
    uint8_t col;        /* H: 0..7, V: 0..15 */
    uint8_t segment;    /* one of SegmentPos */
    uint8_t blink_on;
} Cursor;

/* ---------- Bitfield storage ---------- */
/* total positions:
   horizontal = 3*8*7 = 168
   vertical   = 2*16*7 = 224
   total      = 392 bits
   13 * 32 = 416 bits total storage
*/
typedef struct {
    uint32_t boat_bits[13];
    uint32_t shot_bits[13];
} PlayerMaps;

typedef struct {
    uint8_t singles_left;
    uint8_t doubles_left;
} PlacementState;

/* ---------- externs used by stm32f4xx_it.c ---------- */
extern char ramp;
extern char RED_BRT;
extern char GREEN_BRT;
extern char BLUE_BRT;
extern char RED_STEP;
extern char GREEN_STEP;
extern char BLUE_STEP;
extern char DIM_Enable;
extern int TONE;
extern int COUNT;
extern int INDEX;
extern Music Song[100];
extern int Note;
extern int Save_Note;
extern int Vibrato_Depth;
extern int Vibrato_Rate;
extern int Vibrato_Count;
extern char Animate_On;
extern char Music_ON;
extern char Message_Length;
extern char *Save_Pointer;
extern char *Message_Pointer;
extern int Delay_msec;
extern int Delay_counter;

/* ---------- display globals ---------- */
extern volatile uint8_t display_scan_digit;
extern volatile uint8_t pwm_step;
extern volatile uint16_t blink_counter;
extern volatile uint8_t ui_tick_flag;

extern volatile uint8_t display_chars[8];
extern volatile uint8_t display_dots[8];
extern volatile uint8_t display_brightness[8];
extern volatile uint8_t display_masks[8];

/* ---------- game globals ---------- */
extern GamePhase game_phase;
extern PlayerMaps p1;
extern PlayerMaps p2;
extern Cursor cursor;
extern PlacementState p1_place;
extern PlacementState p2_place;

void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/
#define CS_I2C_SPI_Pin GPIO_PIN_3
#define CS_I2C_SPI_GPIO_Port GPIOE
#define PC14_OSC32_IN_Pin GPIO_PIN_14
#define PC14_OSC32_IN_GPIO_Port GPIOC
#define PC15_OSC32_OUT_Pin GPIO_PIN_15
#define PC15_OSC32_OUT_GPIO_Port GPIOC
#define PH0_OSC_IN_Pin GPIO_PIN_0
#define PH0_OSC_IN_GPIO_Port GPIOH
#define PH1_OSC_OUT_Pin GPIO_PIN_1
#define PH1_OSC_OUT_GPIO_Port GPIOH
#define OTG_FS_PowerSwitchOn_Pin GPIO_PIN_0
#define OTG_FS_PowerSwitchOn_GPIO_Port GPIOC
#define PDM_OUT_Pin GPIO_PIN_3
#define PDM_OUT_GPIO_Port GPIOC
#define B1_Pin GPIO_PIN_0
#define B1_GPIO_Port GPIOA
#define I2S3_WS_Pin GPIO_PIN_4
#define I2S3_WS_GPIO_Port GPIOA
#define SPI1_SCK_Pin GPIO_PIN_5
#define SPI1_SCK_GPIO_Port GPIOA
#define SPI1_MISO_Pin GPIO_PIN_6
#define SPI1_MISO_GPIO_Port GPIOA
#define SPI1_MOSI_Pin GPIO_PIN_7
#define SPI1_MOSI_GPIO_Port GPIOA
#define BOOT1_Pin GPIO_PIN_2
#define BOOT1_GPIO_Port GPIOB
#define CLK_IN_Pin GPIO_PIN_10
#define CLK_IN_GPIO_Port GPIOB
#define LD4_Pin GPIO_PIN_12
#define LD4_GPIO_Port GPIOD
#define LD3_Pin GPIO_PIN_13
#define LD3_GPIO_Port GPIOD
#define LD5_Pin GPIO_PIN_14
#define LD5_GPIO_Port GPIOD
#define LD6_Pin GPIO_PIN_15
#define LD6_GPIO_Port GPIOD
#define I2S3_MCK_Pin GPIO_PIN_7
#define I2S3_MCK_GPIO_Port GPIOC
#define VBUS_FS_Pin GPIO_PIN_9
#define VBUS_FS_GPIO_Port GPIOA
#define OTG_FS_ID_Pin GPIO_PIN_10
#define OTG_FS_ID_GPIO_Port GPIOA
#define OTG_FS_DM_Pin GPIO_PIN_11
#define OTG_FS_DM_GPIO_Port GPIOA
#define OTG_FS_DP_Pin GPIO_PIN_12
#define OTG_FS_DP_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define I2S3_SCK_Pin GPIO_PIN_10
#define I2S3_SCK_GPIO_Port GPIOC
#define I2S3_SD_Pin GPIO_PIN_12
#define I2S3_SD_GPIO_Port GPIOC
#define Audio_RST_Pin GPIO_PIN_4
#define Audio_RST_GPIO_Port GPIOD
#define OTG_FS_OverCurrent_Pin GPIO_PIN_5
#define OTG_FS_OverCurrent_GPIO_Port GPIOD
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define Audio_SCL_Pin GPIO_PIN_6
#define Audio_SCL_GPIO_Port GPIOB
#define Audio_SDA_Pin GPIO_PIN_9
#define Audio_SDA_GPIO_Port GPIOB
#define MEMS_INT2_Pin GPIO_PIN_1
#define MEMS_INT2_GPIO_Port GPIOE

/* note constants */
#define C0 3823
#define Cs0_Db0 3608
#define D0 3405
#define Ds0_Eb0 3214
#define E0 3034
#define F0 2864
#define Fs0_Gb0 2703
#define G0 2551
#define Gs0_Ab0 2408
#define A0 2273
#define As0_Bb0 2145
#define B0 2025

#define C1 1911
#define Cs1_Db1 1803
#define D1 1703
#define Ds1_Eb1 1607
#define E1 1517
#define F1 1432
#define Fs1_Gb1 1351
#define G1 1275
#define Gs1_Ab1 1204
#define A1 1136
#define As1_Bb1 1073
#define B1 1012

#define C2 956
#define Cs2_Db2 902
#define D2 851
#define Ds2_Eb2 804
#define E2 758
#define F2 716
#define Fs2_Gb2 676
#define G2 638
#define Gs2_Ab2 602
#define A2 568
#define As2_Bb2 536
#define B2 506

#define C3 478
#define Cs3_Db3 451
#define D3 426
#define Ds3_Eb3 402
#define E3 379
#define F3 358
#define Fs3_Gb3 338
#define G3 319
#define Gs3_Ab3 301
#define A3 284
#define As3_Bb3 268
#define B3 253

#define C4 239
#define Cs4_Db4 225
#define D4 213
#define Ds4_Eb4 201
#define E4 190
#define F4 179
#define Fs4_Gb4 169
#define G4 159
#define Gs4_Ab4 150
#define A4 142
#define As4_Bb4 134
#define B4 127

#define C5 119
#define Cs5_Db5 113
#define D5 106
#define Ds5_Eb5 100
#define E5 95
#define F5 89
#define Fs5_Gb5 84
#define G5 80
#define Gs5_Ab5 75
#define A5 71
#define As5_Bb5 67
#define B5 63

#define C6 60
#define Cs6_Db6 56
#define D6 53
#define Ds6_Eb6 50
#define E6 47
#define F6 45
#define Fs6_Gb6 42
#define G6 40
#define Gs6_Ab6 38
#define A6 36
#define As6_Bb6 34
#define B6 32

#define whole 1
#define half 2
#define quarter 4
#define _8th 8
#define _16th 16
#define _32nd 32
#define rest 0

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
