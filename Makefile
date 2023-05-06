compile:
	gcc main.c -o laser-tank `sdl2-config --cflags --libs` -lSDL2_image;