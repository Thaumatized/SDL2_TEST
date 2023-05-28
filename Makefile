all: bin/laser-tank sprites

bin/laser-tank: main.c
	@echo "compiling game"
	@mkdir -p bin
	gcc main.c -o bin/laser-tank `sdl2-config --cflags --libs` -lSDL2_image -lm;
	
.PHONY: sprites
sprites:
	@echo "Moving to sprites"
	@cd sprites && $(MAKE)
	
clean:
	rm bin -r