# laser tank
This repo is used to test SDL2 and quite possible is the beginnings of a custom game engine.  
Why do things the easy way?  
  
Anways, since the best way to do both of these things is to make a game, this will, eventually, maybe, be a wavedefense game. Enjoy!  

## Compilation for linux (mint)
### Prerequisites:
Make sure to have python3 and pillow installed.  
Be sure to get the enchant engine.
> git submodule update --init

Install sdl2 and sdl2-image developement libaries:
> sudo apt install libsdl2-dev;  
> sudo apt install libsdl2-image-dev;  
### compile && run:  
> make linux; ./bin/laser-tank  

## Compilation for Windows from linux (mint)
### Prerequisites:
Make sure to have python3 and pillow installed.  
Be sure to get the enchant engine.
> git submodule update --init

install mingw64 for compilation
> sudo apt install mingw-w64;

Download the [SDL developement](https://github.com/libsdl-org/SDL/releases) libary (SDL2-devel-VERSION-mingw.zip) and [SDL_image](https://github.com/libsdl-org/SDL_image/releases) library (SDL2_image-devel-VERSION-mingw.zip).

Extract the 64 bit include, lib and bin folders into a folder called "SDL2" from both, merging the folders.

### compile && run:  
> make windows; ./bin/laser-tank.exe

## Other things about compilation
### All platforms
calling make alone, or with all, will compile for all platforms.
> make
or
> make all
### Clean
Make has the "clean" target which removes the binary folder. This is seldom needed, but it can be helpful if there is something weird in the folder, such as a bad config file.
Removes bin folder:
> make clean

Removes bin and compiles for a platform
> make clean linux

Removes bin and compiles for every platform
> make clean all  