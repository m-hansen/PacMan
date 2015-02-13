#include "AttractScreen.h"

const int SCREEN_WIDTH = 28 * G_SIZE; // 224 width with 8x8 tiles
const int SCREEN_HEIGHT = 36 * G_SIZE; // 288 height with 8x8 tiles

AttractScreen::AttractScreen(SDL_Renderer* renderer) : IScreen(renderer)
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
	titleTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
	SDL_FreeSurface(fontSurface);
	fontSurface = NULL;

	// Adjust the instruction font bounding rectangle
	instrTextRect.w = G_SIZE * 18;
	instrTextRect.h = G_SIZE * 2;
	instrTextRect.x = (SCREEN_WIDTH / 2) - (instrTextRect.w / 2);
	instrTextRect.y = (SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) - (instrTextRect.h / 2));

	// Create the instruction font surface and texture
	fontSurface = TTF_RenderText_Solid(instructionFont, "Press ENTER to play!", SDL_Color{ 255, 255, 255 });
	instructionTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
	SDL_FreeSurface(fontSurface);
	fontSurface = NULL;
}

AttractScreen::~AttractScreen()
{

}

void AttractScreen::HandleEvents()
{
	// Handle events on queue
	while (SDL_PollEvent(&currentEvent) != 0)
	{
		// User requests quit
		if (currentEvent.type == SDL_QUIT)
		{
			isRunning = false;
		}
		// User presses a key
		else if (currentEvent.type == SDL_KEYDOWN)
		{
			switch (currentEvent.key.keysym.sym)
			{
			case SDLK_SPACE:
			case SDLK_RETURN:
				printf("Starting game!\n");
				break;
			case SDLK_ESCAPE:
				// User requests quit
				isRunning = false;
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

void AttractScreen::Update()
{

}

void AttractScreen::Render()
{
	// Clear color
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black
	SDL_RenderClear(renderer);

	// Display the title text
	SDL_RenderCopy(renderer, titleTexture, NULL, &titleTextRect);

	// Display the title text
	SDL_RenderCopy(renderer, instructionTexture, NULL, &instrTextRect);

	// Update the screen
	SDL_RenderPresent(renderer);
}