#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "game.h"

int main(int argc, char* argv[]) {
	// Create the game object
	Game* game = new Game();

	// Initialize and run the game
	if (game->Init()) {
		game->Run();
	}

	// Clean up
	delete game;
	return 0;
}
