#ifndef PLAYER_H_
#define PLAYER_H_

#include "GameConstAndEnums.h" // include constants and TileState enum

// player data structure
typedef struct Player {
    // maps
    // array for placement of horizontal ships, initialized to 0 for no ship placement
    TileState horizPlacementArray[H_WIDTH][H_HEIGHT]; 
    // array for placement of vertical ships
    TileState vertPlacementArray[V_WIDTH][V_HEIGHT]; 

    // array for horizontal guesses, initialize to 0
    TileState horizGuessArray[H_WIDTH][H_HEIGHT]; 
    TileState vertGuessArray[V_WIDTH][V_HEIGHT];

    int singleSegmentShipsLeftToPlace; // tracks how many ships are left to place
    int doubleSegmentShipsLeftToPlace;

} Player;

void PlayerInit(Player *p);

#endif /* PLAYER_H_ */