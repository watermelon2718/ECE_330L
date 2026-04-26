#include "Player.h"

//player struct initializer
void PlayerInit(Player *p) {
    //initialize all arrays to 0
    memset(p->horizPlacementArray, 0, sizeof(p->horizPlacementArray));
    memset(p->vertPlacementArray,  0, sizeof(p->vertPlacementArray));
    memset(p->horizGuessArray,     0, sizeof(p->horizGuessArray));
    memset(p->vertGuessArray,      0, sizeof(p->vertGuessArray));

    p->shipsToPlace = 5; // you start with 5 ships

    p->hitCount = 0; // no ships hit yet
}