#include "AttractScreen.h"
#include "GameplayScreen.h"

AttractScreen AttractScreen::attractScreen;

void AttractScreen::Initialize(Game* game)
{
	SDL_Surface* fontSurface = NULL;

	// Open the fonts
	titleFont = TTF_OpenFont("Resources\\Fonts\\PAC-FONT.TTF", SCREEN_WIDTH);
	instructionFont = TTF_OpenFont("Resources\\Fonts\\ARIAL.TTF", SCREEN_WIDTH / 2);
	
	// Create the title font surface and texture
	title.SetTexture( Utils::CreateFontTexture(game->renderer, titleFont, 
		"Pac-Man", SDL_Color{ 0, 255, 255 }) );
	title.SetBounds(SCREEN_WIDTH, SCREEN_HEIGHT / 5);
	title.SetPosition((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 3));
	
	// Create the instruction font surface and texture
	instructions.SetTexture(Utils::CreateFontTexture(game->renderer, instructionFont,
		"Press ENTER to play!", SDL_Color{ 255, 255, 255 }));
	instructions.SetBounds(SCREEN_WIDTH - (SCREEN_WIDTH / 3), SCREEN_HEIGHT / 16);
	instructions.SetPosition((SCREEN_WIDTH / 2), (SCREEN_HEIGHT - (SCREEN_HEIGHT / 4)));
}

void AttractScreen::Cleanup(Game* game)
{
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
	title.Render(game->renderer);
	instructions.Render(game->renderer);

	// Update the screen
	SDL_RenderPresent(game->renderer);
}