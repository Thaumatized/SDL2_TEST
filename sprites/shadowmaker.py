from PIL import Image

output_filename = "sprites/test_object/shadow.png"

image = Image.new("RGBA", (64, 32))

for y in range(64):
    for x in range(32):
        darkness = int(((0.5 - abs(y-32)/64) + (0.5 - abs(x-16)/32)) * 255)

        if darkness < 50:
            darkness = 0

        image.putpixel((y,x), (0, 0, 0, darkness))

image.save(output_filename)