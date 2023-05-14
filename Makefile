compile:
	mkdir -p bin
	python3 sprites/spritecombiner.py;
	gcc main.c -o bin/laser-tank `sdl2-config --cflags --libs` -lSDL2_image;
