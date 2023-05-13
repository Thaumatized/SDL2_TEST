import os
from PIL import Image

sprite_folder = "monkey"
output_filename = "test_object/spritesheet.png"

# Get a list of all sprite paths in the folder
sprite_paths = [os.path.join(sprite_folder, filename) for filename in os.listdir(sprite_folder) if filename.endswith(".png")]

def spritepathsorter(path):
    return(int(path[7:7+4]))

sprite_paths.sort(key=spritepathsorter)

# Load the first sprite to get its dimensions
first_sprite = Image.open(sprite_paths[0])
sprite_width, sprite_height = first_sprite.size

# Calculate the dimensions of the sprite sheet
num_sprites = len(sprite_paths)
sheet_width = sprite_width * num_sprites
sheet_height = sprite_height * 60

# Create a new blank image for the sprite sheet
sheet_image = Image.new("RGBA", (sheet_width, sheet_height))

# Paste each sprite onto the sprite sheet
for row in range(60):
    for i, sprite_path in enumerate(sprite_paths):
        sprite = Image.open(sprite_path)
        sheet_image.paste(sprite, (i * sprite_width, row * sprite_height + row - 30))

# Save the sprite sheet
sheet_image.save(output_filename)