#ifndef PLAYER_H_
#define PLAYER_H_

#include "GameConstAndEnums.h" // include constants and SquareState enum

// player data structure
typedef struct Player {
    // maps
    // array for placement of horizontal ships, initialized to 0 for no ship placement
    SquareState horizPlacementArray[H_WIDTH][H_HEIGHT]; 
    // array for placement of vertical ships
    SquareState vertPlacementArray[V_WIDTH][V_HEIGHT]; 

    // array for horizontal guesses, initialize to 0
    SquareState horizGuessArray[H_WIDTH][H_HEIGHT]; 
    SquareState vertGuessArray[V_WIDTH][V_HEIGHT];

    int singleSegmentShipsLeftToPlace; // tracks how many ships are left to place
    int doubleSegmentShipsLeftToPlace;

} Player;

#endif /* PLAYER_H_ */