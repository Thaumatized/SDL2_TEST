#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <math.h>

#ifdef __linux__
	#include <unistd.h>
#endif

#ifdef __MINGW32__
	//for printf
	#include <stdio.h>
	// for getPathToExecutable
	#include <libloaderapi.h>
#endif

#include "enchant-engine/configReader.c"
#include "enchant-engine/enchant-engine.h"
#include "enchant-engine/input.h"
#include "enchant-engine/physics.h"

#define SPRITE_ORIENTATIONS (72)
#define MAX_FILE_PATH (1024)

int WINDOW_X = 640;
int WINDOW_Y = 360;
int DISPLAY_MODE = 0; // 0 == windowed

void getConfig(char* pathToExecutable) {
	printf("path to config file\n");
	char path[MAX_FILE_PATH];
	memset(path, 0, MAX_FILE_PATH);
	strcat(path, pathToExecutable);
	strcat(path, "config.ini");

	char displayMode[MAX_LINE];
	memset(displayMode, 0, MAX_LINE);

	config_fetch_options config_options[] = {
		{"WINDOW_X", &WINDOW_X, CONFIG_INT},
		{"WINDOW_Y", &WINDOW_Y, CONFIG_INT},
		{"DISPLAY_MODE", displayMode, CONFIG_STRING},
	};

	printf("get config\n");
	getConfigurations(path, config_options, 3);

	printf("interperet display mode\n");

	if(strcmp(displayMode, "fullscreen") == 0)
	{
		DISPLAY_MODE = SDL_WINDOW_FULLSCREEN;
	}
	else if(strcmp(displayMode, "windowed fullscreen") == 0)
	{
		DISPLAY_MODE = SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	else if(strcmp(displayMode, "windowed") == 0)
	{
		DISPLAY_MODE = 0; //Windowed
	}
}

float degsin(float deg) {return sin(deg*0.0174532925);}
float degcos(float deg) {return cos(deg*0.0174532925);}
float degtan(float deg) {return tan(deg*0.0174532925);}
float degasin(float val) {return 57.2957795*asin(val);}
float degacos(float val) {return 57.2957795*acos(val);}
float degatan(float val) {return 57.2957795*atan(val);}

void getPathToExecutable(char* buf, int bufLen)
{
	#ifdef __linux__
		readlink("/proc/self/exe", buf, bufLen);

		for(int i = bufLen - 1; i >= 0; i--)
		{
			if(buf[i] == '/')
			{
				break;
			}
			buf[i] = 0;
		}
	#endif
	#ifdef __MINGW32__
		GetModuleFileName(NULL, buf, bufLen);

		for(int i = bufLen - 1; i >= 0; i--)
		{
			if(buf[i] == '\\')
			{
				break;
			}
			buf[i] = 0;
		}
	#endif
}

int rotToFrame(float rot) { return (int)(rot  / (360.0 / (float)SPRITE_ORIENTATIONS) + 0.5) % SPRITE_ORIENTATIONS; }

Vector2 rotToVector2(float rot)
{
	int ySign = 1 - (rot > 90 || rot < 270)*2;

	Vector2 result = {
		degsin(rot),
		degcos(rot)*ySign
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

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *shadowTexture;

int testEntity;
PhysicsComponent testEntityPhysics;

int monkeyHeads[4];
PhysicsComponent monkeyHeadPhysics[4];

SDL_Surface *monkeySpriteSheet;


//Binding indexes
int bindingForward;
int bindingBack;
int bindingLeft;
int bindingRight;
int bindingQuickTurn;

void initialize(int argc, char* argv[])
{
	printf("Binding inputs \n");
	bindingForward = bindAction("Forward", "w;Up");
	bindingBack = bindAction("Backward", "s;Down");
	bindingLeft = bindAction("Left", "a;Left");
	bindingRight = bindAction("Right", "d;Right");
	bindingQuickTurn = bindAction("Quick Turn", "Left Ctrl+R");

	printf("path to excutable\n");
	char pathToExecutable[MAX_FILE_PATH];
	memset(pathToExecutable, 0, MAX_FILE_PATH);
	getPathToExecutable(pathToExecutable, MAX_FILE_PATH-1);
	char path[MAX_FILE_PATH];
	memset(path, 0, MAX_FILE_PATH);

	printf("config\n");
	getConfig(pathToExecutable);

	printf("SDL init\n");
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Failed to initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	window = SDL_CreateWindow("My SDL2 Window",
		                                  SDL_WINDOWPOS_UNDEFINED,
		                                  SDL_WINDOWPOS_UNDEFINED,
		                                  WINDOW_X, WINDOW_Y,
		                                  SDL_WINDOW_SHOWN);
		                                  
	SDL_SetWindowFullscreen(window, DISPLAY_MODE);
		                                
	if (!window) {
		printf("Failed to create SDL window: %s\n", SDL_GetError());
		exit(1);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		printf("Failed to create SDL renderer: %s\n", SDL_GetError());
		exit(1);
	}

	//if windowed fullscreen resolution will be the screen resolutions and so it shouldn't be based on settings
	if(DISPLAY_MODE == SDL_WINDOW_FULLSCREEN_DESKTOP)
	{
		SDL_GetWindowSize(window, &WINDOW_X, &WINDOW_Y);
	}

	int FrameRate = 60;
	int ClocksPerFrame = CLOCKS_PER_SEC / FrameRate;
	int frame = 0;

	memset(path, 0, MAX_FILE_PATH);
	strcat(path, pathToExecutable);
	strcat(path, "sprites/shadow.png");
	SDL_Surface *shadowImage = IMG_Load(path);
	shadowTexture = SDL_CreateTextureFromSurface(renderer, shadowImage);

	memset(path, 0, MAX_FILE_PATH);
	strcat(path, pathToExecutable);
	strcat(path, "sprites/monkeysheet.png");
	monkeySpriteSheet = IMG_Load(path);

	//TEST GameObject
	testEntity = createEntity();
	testEntityPhysics = *(createPhysicsComponent(testEntity));
	testEntityPhysics.position.x = WINDOW_X/2;
	testEntityPhysics.position.y = WINDOW_Y/2;

	for(int i = 0; i < 4; i++)
	{
		monkeyHeads[i] = createEntity();
		monkeyHeadPhysics[i] = *(createPhysicsComponent(monkeyHeads[i]));
		monkeyHeadPhysics[i].position.x = (WINDOW_X-256) * (i % 2);
		monkeyHeadPhysics[i].position.y = (WINDOW_Y-256) * (i / 2);;
		monkeyHeadPhysics[i].velocity.x = 0;
		monkeyHeadPhysics[i].velocity.y = 0;
		monkeyHeadPhysics[i].rotation = 0;
	}
}

void update(int frame)
{
	SDL_SetRenderDrawColor(renderer, 153, 138, 78, 255);
	SDL_RenderClear(renderer); //erase

	int animFrame = 0;
	int rotFrame = 0;

	//Rotating monkey heads
	for(int i = 0; i < 4; i++)
	{
		monkeyHeadPhysics[i].rotation = (monkeyHeadPhysics[i].rotation + 1);
		if(monkeyHeadPhysics[i].rotation < 0) monkeyHeadPhysics[i].rotation += 360;
		if(monkeyHeadPhysics[i].rotation > 360) monkeyHeadPhysics[i].rotation -= 360;
		rotFrame = rotToFrame(monkeyHeadPhysics[i].rotation);
		animFrame = (frame+(30*i))%119-59;
		if(animFrame < 0)
		{
			animFrame *= -1;
		}
		SDL_Surface *img = SDL_CreateRGBSurface(0, 128, 128, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
		SDL_Rect srcrect = { rotFrame*128, animFrame*128, 128, 128 };
		SDL_Rect dstrect = { 0, 0, 0, 0 };
		SDL_BlitSurface(monkeySpriteSheet, &srcrect, img, &dstrect);
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, img);
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
		dstrect.x = monkeyHeadPhysics[i].position.x;
		dstrect.y = monkeyHeadPhysics[i].position.y;
		dstrect.w = 256;
		dstrect.h = 256;
		SDL_RenderCopy(renderer, texture, NULL, &dstrect);

		SDL_FreeSurface(img);
		SDL_DestroyTexture(texture);
	}

	//TEST GameObject
	testEntityPhysics.rotation += (-actionPressed(bindingLeft) + actionPressed(bindingRight)) * 6;
	if(actionPressedThisFrame(bindingQuickTurn))
	{
		testEntityPhysics.rotation += 180;
	}
	if(testEntityPhysics.rotation < 0) testEntityPhysics.rotation += 360;
	if(testEntityPhysics.rotation > 360) testEntityPhysics.rotation -= 360;
	rotFrame = rotToFrame(testEntityPhysics.rotation);
	/*
	//This would cause the monkey head to float up and down.
	//I find this undesirable for the playercontrolled one.
	//This will be replaced with advanced logic once the controlled GameObject is an actual tank.
	//Kept this for reference.
	animFrame = frame%119-59;
	if(animFrame < 0)
	{
		animFrame *= -1;
	}
	*/
	animFrame = 59;

	testEntityPhysics.velocity = multiplyVector2(rotToVector2(testEntityPhysics.rotation), (-actionPressed(bindingBack)+actionPressed(bindingForward))*15);
	
	testEntityPhysics.position.x += testEntityPhysics.velocity.x;
	testEntityPhysics.position.y += testEntityPhysics.velocity.y;
	SDL_Surface *img = SDL_CreateRGBSurface(0, 128, 128, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	SDL_Rect srcrect = { rotFrame*128, animFrame*128, 128, 128 };
	SDL_Rect dstrect = { 0, 0, 0, 0 };
	SDL_BlitSurface(monkeySpriteSheet, &srcrect, img, &dstrect);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, img);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	//SHADOW
	dstrect.x = testEntityPhysics.position.x + 64;
	dstrect.y = testEntityPhysics.position.y + 128 + 64;
	dstrect.w = 128;
	dstrect.h = 64;
	SDL_RenderCopy(renderer, shadowTexture, NULL, &dstrect);
	//GameObject it self
	dstrect.x = testEntityPhysics.position.x;
	dstrect.y = testEntityPhysics.position.y;
	dstrect.w = 256;
	dstrect.h = 256;
	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	if(testEntityPhysics.position.x > WINDOW_X)
	{
		testEntityPhysics.position.x -= WINDOW_X + 256;
	}
	if(testEntityPhysics.position.x < 0 - 256)
	{
		testEntityPhysics.position.x += WINDOW_X + 256;
	}
	if(testEntityPhysics.position.y > WINDOW_Y)
	{
		testEntityPhysics.position.y -= WINDOW_Y + 256;
	}
	if(testEntityPhysics.position.y < 0 - 256)
	{
		testEntityPhysics.position.y += WINDOW_Y + 256;
	}

	SDL_FreeSurface(img);
	SDL_DestroyTexture(texture);
	
	SDL_RenderPresent(renderer);
	
	frame++;
}

void cleanUp()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}