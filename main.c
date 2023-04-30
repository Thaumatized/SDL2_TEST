#include <SDL2/SDL.h>
#include <time.h>
#include <unistd.h>

int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Failed to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("My SDL2 Window",
		                                  SDL_WINDOWPOS_UNDEFINED,
		                                  SDL_WINDOWPOS_UNDEFINED,
		                                  640, 480,
		                                  SDL_WINDOW_SHOWN);
	if (!window) {
		printf("Failed to create SDL window: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		printf("Failed to create SDL renderer: %s\n", SDL_GetError());
		return 1;
	}

	int FrameRate = 60;
	int ClocksPerFrame = CLOCKS_PER_SEC / FrameRate;
	int frame = 0;
	
	
	int y[6] =  {
		960 / 6 * 1,
		960 / 6 * 2,
		960 / 6 * 3,
		960 / 6 * 4 % 480,
		960 / 6 * 5 % 480,
		960 / 6 * 6 % 480,
	};
	int yChange[6] = {1, 1, 1, -1, -1, -1};
	int color[6][3] = {
		{255, 0, 0},
		{255, 255, 0},
		{0, 255, 0},
		{0, 255, 255},
		{0, 0, 255},
		{255, 0, 255},
	};
	
	while(1)
	{
		clock_t FrameStartClock = clock();
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // set color to black
		SDL_RenderClear(renderer); //erase
		
		for(int i = 0; i < 6; i++)
		{
			y[i] += yChange[i];
			if(y[i] >= 480)
			{
				yChange[i] = -1;
			}
			if(y[i] <= 0)
			{
				yChange[i] = 1;
			}
			
			SDL_SetRenderDrawColor(renderer, color[i][0], color[i][1], color[i][2], 255); // set color to red
			SDL_RenderDrawLine(renderer, 0, y[i], 640, 480-y[i]);     // draw a diagonal line
		}

		SDL_RenderPresent(renderer);

		SDL_Event event;
		int quit = 0;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
				    quit = 1;
				    break;
				// handle other events...
			}
			
			if(quit)
			{
				break;
			}
		}
		
		if(quit)
		{
			break;
		}
		
		printf("frame: %i\n", frame);
		
		//Sleep until we have taken up enough time.
		int ClocksThisFrame = clock()-FrameStartClock;
		int ClocksToSleep = ClocksPerFrame - ClocksThisFrame;
		usleep(((double)ClocksToSleep) / ClocksPerFrame / FrameRate * 1000000);
		
		frame++;
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
