/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"
#include "seg7.h"

I2C_HandleTypeDef hi2c1;
I2S_HandleTypeDef hi2s3;
SPI_HandleTypeDef hspi1;
TIM_HandleTypeDef htim7;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM7_Init(void);

static void Update_Display_From_Game(void);
static void Apply_Placement_Status_To_Display(void);
static void Read_Cursor_Inputs(void);
static uint16_t Read_ADC_Channel(uint8_t channel);
static void Clear_Displays(void);
static uint8_t pressed_once(uint16_t bitmask);

static uint16_t encode_position(uint8_t map_type, uint8_t row, uint8_t col, uint8_t segment);
static uint8_t is_set(uint32_t *bits, uint16_t index);
static void set_bit(uint32_t *bits, uint16_t index);
static uint8_t valid_single_placement(PlayerMaps *player, Cursor *c);
static void place_single_boat(PlayerMaps *player, Cursor *c);
static uint8_t valid_double_placement(PlayerMaps *player, Cursor *c);
static void place_double_boat(PlayerMaps *player, Cursor *c);
static uint8_t get_segment_symbol(uint8_t segment);

/* USER CODE BEGIN PV */
int DelayValue = 50;
/* USER CODE END PV */

/* Globals shared with interrupt file */
char ramp = 0;
char RED_BRT = 0;
char GREEN_BRT = 0;
char BLUE_BRT = 0;
char RED_STEP = 1;
char GREEN_STEP = 2;
char BLUE_STEP = 3;
char DIM_Enable = 0;
char Music_ON = 0;
int TONE = 0;
int COUNT = 0;
int INDEX = 0;
int Note = 0;
int Save_Note = 0;
int Vibrato_Depth = 1;
int Vibrato_Rate = 40;
int Vibrato_Count = 0;
char Animate_On = 0;
char Message_Length = 0;
char *Message_Pointer = 0;
char *Save_Pointer = 0;
int Delay_msec = 0;
int Delay_counter = 0;

volatile uint8_t display_scan_digit = 0;
volatile uint8_t pwm_step = 0;
volatile uint16_t blink_counter = 0;
volatile uint8_t ui_tick_flag = 0;

volatile uint8_t display_chars[8] = {SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE};
volatile uint8_t display_dots[8] = {0,0,0,0,0,0,0,0};
volatile uint8_t display_brightness[8] = {0,0,0,0,0,0,0,0};

GamePhase game_phase = PHASE_P1_PLACE;
PlayerMaps p1 = {{0},{0}};
PlayerMaps p2 = {{0},{0}};
Cursor cursor = {MAP_HORIZONTAL, 0, 0, SEG_OVER, 1};
PlacementState p1_place = {3,2};
PlacementState p2_place = {3,2};

Music Song[100];

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_TIM7_Init();

    GPIOD->MODER = 0x55555555;
    GPIOA->MODER |= 0x000000FF;   /* PA0-PA3 analog */
    GPIOE->MODER |= 0x55555555;   /* 7-seg outputs */
    GPIOC->MODER |= 0x00000000;   /* inputs */
    GPIOE->ODR = 0xFFFF;

    RCC->APB2ENR |= 1 << 8;       /* ADC1 clock enable */
    ADC1->SMPR2 |= 1;
    ADC1->CR2 |= 1;

    TIM7->PSC = 199;
    TIM7->ARR = 1;
    TIM7->DIER |= 1;
    TIM7->CR1 |= 1;

    Clear_Displays();
    Animate_On = 0;

    while (1)
    {
        if (ui_tick_flag)
        {
            ui_tick_flag = 0;
            Read_Cursor_Inputs();
            Update_Display_From_Game();
        }

        switch (game_phase)
        {
            case PHASE_P1_PLACE:
                if (pressed_once(1 << 10))
                {
                    if (p1_place.singles_left > 0U)
                    {
                        if (valid_single_placement(&p1, &cursor))
                        {
                            place_single_boat(&p1, &cursor);
                            p1_place.singles_left--;
                        }
                    }
                    else if (p1_place.doubles_left > 0U)
                    {
                        if (valid_double_placement(&p1, &cursor))
                        {
                            place_double_boat(&p1, &cursor);
                            p1_place.doubles_left--;
                        }
                    }

                    if ((p1_place.singles_left == 0U) && (p1_place.doubles_left == 0U))
                    {
                        game_phase = PHASE_P2_PLACE;
                    }
                }
                break;

            case PHASE_P2_PLACE:
                if (pressed_once(1 << 10))
                {
                    if (p2_place.singles_left > 0U)
                    {
                        if (valid_single_placement(&p2, &cursor))
                        {
                            place_single_boat(&p2, &cursor);
                            p2_place.singles_left--;
                        }
                    }
                    else if (p2_place.doubles_left > 0U)
                    {
                        if (valid_double_placement(&p2, &cursor))
                        {
                            place_double_boat(&p2, &cursor);
                            p2_place.doubles_left--;
                        }
                    }

                    if ((p2_place.singles_left == 0U) && (p2_place.doubles_left == 0U))
                    {
                        game_phase = PHASE_P1_TURN;
                    }
                }
                break;

            case PHASE_P1_TURN:
                break;

            case PHASE_P2_TURN:
                break;

            default:
                game_phase = PHASE_P1_PLACE;
                break;
        }
    }
}

static uint8_t get_segment_symbol(uint8_t segment)
{
    switch (segment)
    {
        case SEG_OVER:        return OVER;
        case SEG_UPPER_LEFT:  return UPPER_LEFT;
        case SEG_UPPER_RIGHT: return UPPER_RIGHT;
        case SEG_DASH:        return DASH;
        case SEG_LOWER_LEFT:  return LOWER_LEFT;
        case SEG_LOWER_RIGHT: return LOWER_RIGHT;
        case SEG_UNDER:       return UNDER;
        default:              return SPACE;
    }
}

static uint16_t encode_position(uint8_t map_type, uint8_t row, uint8_t col, uint8_t segment)
{
    if (map_type == MAP_HORIZONTAL)
    {
        return (uint16_t)(((row * 8U + col) * 7U) + segment);
    }
    else
    {
        return (uint16_t)(168U + (((row * 8U + col) * 7U) + segment));
    }
}

static uint8_t is_set(uint32_t *bits, uint16_t index)
{
    return (bits[index / 32U] >> (index % 32U)) & 1U;
}

static void set_bit(uint32_t *bits, uint16_t index)
{
    bits[index / 32U] |= (1U << (index % 32U));
}

static uint8_t valid_single_placement(PlayerMaps *player, Cursor *c)
{
    uint16_t idx = encode_position(c->map_type, c->row, c->col, c->segment);
    return !is_set(player->boat_bits, idx);
}

static void place_single_boat(PlayerMaps *player, Cursor *c)
{
    uint16_t idx = encode_position(c->map_type, c->row, c->col, c->segment);
    set_bit(player->boat_bits, idx);
}

static uint8_t valid_double_placement(PlayerMaps *player, Cursor *c)
{
    uint16_t idx1, idx2;

    idx1 = encode_position(c->map_type, c->row, c->col, c->segment);

    if (c->map_type == MAP_HORIZONTAL)
    {
        if (c->col >= 7U)
            return 0U;

        idx2 = encode_position(MAP_HORIZONTAL, c->row, c->col + 1U, c->segment);
    }
    else
    {
        if (c->row == 0U)
        {
            if (c->segment == SEG_UPPER_LEFT)
                idx2 = encode_position(MAP_VERTICAL, c->row, c->col, SEG_UPPER_RIGHT);
            else if (c->segment == SEG_UPPER_RIGHT)
                idx2 = encode_position(MAP_VERTICAL, c->row, c->col, SEG_UPPER_LEFT);
            else
                return 0U;
        }
        else
        {
            if (c->segment == SEG_LOWER_LEFT)
                idx2 = encode_position(MAP_VERTICAL, c->row, c->col, SEG_LOWER_RIGHT);
            else if (c->segment == SEG_LOWER_RIGHT)
                idx2 = encode_position(MAP_VERTICAL, c->row, c->col, SEG_LOWER_LEFT);
            else
                return 0U;
        }
    }

    if (is_set(player->boat_bits, idx1)) return 0U;
    if (is_set(player->boat_bits, idx2)) return 0U;

    return 1U;
}

static void place_double_boat(PlayerMaps *player, Cursor *c)
{
    uint16_t idx1, idx2;

    idx1 = encode_position(c->map_type, c->row, c->col, c->segment);

    if (c->map_type == MAP_HORIZONTAL)
    {
        idx2 = encode_position(MAP_HORIZONTAL, c->row, c->col + 1U, c->segment);
    }
    else
    {
        if (c->row == 0U)
        {
            if (c->segment == SEG_UPPER_LEFT)
                idx2 = encode_position(MAP_VERTICAL, c->row, c->col, SEG_UPPER_RIGHT);
            else
                idx2 = encode_position(MAP_VERTICAL, c->row, c->col, SEG_UPPER_LEFT);
        }
        else
        {
            if (c->segment == SEG_LOWER_LEFT)
                idx2 = encode_position(MAP_VERTICAL, c->row, c->col, SEG_LOWER_RIGHT);
            else
                idx2 = encode_position(MAP_VERTICAL, c->row, c->col, SEG_LOWER_LEFT);
        }
    }

    set_bit(player->boat_bits, idx1);
    set_bit(player->boat_bits, idx2);
}

static void Apply_Placement_Status_To_Display(void)
{
    PlacementState *ps;

    if (game_phase == PHASE_P1_PLACE)
        ps = &p1_place;
    else if (game_phase == PHASE_P2_PLACE)
        ps = &p2_place;
    else
        return;

    display_chars[0] = CHAR_S;
    display_chars[1] = CHAR_0 + ps->singles_left;
    display_chars[2] = CHAR_D;
    display_chars[3] = CHAR_0 + ps->doubles_left;
}

static void Update_Display_From_Game(void)
{
    uint8_t col, seg;
    uint16_t idx;
    uint8_t draw_digit;
    uint8_t placed_count[8] = {0,0,0,0,0,0,0,0};
    PlayerMaps *active_player;
    uint8_t view_col;

    active_player = (game_phase == PHASE_P2_PLACE) ? &p2 : &p1;

    for (col = 0; col < 8; col++)
    {
        display_chars[col] = SPACE;
        display_dots[col] = 0;
        display_brightness[col] = 10;
    }

    /* rightmost 4 digits are map view */
    for (view_col = 0; view_col < 4; view_col++)
    {
        col = view_col;
        draw_digit = 7 - view_col;

        if (cursor.map_type == MAP_HORIZONTAL)
        {
            for (seg = 0; seg < 7; seg++)
            {
                idx = encode_position(MAP_HORIZONTAL, cursor.row, col, seg);
                if (is_set(active_player->boat_bits, idx))
                {
                    placed_count[draw_digit]++;

                    if (placed_count[draw_digit] == 1)
                        display_chars[draw_digit] = get_segment_symbol(seg);
                    else
                        display_chars[draw_digit] = EQUAL;
                }
            }
        }
        else
        {
            for (seg = 0; seg < 7; seg++)
            {
                idx = encode_position(MAP_VERTICAL, cursor.row, col, seg);
                if (is_set(active_player->boat_bits, idx))
                {
                    placed_count[draw_digit]++;

                    if (placed_count[draw_digit] == 1)
                        display_chars[draw_digit] = get_segment_symbol(seg);
                    else
                        display_chars[draw_digit] = EQUAL;
                }
            }
        }
    }

    /* cursor only visible if inside the shown 4-digit window */
    if (cursor.col < 4U)
    {
        draw_digit = 7 - cursor.col;
        if (cursor.blink_on)
        {
            display_chars[draw_digit] = get_segment_symbol(cursor.segment);
        }
    }

    Apply_Placement_Status_To_Display();
}

static void Read_Cursor_Inputs(void)
{
    uint16_t x_adc, y_adc;
    uint8_t pos;

    x_adc = Read_ADC_Channel(1);
    y_adc = Read_ADC_Channel(2);

    cursor.map_type = (GPIOC->IDR & (1 << 9)) ? MAP_VERTICAL : MAP_HORIZONTAL;

    if (cursor.map_type == MAP_HORIZONTAL)
    {
        cursor.row = (uint8_t)((y_adc * 3U) / 4096U);
        if (cursor.row > 2U) cursor.row = 2U;

        cursor.col = (uint8_t)((x_adc * 8U) / 4096U);
        if (cursor.col > 7U) cursor.col = 7U;

        if (cursor.row == 0U)
            cursor.segment = SEG_OVER;
        else if (cursor.row == 1U)
            cursor.segment = SEG_DASH;
        else
            cursor.segment = SEG_UNDER;
    }
    else
    {
        cursor.row = (y_adc > 2048U) ? 1U : 0U;

        pos = (uint8_t)((x_adc * 16U) / 4096U);
        if (pos > 15U) pos = 15U;

        cursor.col = pos / 2U;

        if (cursor.row == 0U)
            cursor.segment = (pos % 2U) ? SEG_UPPER_RIGHT : SEG_UPPER_LEFT;
        else
            cursor.segment = (pos % 2U) ? SEG_LOWER_RIGHT : SEG_LOWER_LEFT;
    }
}

static uint16_t Read_ADC_Channel(uint8_t channel)
{
    ADC1->SQR3 = channel;
    ADC1->CR2 |= (1 << 30);
    while (!(ADC1->SR & (1 << 1))) {}
    return (uint16_t)ADC1->DR;
}

static void Clear_Displays(void)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        display_chars[i] = SPACE;
        display_dots[i] = 0;
        display_brightness[i] = 0;
    }
}

static uint8_t pressed_once(uint16_t bitmask)
{
    static uint16_t old_state = 0;
    static uint8_t initialized = 0;
    uint16_t new_state = GPIOC->IDR & bitmask;

    if (!initialized)
    {
        old_state = GPIOC->IDR;
        initialized = 1;
        return 0U;
    }

    if (new_state && !(old_state & bitmask))
    {
        old_state |= bitmask;
        return 1U;
    }
    else if (!new_state)
    {
        old_state &= (uint16_t)(~bitmask);
    }

    return 0U;
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }
}

static void MX_TIM7_Init(void)
{
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    htim7.Instance = TIM7;
    htim7.Init.Prescaler = 0;
    htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim7.Init.Period = 65535;
    htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
    {
        Error_Handler();
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
}

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, LD4_Pin | LD3_Pin | LD5_Pin | LD6_Pin | Audio_RST_Pin, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = CS_I2C_SPI_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(CS_I2C_SPI_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
    HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = PDM_OUT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(PDM_OUT_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = B1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = BOOT1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = CLK_IN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(CLK_IN_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LD4_Pin | LD3_Pin | LD5_Pin | LD6_Pin | Audio_RST_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MEMS_INT2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
    HAL_GPIO_Init(MEMS_INT2_GPIO_Port, &GPIO_InitStruct);
}

void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif
