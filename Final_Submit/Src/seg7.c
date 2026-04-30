#include "seg7.h"

/* * 7-Segment Mapping (Standard Layout):
 * Segments: a, b, c, d, e, f, g, dp
 * GPIOE Bits: 0, 1, 2, 3, 4, 5, 6, 7
 */

#define COMMON_ANODE 1
/* Character set Common Anode */
#if COMMON_ANODE == 1

// Characters assigned numerically based on 0-9, A-Z, plus special characters
const unsigned char _7SEG[] =
		{~(0x3F),~(0x06),~(0x5B),~(0x4F),~(0x66),~(0x6D),~(0x7D),
		~(0x07),~(0x7F),~(0x6F),~(0x77),~(0x7C),~(0x39),~(0x5E),
		~(0x79),~(0x71),~(0x3D),~(0x76),~(0x06),~(0x1E),~(0x7A),
		~(0x38),~(0x15),~(0x54),~(0x3F),~(0x73),~(0x67),~(0x50),
		~(0x6D),~(0x78),~(0x3E),~(0x62),~(0x2A),~(0x64),~(0x6E),
		~(0x5B),~(0x80),~(0x48),~(0x01),~(0x02),~(0x04),~(0x08),
		~(0x10),~(0x20),~(0x40),~(0x00)};
		
// Characters assigned based on ASCII character codes		
const unsigned char _7SEG_ASCII[]=
{
	['0'] = ~(0x3F),
	['1'] = ~(0x06),
	['2'] = ~(0x5B),
	['3'] = ~(0x4F),
	['4'] = ~(0x66),
	['5'] = ~(0x6D),
	['6'] = ~(0x7D),
	['7'] = ~(0x07),
	['8'] = ~(0x7F),
	['9'] = ~(0x6F),
	['A'] = ~(0x77),
	['B'] = ~(0x7C),
	['C'] = ~(0x39),
	['D'] = ~(0x5E),
	['E'] = ~(0x79),
	['F'] = ~(0x71),
	['G'] = ~(0x3D),
	['H'] = ~(0x76),
	['I'] = ~(0x06),
	['J'] = ~(0x1E),
	['K'] = ~(0x7A),
	['L'] = ~(0x38),
	['M'] = ~(0x15),
	['N'] = ~(0x54),
	['O'] = ~(0x3F),
	['P'] = ~(0x73),
	['Q'] = ~(0x67),
	['R'] = ~(0x50),
	['S'] = ~(0x6D),
	['T'] = ~(0x78),
	['U'] = ~(0x3E),
	['V'] = ~(0x62),
	['W'] = ~(0x2A),
	['X'] = ~(0x64),
	['Y'] = ~(0x6E),
	['Z'] = ~(0x5B),
	['a'] = ~(0x77),
	['b'] = ~(0x7C),
	['c'] = ~(0x39),
	['d'] = ~(0x5E),
	['e'] = ~(0x79),
	['f'] = ~(0x71),
	['g'] = ~(0x3D),
	['h'] = ~(0x76),
	['i'] = ~(0x06),
	['j'] = ~(0x1E),
	['k'] = ~(0x7A),
	['l'] = ~(0x38),
	['m'] = ~(0x15),
	['n'] = ~(0x54),
	['o'] = ~(0x3F),
	['p'] = ~(0x73),
	['q'] = ~(0x67),
	['r'] = ~(0x50),
	['s'] = ~(0x6D),
	['t'] = ~(0x78),
	['u'] = ~(0x3E),
	['v'] = ~(0x62),
	['w'] = ~(0x2A),
	['x'] = ~(0x64),
	['y'] = ~(0x6E),
	['z'] = ~(0x5B),
	['.'] = ~(0x80),
	['='] = ~(0x48),
	[' '] = ~(0x00),
	['-'] = ~(0x20),
	['_'] = ~(0x08),
	[127] = 0,
};


#else

// Characters assigned numerically based on 0-9, A-Z, plus special characters
const unsigned char _7SEG[] =
		{(0x3F),(0x06),(0x5B),(0x4F),(0x66),(0x6D),(0x7D),
		(0x07),(0x7F),(0x6F),(0x77),(0x7C),(0x39),(0x5E),
		(0x79),(0x71),(0x3D),(0x76),(0x06),(0x1E),(0x7A),
		(0x38),(0x15),(0x54),(0x3F),(0x73),(0x67),(0x50),
		(0x6D),(0x78),(0x3E),(0x62),(0x2A),(0x64),(0x6E),
		(0x5B),(0x80),(0x48),(0x01),(0x02),(0x04),(0x08),
		(0x10),(0x20),(0x40),(0x00)};
		
// Characters assigned based on ASCII character codes		
const unsigned char _7SEG_ASCII[]=
{
	['0'] = (0x3F),
	['1'] = (0x06),
	['2'] = (0x5B),
	['3'] = (0x4F),
	['4'] = (0x66),
	['5'] = (0x6D),
	['6'] = (0x7D),
	['7'] = (0x07),
	['8'] = (0x7F),
	['9'] = (0x6F),
	['A'] = (0x77),
	['B'] = (0x7C),
	['C'] = (0x39),
	['D'] = (0x5E),
	['E'] = (0x79),
	['F'] = (0x71),
	['G'] = (0x3D),
	['H'] = (0x76),
	['I'] = (0x06),
	['J'] = (0x1E),
	['K'] = (0x7A),
	['L'] = (0x38),
	['M'] = (0x15),
	['N'] = (0x54),
	['O'] = (0x3F),
	['P'] = (0x73),
	['Q'] = (0x67),
	['R'] = (0x50),
	['S'] = (0x6D),
	['T'] = (0x78),
	['U'] = (0x3E),
	['V'] = (0x62),
	['W'] = (0x2A),
	['X'] = (0x64),
	['Y'] = (0x6E),
	['Z'] = (0x5B),
	['a'] = (0x77),
	['b'] = (0x7C),
	['c'] = (0x39),
	['d'] = (0x5E),
	['e'] = (0x79),
	['f'] = (0x71),
	['g'] = (0x3D),
	['h'] = (0x76),
	['i'] = (0x06),
	['j'] = (0x1E),
	['k'] = (0x7A),
	['l'] = (0x38),
	['m'] = (0x15),
	['n'] = (0x54),
	['o'] = (0x3F),
	['p'] = (0x73),
	['q'] = (0x67),
	['r'] = (0x50),
	['s'] = (0x6D),
	['t'] = (0x78),
	['u'] = (0x3E),
	['v'] = (0x62),
	['w'] = (0x2A),
	['x'] = (0x64),
	['y'] = (0x6E),
	['z'] = (0x5B),
	['.'] = (0x80),
	['='] = (0x48),
	[' '] = (0x00),
	['-'] = (0x20),
	['_'] = (0x08),
	[127] = 0,
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