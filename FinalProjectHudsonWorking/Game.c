#include "Game.h" // include all necessary constants and things

void InitGame(Game *g) {
    // CLEAR ALL MEMORY
    memset(g, 0, sizeof(Game));
    //initialize players
    PlayerInit(&(g->players[0]));
    PlayerInit(&(g->players[1]));
    // Set specific non-zero starting values for Game state
    g->gamePhase = PHASE_TITLE;
    g->activePlayer = PLAYER1;
    // Initialize Cursor
    CursorInit(&(g->cursor));
    DisplayInit(&(g->display));

    MessageInit(&(g->scrollMessage), "BATTLESHIP ", 150, 1);
    g->displayMode = MESSAGE;

    // Initialize song to main theme
    MusicInit(&(g->music), main_theme, 40, 1);
    //init buttons
    Button_Init(&(g->modeButton), GPIOC, GPIO_PIN_10);
    Button_Init(&(g->confirmButton), GPIOC, GPIO_PIN_11);
}

void DisplayOutput(Game *g) {
    if(g->displayMode == MESSAGE) {
        DisplayScrollMessage(&(g->scrollMessage));
    } else if (g->displayMode == BOARD) {
        DisplayBoard(g);
    }
}


int PlacePhase(Game *g) {
    
}; // ship placing phase
int GuessPhase(Game *g); // ship guessing phase

int PlaceShip(Game *g); // function to attempt to place ship, returns 0 for success, 1 for fail
int GuessTile(Game *g); // function for guessing at cursor location, returns 0 for success, 1 for fail

