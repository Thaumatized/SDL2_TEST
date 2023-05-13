compile:
	mkdir -p bin
	gcc main.c -o bin/laser-tank `sdl2-config --cflags --libs` -lSDL2_image;
