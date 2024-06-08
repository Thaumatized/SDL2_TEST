#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#include "configReader.c"

#define SPRITE_ORIENTATIONS (72)
#define MAX_FILE_PATH (1024)

int WINDOW_X = 640;
int WINDOW_Y = 360;

void getConfig(char* pathToExecutable) {
	char path[MAX_FILE_PATH];
	memset(path, 0, MAX_FILE_PATH);
	strcat(path, pathToExecutable);
	strcat(path, "config.ini");

	config_fetch_options config_options[] = {
		{"WINDOW_X", &WINDOW_X, CONFIG_INT},
		{"WINDOW_Y", &WINDOW_Y, CONFIG_INT},
	};

	getConfigurations(path, config_options, 2);
}

float degsin(float deg) {return sin(deg*0.0174532925);}
float degcos(float deg) {return cos(deg*0.0174532925);}
float degtan(float deg) {return tan(deg*0.0174532925);}
float degasin(float val) {return 57.2957795*asin(val);}
float degacos(float val) {return 57.2957795*acos(val);}
float degatan(float val) {return 57.2957795*atan(val);}

void getPathToExecutable(char* buf, int bufLen)
{
	readlink("/proc/self/exe", buf, bufLen); //Linux.
	//GetModuleFileName(NULL, buf, bufLen) //Windows?

	for(int i = bufLen - 1; i >= 0; i--)
	{
		if(buf[i] == '/')
		{
			break;
		}
		buf[i] = 0;
	}
}

struct Vector2
{
	float x;
	float y;
} typedef Vector2;

struct Object
{
	Vector2 pos;
	Vector2 vel;
	float rot;
	int spriteSize;
	SDL_Surface* spriteSheet;
} typedef Object;

int rotToFrame(float rot) { return (int)(rot  / (360.0 / (float)SPRITE_ORIENTATIONS) + 0.5) % SPRITE_ORIENTATIONS; }

Vector2 rotToVector2(float rot)
{
	int ySing = 1 - (rot > 90 || rot < 270)*2;

	Vector2 result = {
		degsin(rot),
		degcos(rot)*ySing
	};
	return result;
}

Vector2 multiplyVector2(Vector2 vector, float multiplier)
{
	Vector2 result = {
		vector.x * multiplier,
		vector.y * multiplier
	};

	return result;
}

int main()
{
	char pathToExecutable[MAX_FILE_PATH];
	memset(pathToExecutable, 0, MAX_FILE_PATH);
	getPathToExecutable(pathToExecutable, MAX_FILE_PATH-1);
	char path[MAX_FILE_PATH];
	memset(path, 0, MAX_FILE_PATH);

	getConfig(pathToExecutable);

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Failed to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("My SDL2 Window",
		                                  SDL_WINDOWPOS_UNDEFINED,
		                                  SDL_WINDOWPOS_UNDEFINED,
		                                  WINDOW_X, WINDOW_Y,
		                                  SDL_WINDOW_SHOWN);
		                                  
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		                                
	if (!window) {
		printf("Failed to create SDL window: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		printf("Failed to create SDL renderer: %s\n", SDL_GetError());
		return 1;
	}

	int FrameRate = 60;
	int ClocksPerFrame = CLOCKS_PER_SEC / FrameRate;
	int frame = 0;
	
	//Test monkey heads preparation
    SDL_Surface *images[SPRITE_ORIENTATIONS];
    for(int i = 0; i < SPRITE_ORIENTATIONS; i++)
    {
    	char spritelocation[] = "sprites/monkey/0001.png";
    	spritelocation[17] = 48 + (i+ 1)/10; //to ascii number
    	spritelocation[18] = 48 + ((i+ 1)%10); //to ascii number
    	images[i]= IMG_Load(spritelocation);
    }

	memset(path, 0, MAX_FILE_PATH);
	strcat(path, pathToExecutable);
	strcat(path, "sprites/shadow.png");
	SDL_Surface *shadowImage = IMG_Load(path);
	SDL_Texture *shadowTexture = SDL_CreateTextureFromSurface(renderer, shadowImage);

	//TEST OBJECT
	Object testObject;
	testObject.pos.x = WINDOW_X/2;
	testObject.pos.y = WINDOW_Y/2;
	testObject.vel.x = 0;
	testObject.vel.y = 0;
	testObject.rot = 0;
	testObject.spriteSize = 128;
	memset(path, 0, MAX_FILE_PATH);
	strcat(path, pathToExecutable);
	strcat(path, "sprites/monkeysheet.png");
	testObject.spriteSheet = IMG_Load(path);


	Object monkeyHeads[4];
	for(int i = 0; i < 4; i++)
	{
		monkeyHeads[i].pos.x = (WINDOW_X-256) * (i % 2);
		monkeyHeads[i].pos.y = (WINDOW_Y-256) * (i / 2);;
		monkeyHeads[i].vel.x = 0;
		monkeyHeads[i].vel.y = 0;
		monkeyHeads[i].rot = 0;
		monkeyHeads[i].spriteSize = 128;
		monkeyHeads[i].spriteSheet = testObject.spriteSheet;
	}

	//test object control variables
	int A = 0;
	int D = 0;
	int W = 0;
	int S = 0;

	
	while(1)
	{
		clock_t FrameStartClock = clock();
		SDL_SetRenderDrawColor(renderer, 153, 138, 78, 255);
		SDL_RenderClear(renderer); //erase

		int animFrame = 0;
		int rotFrame = 0;

		//Rotating monkey heads
		for(int i = 0; i < 4; i++)
		{
			//int animFrame = ((frame%(SPRITE_ORIENTATIONS*4)/4) + (SPRITE_ORIENTATIONS/4*(x+y*2))) % SPRITE_ORIENTATIONS;
			//SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, images[animFrame]);
			//SDL_Rect dstrect = { (WINDOW_X-256)*x, (WINDOW_Y-256)*y, 256, 256 };
			

			monkeyHeads[i].rot = (monkeyHeads[i].rot + 1);
			if(monkeyHeads[i].rot < 0) monkeyHeads[i].rot += 360;
			if(monkeyHeads[i].rot > 360) monkeyHeads[i].rot -= 360;
			rotFrame = rotToFrame(monkeyHeads[i].rot);
			animFrame = (frame+(30*i))%119-59;
			if(animFrame < 0)
			{
				animFrame *= -1;
			}
			SDL_Surface *img = SDL_CreateRGBSurface(0, monkeyHeads[i].spriteSize, monkeyHeads[i].spriteSize, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
			SDL_Rect srcrect = { rotFrame*monkeyHeads[i].spriteSize, animFrame*monkeyHeads[i].spriteSize, 128, 128 };
			SDL_Rect dstrect = { 0, 0, 0, 0 };
			SDL_BlitSurface(monkeyHeads[i].spriteSheet, &srcrect, img, &dstrect);
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, img);
			SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
			dstrect.x = monkeyHeads[i].pos.x;
			dstrect.y = monkeyHeads[i].pos.y;
			dstrect.w = 256;
			dstrect.h = 256;
			SDL_RenderCopy(renderer, texture, NULL, &dstrect);

			SDL_FreeSurface(img);
			SDL_DestroyTexture(texture);
		}

		//TEST OBJECT
		testObject.rot += (-A + D) * 6;
		if(testObject.rot < 0) testObject.rot += 360;
		if(testObject.rot > 360) testObject.rot -= 360;
		rotFrame = rotToFrame(testObject.rot);
		/*
		//This would cause the monkey head to float up and down.
		//I find this undesirable for the playercontrolled one.
		//This will be replaced with advanced logic once the controlled object is an actual tank.
		//Kept this for reference.
		animFrame = frame%119-59;
		if(animFrame < 0)
		{
			animFrame *= -1;
		}
		*/
		animFrame = 59;

		testObject.vel = multiplyVector2(rotToVector2(testObject.rot), (-S+W)*15);
		
		testObject.pos.x += testObject.vel.x;
		testObject.pos.y += testObject.vel.y;
		SDL_Surface *img = SDL_CreateRGBSurface(0, testObject.spriteSize, testObject.spriteSize, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
		SDL_Rect srcrect = { rotFrame*testObject.spriteSize, animFrame*testObject.spriteSize, 128, 128 };
		SDL_Rect dstrect = { 0, 0, 0, 0 };
		SDL_BlitSurface(testObject.spriteSheet, &srcrect, img, &dstrect);
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, img);
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
		//SHADOW
		dstrect.x = testObject.pos.x + 64;
		dstrect.y = testObject.pos.y + 128 + 64;
		dstrect.w = 128;
		dstrect.h = 64;
		SDL_RenderCopy(renderer, shadowTexture, NULL, &dstrect);
		//Object it self
		dstrect.x = testObject.pos.x;
		dstrect.y = testObject.pos.y;
		dstrect.w = 256;
		dstrect.h = 256;
		SDL_RenderCopy(renderer, texture, NULL, &dstrect);
		if(testObject.pos.x > WINDOW_X)
		{
			testObject.pos.x -= WINDOW_X + 256;
		}
		if(testObject.pos.x < 0 - 256)
		{
			testObject.pos.x += WINDOW_X + 256;
		}
		if(testObject.pos.y > WINDOW_Y)
		{
			testObject.pos.y -= WINDOW_Y + 256;
		}
		if(testObject.pos.y < 0 - 256)
		{
			testObject.pos.y += WINDOW_Y + 256;
		}

		SDL_FreeSurface(img);
		SDL_DestroyTexture(texture);
		
		SDL_RenderPresent(renderer);

		SDL_Event event;
		int quit = 0;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
				    quit = 1;
				    break;
				case SDL_KEYDOWN:
					if(!strcmp(SDL_GetKeyName(event.key.keysym.sym), "A"))
					{
						A = 1;
					}
					else if(!strcmp(SDL_GetKeyName(event.key.keysym.sym), "D"))
					{
						D = 1;
					}
					if(!strcmp(SDL_GetKeyName(event.key.keysym.sym), "W"))
					{
						W = 1;
					}
					else if(!strcmp(SDL_GetKeyName(event.key.keysym.sym), "S"))
					{
						S = 1;
					}
					break;
				case SDL_KEYUP:
					if(!strcmp(SDL_GetKeyName(event.key.keysym.sym), "A"))
					{
						A = 0;
					}
					if(!strcmp(SDL_GetKeyName(event.key.keysym.sym), "D"))
					{
						D = 0;
					}
					if(!strcmp(SDL_GetKeyName(event.key.keysym.sym), "W"))
					{
						W = 0;
					}
					else if(!strcmp(SDL_GetKeyName(event.key.keysym.sym), "S"))
					{
						S = 0;
					}
				    break;
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
		
		//printf("frame: %i\n", frame);
		
		//Sleep until we have taken up enough time.
		int ClocksThisFrame = clock()-FrameStartClock;
		int ClocksToSleep = ClocksPerFrame - ClocksThisFrame;
		if(ClocksToSleep > 0)
		{
			usleep((int)(((double)ClocksToSleep) / ClocksPerFrame / FrameRate * 1000000));
		}
		else
		{
			printf("LAG FRAME!\n");
		}
		
		frame++;
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
