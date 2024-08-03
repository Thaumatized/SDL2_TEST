all: directories common linux windows

.PHONY: directories clean common linux windows

# What you are supposed to call
clean:
	@echo "Cleaning old binaries"
	@rm bin -rf

linux: directories common bin/laser-tank

windows: directories common bin/laser-tank.exe bin/SDL2.dll bin/SDL2_image.dll

#Common
common: directories bin/config.ini bin/sprites

directories:
	@mkdir -p bin/sprites/

bin/config.ini: default-config.ini
	@echo "Copying default config"
	@cp default-config.ini bin/config.ini
	
bin/sprites: bin/sprites/monkeysheet.png bin/sprites/shadow.png
	@echo "Sprites done"

bin/sprites/monkeysheet.png: sprites/spritecombiner.py sprites/monkey/*
	@echo "Making spriteheet"
	@python3 sprites/spritecombiner.py;
	
bin/sprites/shadow.png: sprites/shadowmaker.py
	@echo "Making shadow"
	@python3 sprites/shadowmaker.py;

#Linux
bin/laser-tank: enchant-engine/* game.c
	@echo "Compiling game for Linux"
	@gcc enchant-engine/enchant-engine.c game.c -o bin/laser-tank `sdl2-config --cflags --libs` -lSDL2_image -lm;

#Windows
bin/laser-tank.exe: enchant-engine/* game.c
	@echo "Compiling game for Windows"
	@x86_64-w64-mingw32-gcc enchant-engine/enchant-engine.c game.c -o bin/laser-tank.exe -ISDL2/include/ -LSDL2/lib/ -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

bin/SDL2.dll: SDL2/bin/SDL2.dll
	@cp SDL2/bin/SDL2.dll bin/SDL2.dll

bin/SDL2_image.dll: SDL2/bin/SDL2_image.dll
	@cp SDL2/bin/SDL2_image.dll bin/SDL2_image.dll
	

