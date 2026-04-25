#include "Game.h" // include all necessary constants and things

void InitGame(Game *g) {
    // Clear everything to zero/default first
    memset(g, 0, sizeof(Game));

    // Set specific non-zero starting values for Game state
    g->gamePhase = PHASE_TITLE;
    g->activePlayer = PLAYER1;

    // Initialize Cursor
    g->cursor.xPos = 0;
    g->cursor.yPos = 0;
    g->cursor.cursorMode = VERTICAL;

    // Initialize Player-specific data
    for (int i = 0; i < 2; i++) {
        g->players[i].singleSegmentShipsLeftToPlace = 3;
        g->players[i].doubleSegmentShipsLeftToPlace = 2;

        // Note: Arrays like horizPlacementArray are already
        // set to NONE (0) by the memset above.
    }

    // Initialize song to main theme
    g->music.notes = main_theme; // music arrays from SoundArrays.h
    MusicInit(&(g->music), 40, 1);

    Button_Init(&(g->modeButton), GPIOC, GPIO_PIN_10); // initialize buttons
    Button_Init(&(g->confirmButton), GPIOC, GPIO_PIN_11);
}

int PlacePhase(Game *g); // ship placing phase
int GuessPhase(Game *g); // ship guessing phase

int PlaceShip(Game *g); // function to attempt to place ship, returns 0 for success, 1 for fail
int GuessTile(Game *g); // function for guessing at cursor location, returns 0 for success, 1 for fail
