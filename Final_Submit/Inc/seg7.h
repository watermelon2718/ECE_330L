#ifndef __SEG7_H
#define __SEG7_H

#include "stm32f4xx_hal.h" // include hardware interface

#ifdef __cplusplus
extern "C" {
#endif

// Prototypes
void Seven_Segment_Digit(unsigned char digit, unsigned char hex_char, unsigned char dot); // displays digit
void Seven_Segment_ASCII(unsigned char digit, unsigned char ascii_char, unsigned char dot); // displays ascii
void Seven_Segment(unsigned int HexValue); // displays across multiple displays, unused

// Use extern so multiple files can include this header without "multiple definition" errors
extern const unsigned char _7SEG[]; // seven segment codes
extern const unsigned char _7SEG_ASCII[]; // seven segment ascii codes

#define COMMON_ANODE 1 // define mode of operation

// Keep your existing #define CHAR_A, SPACE, etc. macros here...

#ifdef __cplusplus
}
#endif

#endif /* __SEG7_H */