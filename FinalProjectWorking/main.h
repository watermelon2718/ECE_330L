//header guard
#ifndef __MAIN_H 
#define __MAIN_H

//includes
#include "stm32f4xx_hal.h" // hardware
#include "Game.h" // game structures
#include "Music.h" // music constants and struct
#include <string.h> // strings for some reason
#include "Junk.h" // other

extern Game game; // make the main game visible globally

void Error_Handler(void);

#endif /* __MAIN_H */
