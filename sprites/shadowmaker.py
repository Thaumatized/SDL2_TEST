from PIL import Image
import math

output_filename = "bin/sprites/shadow.png"

size_x = 64
size_y = 32

image = Image.new("RGBA", (size_x, size_y))

for x in range(size_x):
    for y in range(size_y):
        distanceFromCenter = math.sqrt(pow(abs(((size_x/2)-x)/(size_x/2)), 2) + pow(abs(((size_y/2)-y)/(size_y/2)), 2))
        distanceFromCenter *= 0.98 # makes the shadow a bit smaller. Stops clipping at the edges.
        darkness = 255 - int(distanceFromCenter*255)

        if darkness < 50:
            darkness = 0

        image.putpixel((x, y), (0, 0, 0, darkness))

image.save(output_filename)
