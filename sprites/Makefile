all: test_object/spritesheet.png test_object/shadow.png
	@echo "Sprites done"

test_object/spritesheet.png:
	@echo "Making spriteheet"
	@python3 spritecombiner.py;
	
test_object/shadow.png:
	@echo "Making shadow"
	@python3 shadowmaker.py;
