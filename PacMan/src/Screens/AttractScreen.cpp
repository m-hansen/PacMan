#include "AttractScreen.h"
#include "GameplayScreen.h"

AttractScreen AttractScreen::attractScreen;

void AttractScreen::Initialize(Game* game)
{
	SDL_Surface* fontSurface = NULL;

	// Open the fonts
	titleFont = TTF_OpenFont("Resources\\Fonts\\PAC-FONT.TTF", SCREEN_WIDTH);
	instructionFont = TTF_OpenFont("Resources\\Fonts\\ARIAL.TTF", SCREEN_WIDTH / 2);

	// Adjust the title font bounding rectangle
	titleTextRect.w = SCREEN_WIDTH;
	titleTextRect.h = SCREEN_HEIGHT / 5;
	titleTextRect.x = (SCREEN_WIDTH / 2) - (titleTextRect.w / 2);
	titleTextRect.y = (SCREEN_HEIGHT / 3) - (titleTextRect.h / 2);

	// Create the title font surface and texture
	fontSurface = TTF_RenderText_Solid(titleFont, "Pac-Man", SDL_Color{ 0, 255, 255 });
	titleTexture = SDL_CreateTextureFromSurface(game->renderer, fontSurface);
	SDL_FreeSurface(fontSurface);
	fontSurface = NULL;

	// Adjust the instruction font bounding rectangle
	instrTextRect.w = G_SIZE * 18;
	instrTextRect.h = G_SIZE * 2;
	instrTextRect.x = (SCREEN_WIDTH / 2) - (instrTextRect.w / 2);
	instrTextRect.y = (SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) - (instrTextRect.h / 2));

	// Create the instruction font surface and texture
	fontSurface = TTF_RenderText_Solid(instructionFont, "Press ENTER to play!", SDL_Color{ 255, 255, 255 });
	instructionTexture = SDL_CreateTextureFromSurface(game->renderer, fontSurface);
	SDL_FreeSurface(fontSurface);
	fontSurface = NULL;
}

void AttractScreen::Cleanup(Game* game)
{

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

	// Display the title text
	SDL_RenderCopy(game->renderer, titleTexture, NULL, &titleTextRect);

	// Display the instruction text
	SDL_RenderCopy(game->renderer, instructionTexture, NULL, &instrTextRect);

	// Update the screen
	SDL_RenderPresent(game->renderer);
}