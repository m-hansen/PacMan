#include "Game.h"
#include "GameplayScreen.h"

int main( int argc, char* args[] )
{
    Game game;

	// Only run the game if it initialized properly
	if (!game.Initialize())
	{
		return 1;
	}

	// Load common resources
	game.LoadContent();

	game.ChangeScreen(GameplayScreen::Instance());

	while (game.Running())
	{
		game.HandleEvents();
		game.Update();
		game.Render();
	}
	
    return 0;    
}