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
    ButtonInit(&(g->modeButton), GPIOC, GPIO_PIN_10);
    ButtonInit(&(g->confirmButton), GPIOC, GPIO_PIN_11);
}

void GameRun(Game *g) {
    switch (g->gamePhase) {

    case PHASE_TITLE:
        //do not move on to the next phase until confirm button pressed
        if(WasPressed(&(g->confirmButton))){
            g->gamePhase = PHASE_PLACE;
        }
        break;
    case PHASE_PLACE:

        break;

    }
}

//this function will run during the placing phase
int PlacePhase(Game *g) {
    for(player = 0; player < 1; player++)     {
        while(g->players[1].doubleSegmentShipsLeftToPlace > 0) {
            PlaceShip(g);
        }
        
    }

}; // ship placing phase
int GuessPhase(Game *g); // ship guessing phase

int PlaceShip(Game *g); // function to attempt to place ship, returns 0 for success, 1 for fail
int GuessTile(Game *g); // function for guessing at cursor location, returns 0 for success, 1 for fail


//switches player elegantly
void TogglePlayer(Game *g) {
    if(g->activePlayer == PLAYER1) {
        g->activePlayer = PLAYER2;
    } else if (g->activePlayer == PLAYER2) {
        g->activePlayer = PLAYER1;
    }
}

//renders display and outputs, used by systick
void DisplayOutput(Game *g) {
    if(g->displayMode == MESSAGE) {
        DisplayScrollMessage(&(g->scrollMessage));
    } else if (g->displayMode == BOARD) {
        DisplayBoard(g);
    }
}

//polls all inputs, used in the main while(1)
void GamePoll(Game *g) {
    //update buttons
    ButtonUpdate(&(g->modeButton));
    ButtonUpdate(&(g->confirmButton))
    //update potentiometers
    PotUpdate(&(g->xPot));
    PotUpdate(&(g->yPot));
}