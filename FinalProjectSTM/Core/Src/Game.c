#include "Game.h" // include all necessary constants and things

void InitGame(Game *g) {
    // CLEAR ALL MEMORY -- used for restarting the game
    memset(g, 0, sizeof(Game));
    //initialize players
    PlayerInit(&(g->players[0]));
    PlayerInit(&(g->players[1]));
    // Set specific non-zero starting values for Game state
    g->gamePhase = PHASE_TITLE; // game phase starts at title
    g->activePlayer = PLAYER1; // player 1 goes first
    // Initialize Cursor
    CursorInit(&(g->cursor));
    //Initialize display
    DisplayInit(&(g->display));
    //initialize message engine to title screen
    MessageInit(&(g->scrollMessage), "        BATTLESHIP             PRESS TO PLAY              ", 150, 1);
    g->displayMode = MESSAGE; // put display in message mode, not board mode

    // Initialize song to title theme track
    MusicInit(&(g->music), track_title, 100, 1);
    PlayTrack(&(g->music), track_title);

    //init buttons
    ButtonInit(&(g->modeButton), GPIOC, GPIO_PIN_10); // mode button
    ButtonInit(&(g->confirmButton), GPIOC, GPIO_PIN_11); // confirm button 
    ButtonInit(&(g->musicSwitch), GPIOC, GPIO_PIN_15); // please turn that music off now, it's kind of grating at this point, button (actually a switch, but we treat it like a button for convenience)

    //init pots
    InitPot(&(g->xPot), &(g->cursor), 1, X_AXIS, ADC1);// on channel 1
    InitPot(&(g->yPot), &(g->cursor), 2, Y_AXIS, ADC2);// on channel 2

}

void GameRun(Game *g) {
    //execute functionalities for the current game phase
    switch (g->gamePhase) {

    //title phase
    case PHASE_TITLE:
        //do not move on to the next phase until confirm button pressed
        if(Confirm(g)){
            //change to placing phase
            g->gamePhase = PHASE_PLACE;
            //set message to placing phase banner
            Cutscene(g, "        PLAYER 1 PLACE SHIPS ");
            PlayTrack(&g->music, track_castlevania); // play a new track for the placing phase
        }
        break;

    //placing phase
    case PHASE_PLACE:
        //don't progress until button pressed to clear message
        if(g->displayMode == MESSAGE) {
            //if confirm pressed, continue the program flow
            if(Confirm(g)){
                g->displayMode = BOARD; // start displaying theboard
            }
        } else { //now that we're done displaying the message

            //if confirm pressed, place ship
            if(Confirm(g)) {
                PlaceShip(g); // place ship
                PlayEffect(&g->music, effect_place); // play place sound effect
            }

            //if mode placed, toggle vert/horiz
            if(Mode(g)) {
                CursorHorizVertToggle(&(g->cursor)); // toggle
            }

            // Get a pointer to the current player, for convenience
            Player *p = &(g->players[g->activePlayer]);

            // If current player is done placing
            if (p->shipsToPlace == 0) {
                if (g->activePlayer == PLAYER1) {
                    g->cursor.cursorLength = DOUBLE_SEG; // set up for player 2 to place double length ships
                    TogglePlayer(g); // switch players
                    Cutscene(g, "        PLAYER 2 PLACE SHIPS "); // indicate that we are on new player
                } else {
                    // Player 2 is done, move to guessing phase
                    g->gamePhase = PHASE_TURN;
                    TogglePlayer(g); // Set back to P1 for the first turn
                    Cutscene(g, "        PLAYER 1 GUESS TILE "); // put up message to indicate guessing phase
                    PlayTrack(&g->music, track_megalovania); // play new track for the guesing phase
                    PlayEffect(&g->music, effect_switch); // play effect to signal that we're switching phases again
                }
            } else if (p->shipsToPlace == 3) { // if the player placing right now has only three ships left, they should be single segment
                g->cursor.cursorLength = SINGLE_SEG; // make them single segment
            }
        }

        break;

    //guessing phase
    case PHASE_TURN:
        if(g->displayMode == MESSAGE) {
            //if confirm pressed, continue the program flow
            if(Confirm(g)){
                g->displayMode = BOARD; // start displaying theboard
            }
        } else {
            //if mode placed, toggle vert/horiz
            if(Mode(g)) {
                CursorHorizVertToggle(&(g->cursor));
            }
            //if confirm is pressed
            if(Confirm(g)) {
                int guess = GuessTile(g); // store whether guess was invalid, hit, or miss

                if(guess == 0) { // miss
                    TogglePlayer(g); // switch players
                    Cutscene(g, (g->activePlayer == PLAYER1) ? "        PLAYER 1 GUESS TILE " : "        PLAYER 2 GUESS TILE "); // display message to indicate player switch
                    PlayEffect(&g->music, effect_miss); // play miss effect
                } else if (guess == 1) { // if guess is a hit, increase a player's hitcounter
                    Cutscene(g, (g->activePlayer == PLAYER1) ? "        HIT PLAYER 2 SHIP         GUESS AGAIN " : "        HIT PLAYER 1 SHIP         GUESS AGAIN "); // show guess again message
                    PlayEffect(&(g->music), effect_hit); // play hit effect 
                    g->players[g->activePlayer].hitCount++; // increment hit counter
                    //if all ships are sunk, active player wins
                    if(g->players[g->activePlayer].hitCount >= 7){
                        g->gamePhase = PHASE_END; // end phase
                        Cutscene(g, (g->activePlayer == PLAYER1) ? "        PLAYER 1 WINS        PRESS TO PLAY AGAIN " : "        PLAYER 2 WINS        PRESS TO PLAY AGAIN "); // prompt to play again
                        PlayTrack(&g->music, track_end); // put on the game end track
                        PlayEffect(&g->music, effect_win); // play game won effect
                    } 
                }
            }
        }
        break;
    //end phase
    case PHASE_END:
        //wait for press to restart game
        if(Confirm(g)){
            //change to title phase 
            g->gamePhase = PHASE_TITLE;
            InitGame(g); // restart the game
            //set message to title phase banner
            Cutscene(g, "        BATTLESHIP             PRESS TO PLAY              ");
        }
    }
}

// function to attempt to place ship, returns 0 for success, 1 for fail
int PlaceShip(Game *g) {
    //checking for horizontal or vertical validity
    switch(g->cursor.cursorMode) {
        case VERTICAL:
            //if nothing is placed in the placement array of the player placing right now, place there
            if(g->players[g->activePlayer].vertPlacementArray[g->cursor.xPos][g->cursor.yPos] == NONE){
                g->players[g->activePlayer].vertPlacementArray[g->cursor.xPos][g->cursor.yPos] = PLACED; // place at cursor
                if(g->cursor.cursorLength == DOUBLE_SEG) { g->players[g->activePlayer].vertPlacementArray[g->cursor.xPos][g->cursor.yPos+1] = PLACED;}  // place at second cursor segment if present
                g->players[g->activePlayer].shipsToPlace--; // decrement ships left to place
                return 0; //success
            } else {
                return -1; // placement failed
            }
            break;

        case HORIZONTAL:
            //if nothing is placed in the placement array of the player placing right now, place there
            if(g->players[g->activePlayer].horizPlacementArray[g->cursor.xPos][g->cursor.yPos] == NONE){
                g->players[g->activePlayer].horizPlacementArray[g->cursor.xPos][g->cursor.yPos] = PLACED;  //place at cursor
                if(g->cursor.cursorLength == DOUBLE_SEG) { g->players[g->activePlayer].horizPlacementArray[g->cursor.xPos+1][g->cursor.yPos] = PLACED;}  // place at second cursor segment if present
                g->players[g->activePlayer].shipsToPlace--; // player has one less ship to place
                return 0; //success
            } else {
                return -1; // placement failed
            }
            break;        
    }

    return -1; // failed for some reason
}

//function to attempt to guess a tile, returns 1 if hit, 0 if miss, and -1 if guess was invalid
int GuessTile(Game *g) {
    //guess a tile
    switch(g->cursor.cursorMode) {
        case VERTICAL:
            //if tile at cursor hasn't been guessed yet
            if(g->players[g->activePlayer].vertGuessArray[g->cursor.xPos][g->cursor.yPos] == NONE){
                //set the tile in the guess array as a hit if the corresponding tile is placed in the opponent's placement array, otherwise miss
                int isHit = (g->players[1-g->activePlayer].vertPlacementArray[g->cursor.xPos][g->cursor.yPos]==PLACED);
                g->players[g->activePlayer].vertGuessArray[g->cursor.xPos][g->cursor.yPos] = (isHit) ? HIT : MISS; // assign hit/miss value to return
               
                return isHit; //returns 1 for hit, 0 for miss
            } else {
                return -1; // guess invalid
            }
            break;

        case HORIZONTAL:
            //if tile at cursor hasn't been guessed yet, set it to match opponent's placement array
            if(g->players[g->activePlayer].horizGuessArray[g->cursor.xPos][g->cursor.yPos] == NONE){
               //set the tile in the guess array as a hit if the corresponding tile is placed in the opponent's placement array
                int isHit = (g->players[1-g->activePlayer].horizPlacementArray[g->cursor.xPos][g->cursor.yPos]==PLACED);
                g->players[g->activePlayer].horizGuessArray[g->cursor.xPos][g->cursor.yPos] = (isHit) ? HIT : MISS; // assign hit/miss value to return
                
                return isHit; //returns 1 for hit, 0 for miss
            } else {
                return -1; // guess invalid
            }
            break;        
    }

    return -1; // if failed for some reason
} // function for guessing at cursor location, returns 0 for success, -1 for fail


//helper function to swtich player
void TogglePlayer(Game *g) {
    //switch to other player
    if(g->activePlayer == PLAYER1) {
        g->activePlayer = PLAYER2;
    } else if (g->activePlayer == PLAYER2) {
        g->activePlayer = PLAYER1;
    }
}

//renders display and outputs, used by systick
void DisplayOutput(Game *g) {
    if(g->displayMode == MESSAGE) {//display message if in message mode 
        DisplayScrollMessage(&(g->scrollMessage));
    } else if (g->displayMode == BOARD) { // display board if in board mode
        DisplayBoard(g);
    }
}

//polls all inputs, used in the main while(1)
void GamePoll(Game *g) {
    //update buttons
    ButtonUpdate(&(g->modeButton));
    ButtonUpdate(&(g->confirmButton));
    ButtonUpdate(&g->musicSwitch);

    if(WasPressed(&g->musicSwitch)) {g->music.doPlay = (g->music.doPlay) ? 0 : 1;} //toggle music to on or off if switch has been toggled

    //update potentiometers
    UpdatePot(&(g->xPot));
    UpdatePot(&(g->yPot));
}

//helper functions

//confirm button press
int Confirm(Game *g) {
    return WasPressed(&(g->confirmButton));
}

//mode button press
int Mode(Game *g){
    return WasPressed(&(g->modeButton));
}

//trigger next text phase
void Cutscene(Game *g, char *c) {
    g->displayMode = MESSAGE; // put the display in message mode
    SetMessage(&(g->scrollMessage), c, 150, 1); // set to new message
}