#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <unistd.h>

#define WINDOW_X (1366)
#define WINDOW_Y (768)

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
		                                  
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		                                
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
	
	
	int lineY[6] =  {
		WINDOW_Y / 3 * 1,
		WINDOW_Y / 3 * 2,
		WINDOW_Y / 3 * 3,
		WINDOW_Y / 3 * 4 % WINDOW_Y,
		WINDOW_Y / 3 * 5 % WINDOW_Y,
		WINDOW_Y / 6 * 6 % WINDOW_Y,
	};
	int lineYChange[6] = {1, 1, 1, -1, -1, -1};
	int lineColor[6][3] = {
		{255, 0, 0},
		{255, 255, 0},
		{0, 255, 0},
		{0, 255, 255},
		{0, 0, 255},
		{255, 0, 255},
	};
	
	//Turret preparation
    SDL_Surface *images[16];
    for(int i = 0; i < 16; i++)
    {
    	char spritelocation[] = "sprites/0001.png";
    	spritelocation[10] = 48 + (i+ 1)/10; //to ascii number
    	spritelocation[11] = 48 + ((i+ 1)%10); //to ascii number
    	printf("sprite%i: %s\n", i, spritelocation);
    	images[i]= IMG_Load(spritelocation);
    }

	
	while(1)
	{
		clock_t FrameStartClock = clock();
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // set lineColor to black
		SDL_RenderClear(renderer); //erase
		
		for(int i = 0; i < 6; i++)
		{
			lineY[i] += lineYChange[i];
			if(lineY[i] >= WINDOW_Y)
			{
				lineYChange[i] = -1;
			}
			if(lineY[i] <= 0)
			{
				lineYChange[i] = 1;
			}
			
			SDL_SetRenderDrawColor(renderer, lineColor[i][0], lineColor[i][1], lineColor[i][2], 255); // set lineColor to red
			SDL_RenderDrawLine(renderer, 0, lineY[i], WINDOW_X, WINDOW_Y-lineY[i]);     // draw a diagonal line
		}

		//Turret
		for(int x = 0; x < 2; x++)
		{
			for(int y = 0; y < 2; y++)
			{
				int animFrame = ((frame%64/4) + (4*(x+y*2))) % 16;
				printf("animFrame: %i\n", animFrame);
				SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, images[animFrame]);
				SDL_Rect dstrect = { (WINDOW_X-256)*x, (WINDOW_Y-256)*y, 256, 256 };
				SDL_RenderCopy(renderer, texture, NULL, &dstrect);
			}
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
