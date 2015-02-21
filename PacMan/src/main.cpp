#include "Game.h"
#include "AttractScreen.h"

int main( int argc, char* args[] )
{
    Game game;

	Utils::CreateFolder(DEBUG_LOG_FOLDER);

	// Only run the game if it initialized properly
	if (!game.Initialize())
	{
		return 1;
	}

	// Load common resources
	game.LoadContent();

	game.ChangeScreen(AttractScreen::Instance());

	while (game.Running())
	{
		game.HandleEvents();
		game.Update();
		game.Render();
	}
	
    return 0;    
}