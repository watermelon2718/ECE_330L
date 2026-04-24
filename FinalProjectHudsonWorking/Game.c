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
    
    // Set play state and starting position
    g->music.doPlay = 1;       // Start playing immediately
    g->music.index = 0;        // Start at the first note
    g->music.counter = 0;      // Reset duration counter
    g->music.tone = 0;         // Reset frequency counter
    
    // Initialize Pitch and Vibrato
    // We load the first note's pitch into our working variables
    g->music.saveNote = g->music.notes[0].pitch;
    g->music.currentNote = g->music.saveNote;
    
    // Configure Vibrato Settings
    g->music.vibratoCount = 0;
    g->music.vibratoRate = 40;  // Adjust this for speed (lower = faster)
    g->music.vibratoDepth = 1;  // Adjust this for intensity (how many frequency units it shifts)
    g->music.vibratoDir = 1;    // Start the oscillation moving 'up'

}

int PlacePhase(Game *g); // ship placing phase
int GuessPhase(Game *g); // ship guessing phase

int PlaceShip(Game *g); // function to attempt to place ship, returns 0 for success, 1 for fail
int GuessTile(Game *g); // function for guessing at cursor location, returns 0 for success, 1 for fail
