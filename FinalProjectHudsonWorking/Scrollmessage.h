#ifndef SCROLLMESSAGE_
#define SCROLLMESSAGE_

#include "seg7.h"

typedef struct ScrollMessage {
    char *text;           // Pointer to the actual string (e.g., "PLAYER 1 WINS")
    char *currentPtr;     // Equivalent to your Message_Pointer
    char *startPtr;       // Equivalent to your Save_Pointer
    
    int length;           // Total length of the string
    int delayMsec;       // How many ticks to wait before shifting
    int delayCounter;    // Current tick count
    int animateOn;       // Boolean: 1 to scroll, 0 to stop
} ScrollMessage;

void SetMessage(ScrollMessage *m, char *str, int delay, int isAnimated); // setter to change message
void MessageInit(ScrollMessage *m, char *str, int delay, int isAnimated); // init
void DisplayScrollMessage(ScrollMessage *m); // output to gpio

#endif