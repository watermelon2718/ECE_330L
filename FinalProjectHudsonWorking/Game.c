#include "Game.h" // include all necessary constants and things

void InitGame(Game *g) {
    // 1. Clear everything to zero/default first
    memset(g, 0, sizeof(Game));

    // 2. Set specific non-zero starting values for Game state
    g->gamePhase = PHASE_TITLE;
    g->activePlayer = PLAYER1;

    // 3. Initialize Cursor
    g->cursor.xPos = 0;
    g->cursor.yPos = 0;
    g->cursor.cursorMode = VERTICAL;

    // 4. Initialize Player-specific data
    for (int i = 0; i < 2; i++) {
        g->players[i].singleSegmentShipsLeftToPlace = 3;
        g->players[i].doubleSegmentShipsLeftToPlace = 2;

        // Note: Arrays like horizPlacementArray are already
        // set to NONE (0) by the memset above.
    }
}

int PlaceShip(Game *g); // function to attempt to place ship, returns 0 for success, 1 for fail

int GuessTile(Game *g) // function for guessing at cursor location, returns 0 for success, 1 for fail