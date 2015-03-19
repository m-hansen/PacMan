#include "AttractScreen.h"
#include "GameplayScreen.h"

AttractScreen AttractScreen::attractScreen;

void AttractScreen::Initialize(Game* game)
{
	SDL_Surface* fontSurface = NULL;

	// Open the fonts
	titleFont = TTF_OpenFont("Resources/Fonts/PAC-FONT.TTF", Config::screenWidth);
	instructionFont = TTF_OpenFont("Resources/Fonts/ARIAL.TTF", Config::screenWidth);
	
	// Create the title font surface and texture
	SDL_Texture* titleTexture = Utils::CreateFontTexture(game->renderer, titleFont, 
		"Pac-Man", SDL_Color{ 0, 255, 255 });
	title = new Sprite(titleTexture, (Config::screenWidth / 2), (Config::screenHeight / 3), 
		Config::screenWidth, (Config::screenHeight / 5));

	// Create the instruction font surface and texture
	SDL_Texture* instrTexture = Utils::CreateFontTexture(game->renderer, instructionFont, 
		"Press ENTER to play!", SDL_Color{ 255, 255, 255 });
	instructions = new Sprite(instrTexture, (Config::screenWidth / 2), (Config::screenHeight - (Config::screenHeight / 4)),
		(Config::screenWidth - (Config::screenWidth / 3)), (Config::screenHeight / 16));
}

void AttractScreen::Cleanup(Game* game)
{
	delete (instructions);
	instructions = NULL;

	delete (title);
	title = NULL;

	TTF_CloseFont(instructionFont);
	instructionFont = NULL;

	TTF_CloseFont(titleFont);
	titleFont = NULL;
}

AttractScreen::~AttractScreen()
{

}

void AttractScreen::HandleEvents(Game* game)
{
	// Handle events on queue
	while (SDL_PollEvent(&currentEvent) != 0)
	{
		// User requests quit
		if (currentEvent.type == SDL_QUIT)
		{
			game->Quit();
		}
		// User presses a key
		else if (currentEvent.type == SDL_KEYDOWN)
		{
			switch (currentEvent.key.keysym.sym)
			{
			case SDLK_SPACE:
			case SDLK_RETURN:
				game->ChangeScreen(GameplayScreen::Instance());
				printf("Starting game!\n");
				break;
			case SDLK_ESCAPE:
				// User requests quit
				game->Quit();
				break;
			}
		}
		// User clicks the mouse
		else if (currentEvent.type == SDL_MOUSEBUTTONDOWN)
		{
			switch (currentEvent.button.clicks)
			{
			case SDL_BUTTON_LEFT:
				printf("Left mouse button clicked at\tx=%d\ty=%d\n", currentEvent.button.x, currentEvent.button.y);
				break;
			}
		}

		previousEvent = currentEvent;
	}
}

void AttractScreen::Update(Game* game)
{

}

void AttractScreen::Render(Game* game)
{
	// Clear color
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255); // black
	SDL_RenderClear(game->renderer);

	// Display the sprites
	title->Render(game->renderer);
	instructions->Render(game->renderer);

	// Update the screen
	SDL_RenderPresent(game->renderer);
}