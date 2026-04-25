#include "Player.h"

void PlayerInit(Player *p) {
    //initialize all arrays to 0
    memset(p->horizPlacementArray, 0, sizeof(p->horizPlacementArray));
    memset(p->vertPlacementArray,  0, sizeof(p->vertPlacementArray));
    memset(p->horizGuessArray,     0, sizeof(p->horizGuessArray));
    memset(p->vertGuessArray,      0, sizeof(p->vertGuessArray));

    p->singleSegmentShipsLeftToPlace = 3; // tracks how many ships are left to place
    p->doubleSegmentShipsLeftToPlace = 2;
}