
#include "ScrollMessage.h"

void SetMessage(ScrollMessage *m, char *str, int delay, int isAnimated) {
    m->text = str;
    m->startPtr = str;
    m->currentPtr = str;
    m->length = strlen(str);
    m->delayMsec = delay;
    m->delayCounter = 0;
    m->animateOn = isAnimated;
}

void MessageInit(ScrollMessage *m, char *str, int delay, int isAnimated) {
    SetMessage(m, str, delay, isAnimated);
}

void DisplayScrollMessage(ScrollMessage *m) {
    if (m->animateOn > 0) {
        m->delayCounter++;
        
        // Only shift the text when the specified delay has passed
        if (m->delayCounter > m->delayMsec) {
            m->delayCounter = 0;

            // Iterate through the 8 physical digits of the display
            for (int i = 0; i < 8; i++) {
                // Determine the logical index in the string for the current digit
                int charOffset = (m->currentPtr - m->startPtr) + i;
                
                if (charOffset < m->length) {
                    // Current digit falls within string bounds; display character
                    // Digit 7 is leftmost, Digit 0 is rightmost
                    Seven_Segment_ASCII(7 - i, *(m->currentPtr + i), 0);
                } else {
                    // Past the end of string; "virtual padding" provides blank space
                    Seven_Segment_ASCII(7 - i, ' ', 0); 
                }
            }

            // Move the window one character to the right
            m->currentPtr++;

            // Loop back logic: Restart once the text has completely scrolled off-screen
            // (i.e., the start of the window has passed the end of the string)
            if ((m->currentPtr - m->startPtr) >= m->length) {
                m->currentPtr = m->startPtr; 
            }
        }
    } else {
        // STATIC MODE: Display the first 8 characters without movement
        for (int i = 0; i < 8; i++) {
            if (i < m->length) {
                Seven_Segment_ASCII(7 - i, m->text[i], 0);
            } else {
                // Pad with spaces if the static string is shorter than 8 chars
                Seven_Segment_ASCII(7 - i, ' ', 0);
            }
        }
    }
}