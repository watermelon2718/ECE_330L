#include "seg7.h"

/* * 7-Segment Mapping (Standard Layout):
 * Segments: a, b, c, d, e, f, g, dp
 * GPIOE Bits: 0, 1, 2, 3, 4, 5, 6, 7
 */

#if COMMON_ANODE == 1
    // Characters 0-F and special symbols
    const unsigned char _7SEG[] = {
        ~(0x3F), ~(0x06), ~(0x5B), ~(0x4F), ~(0x66), ~(0x6D), ~(0x7D), // 0-6
        ~(0x07), ~(0x7F), ~(0x6F), ~(0x77), ~(0x7C), ~(0x39), ~(0x5E), // 7-D
        ~(0x79), ~(0x71), ~(0x3D), ~(0x76), ~(0x06), ~(0x1E), ~(0x7A), // E-K
        ~(0x38), ~(0x15), ~(0x54), ~(0x3F), ~(0x73), ~(0x67), ~(0x50), // L-R
        ~(0x6D), ~(0x78), ~(0x3E), ~(0x62), ~(0x2A), ~(0x64), ~(0x6E), // S-Y
        ~(0x5B), ~(0x80), ~(0x48), ~(0x01), ~(0x02), ~(0x04), ~(0x08), // Z, ., =, a, b, c, d
        ~(0x10), ~(0x20), ~(0x40), ~(0x00)                            // e, f, g, space
    };

    // Full ASCII table for characters
    const unsigned char _7SEG_ASCII[256] = {
        ['0'] = ~(0x3F), ['1'] = ~(0x06), ['2'] = ~(0x5B), ['3'] = ~(0x4F),
        ['4'] = ~(0x66), ['5'] = ~(0x6D), ['6'] = ~(0x7D), ['7'] = ~(0x07),
        ['8'] = ~(0x7F), ['9'] = ~(0x6F), 
        ['A'] = ~(0x77), ['a'] = ~(0x77),
        ['B'] = ~(0x7C), ['b'] = ~(0x7C),
        ['C'] = ~(0x39), ['c'] = ~(0x39),
        ['D'] = ~(0x5E), ['d'] = ~(0x5E),
        ['E'] = ~(0x79), ['e'] = ~(0x79),
        ['F'] = ~(0x71), ['f'] = ~(0x71),
        ['G'] = ~(0x3D), ['g'] = ~(0x3D),
        ['H'] = ~(0x76), ['h'] = ~(0x76),
        ['I'] = ~(0x06), ['i'] = ~(0x06),
        ['J'] = ~(0x1E), ['j'] = ~(0x1E),
        ['L'] = ~(0x38), ['l'] = ~(0x38),
        ['N'] = ~(0x54), ['n'] = ~(0x54),
        ['O'] = ~(0x3F), ['o'] = ~(0x3F),
        ['P'] = ~(0x73), ['p'] = ~(0x73),
        ['R'] = ~(0x50), ['r'] = ~(0x50),
        ['S'] = ~(0x6D), ['s'] = ~(0x6D),
        ['U'] = ~(0x3E), ['u'] = ~(0x3E),
        ['Y'] = ~(0x6E), ['y'] = ~(0x6E),
        ['.'] = ~(0x80), ['='] = ~(0x48), [' '] = ~(0x00),
        ['-'] = ~(0x40), ['_'] = ~(0x08)
    };
#else
    // Common Cathode Definitions
    const unsigned char _7SEG[] = {
        0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 
        0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x3D, 0x76, 0x06, 0x1E, 
        0x7A, 0x38, 0x15, 0x54, 0x3F, 0x73, 0x67, 0x50, 0x6D, 0x78, 
        0x3E, 0x62, 0x2A, 0x64, 0x6E, 0x5B, 0x80, 0x48, 0x01, 0x02, 
        0x04, 0x08, 0x10, 0x20, 0x40, 0x00
    };

    const unsigned char _7SEG_ASCII[256] = {
        ['0'] = 0x3F, ['1'] = 0x06, ['2'] = 0x5B, ['3'] = 0x4F,
        ['4'] = 0x66, ['5'] = 0x6D, ['6'] = 0x7D, ['7'] = 0x07,
        ['8'] = 0x7F, ['9'] = 0x6F, ['A'] = 0x77, ['B'] = 0x7C,
        ['C'] = 0x39, ['D'] = 0x5E, ['E'] = 0x79, ['F'] = 0x71,
        [' '] = 0x00, ['.'] = 0x80, ['-'] = 0x40
    };
#endif

/**
 * @brief Outputs a single hexadecimal character to a specific digit.
 */
void Seven_Segment_Digit(unsigned char digit, unsigned char hex_char, unsigned char dot) {
    if (digit > 7) return; // Prevent out-of-bounds shifting

    // Mask upper bits, apply character pattern, and clear the bit for the specific digit
    // The ~(1 << (digit + 8)) selects which of the 8 digits to turn on.
    GPIOE->ODR = (0xFF00 | _7SEG[hex_char & 0x3F]) & ~(1 << (digit + 8));
    
    if (dot > 0) {
        #if COMMON_ANODE == 1
            GPIOE->ODR &= ~(1 << 7); // Set DP pin low
        #else
            GPIOE->ODR |= (1 << 7);  // Set DP pin high
        #endif
    }

    // Latch high to prevent ghosting (Commonly used in multiplexed displays)
    GPIOE->ODR |= 0xFF00;
}

/**
 * @brief Outputs an ASCII character to a specific digit.
 */
void Seven_Segment_ASCII(unsigned char digit, unsigned char ascii_char, unsigned char dot) {
    if (digit > 7) return;

    GPIOE->ODR = (0xFF00 | _7SEG_ASCII[ascii_char]) & ~(1 << (digit + 8));
    
    if (dot > 0) {
        #if COMMON_ANODE == 1
            GPIOE->ODR &= ~(1 << 7);
        #else
            GPIOE->ODR |= (1 << 7);
        #endif
    }

    GPIOE->ODR |= 0xFF00;
}

/**
 * @brief Displays an 8-digit hexadecimal value across the full display.
 */
void Seven_Segment(unsigned int HexValue) {
    for (int digit = 0; digit < 8; digit++) {
       // Shift hex value and mask for 4 bits (0-F)
       Seven_Segment_Digit(digit, (HexValue >> (digit * 4)) & 0xF, 0);
    }
}