#include <SDL2/SDL.h>
#include <time.h>
#include<unistd.h>

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

	clock_t starttime = clock();
	int framerate = 60;
	int frame = 0;
	
	
	int y = 0;
	while(1)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // set color to black
		SDL_RenderClear(renderer); //erase
		y = (y + 1) % 480;
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // set color to red
		SDL_RenderDrawLine(renderer, 0, y, 640, 480-y);     // draw a diagonal line

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
		
		long clocks = clock()-starttime;
		double accuratetime = (double)(clocks)/CLOCKS_PER_SEC;
		long asframes = accuratetime/((double)1/framerate);
		//sleep(0.01);
		printf("frame: %i, clocks: %li, time: %lf, frames: %li\n", frame, clocks, accuratetime, asframes);
		
		
		while(frame >= asframes)
		{
			clocks = clock()-starttime;
			accuratetime = (double)(clocks)/CLOCKS_PER_SEC;
			asframes = accuratetime/((double)1/framerate);
			//sleep(0.01);
		}
		frame++;
		printf("frame: %i\n", frame);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
