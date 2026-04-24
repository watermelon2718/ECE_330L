//player data structure
typedef struct Player {
	//maps
	SquareState horizPlacementArray[H_WIDTH][H_HEIGHT]; // array for placement of horizontal ships, initialized to 0 for no ship placement
	SquareState vertPlacementArray[V_WIDTH][V_HEIGHT]; // array for placement of horizontal ships, initialized to 0 for no ship placement

	SquareState horizGuessArray[H_WIDTH][H_HEIGHT]; // array for horizontal guesses, initialize to 0
	SquareState vertGuessArray[V_WIDTH][V_HEIGHT];

	int singleSegmentShipsLeftToPlace; // tracks how many ships are left to place
	int doubleSegmentShipsLeftToPlace;

} Player;