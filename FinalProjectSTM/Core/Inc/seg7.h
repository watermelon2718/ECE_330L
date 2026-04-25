#ifndef __SEG7_H
#define __SEG7_H

#include "stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

// Prototypes
void Seven_Segment_Digit(unsigned char digit, unsigned char hex_char, unsigned char dot);
void Seven_Segment_ASCII(unsigned char digit, unsigned char ascii_char, unsigned char dot);
void Seven_Segment(unsigned int HexValue);

// Use extern so multiple files can include this header without "multiple definition" errors
extern const unsigned char _7SEG[];
extern const unsigned char _7SEG_ASCII[];

#define COMMON_ANODE 1

// Keep your existing #define CHAR_A, SPACE, etc. macros here...

#ifdef __cplusplus
}
#endif

#endif /* __SEG7_H */