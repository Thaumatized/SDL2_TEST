all: mkdir bin/laser-tank bin/sprites

.PHONY: mkdir clean

mkdir:
	@mkdir -p bin/sprites/

bin/laser-tank: main.c
	@echo "compiling game"
	gcc main.c -o bin/laser-tank `sdl2-config --cflags --libs` -lSDL2_image -lm;
	
bin/sprites: bin/sprites/monkeysheet.png bin/sprites/shadow.png
	@echo "Sprites done"

bin/sprites/monkeysheet.png: sprites/spritecombiner.py sprites/monkey/*
	@echo "Making spriteheet"
	@python3 sprites/spritecombiner.py;
	
bin/sprites/shadow.png: sprites/shadowmaker.py
	@echo "Making shadow"
	@python3 sprites/shadowmaker.py;

clean:
	rm bin -r
