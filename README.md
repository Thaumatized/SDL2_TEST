This repo is used to test SDL2.

required library installations;
sudo apt install libsdl2-dev;
sudo apt install libsdl2-image-dev;

compile && run:
gcc main.c -o laser-tank `sdl2-config --cflags --libs` -lSDL2_image; ./laser-tank
or with make
make; ./bin/laser-tank
