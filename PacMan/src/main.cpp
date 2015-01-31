#include "Game.h"

int main( int argc, char* args[] )
{
    Game game;

	// Only run the game if it initialized properly
	if (game.Initialize())
	{
		game.LoadContent();
		game.Run();
	}
	
    return 0;    
}