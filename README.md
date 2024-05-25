# laser tank
This repo is used to test SDL2 and quite possible is the beginnings of a custom game engine.  
Why do things the easy way?  
  
Anways, since the best way to do both of these things is to make a game, this will be a wavedefense game. Enjoy!  

## Compilation for linux (mint)
### Prerequisites:
> sudo apt install libsdl2-dev;  
> sudo apt install libsdl2-image-dev;  

also make sure to have python3 and pillow installed.
  
### compile && run:  
> make; ./bin/laser-tank  

## Compilation for Windows
### Notes:
Compilation for windows is done as cross compilation from linux atleast for now.

### Prerequisites:
> sudo apt install mingw-w64;

Download the [SDL developement](https://github.com/libsdl-org/SDL/releases) libary (SDL2-devel-VERSION-mingw.zip) and [SDL_image](https://github.com/libsdl-org/SDL_image/releases) library (SDL2_image-devel-VERSION-mingw.zip).

Extract the 64 bit include and lib folders into a folder called "SDL2" from both.

Extract the 64 bit binaries (.dll) to bin from both.

### compile && run:  
Haven't bothered with make for windows yet.
  
Sprites:
> mkdir -p bin/sprites/; python3 sprites/spritecombiner.py; python3 sprites/shadowmaker.py;
  
Program:
> x86_64-w64-mingw32-gcc main.c -o bin/laser-tank.exe -ISDL2/include/ -LSDL2/lib/ -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
