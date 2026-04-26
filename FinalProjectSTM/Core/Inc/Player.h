#ifndef PLAYER_H_
#define PLAYER_H_

#include "GameConstAndEnums.h" // include constants and TileState enum
#include <string.h>

// player data structure
typedef struct Player {
    // maps

    // array for placement of horizontal ships, initialized to 0 for no ship placement
    TileState horizPlacementArray[H_WIDTH][H_HEIGHT]; 
    // array for placement of vertical ships
    TileState vertPlacementArray[V_WIDTH][V_HEIGHT]; 

    // array for horizontal guesses, initialize to 0
    TileState horizGuessArray[H_WIDTH][H_HEIGHT]; 
    // array for vertical guesses, initialize to 0
    TileState vertGuessArray[V_WIDTH][V_HEIGHT];

    int shipsToPlace; // keeps track of how many ships are left to be placed

    int hitCount; // keeps track of how many hits the player has made on the other player, used for win condition

} Player;

void PlayerInit(Player *p); // initialize player struct

#endif /* PLAYER_H_ */