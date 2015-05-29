#!/usr/bin/python2.7

from PIL import Image


def is_white(item):
    return item[0] == 255 and item[1] == 255 and item[2] == 255


def convert_transparent(img):
    img = img.convert("RGBA")

    transparent = 255, 255, 255, 0
    newData = [transparent if is_white(item) else item
               for item in img.getdata()]

    img.putdata(newData)

    return img

numbers = ['1', '2', '3', '4', '5', '10']
files = ['filtered/out' + number + '.png' for number in numbers]

# Magic
x_mins = [438041, 438187, 438659, 437872, 437900, 437297]
x_maxs = [439937, 439389, 440756, 439100, 439195, 438657]
y_mins = [8755343, 8757836, 8756167, 8756753, 8758251, 8755970]
y_maxs = [8757979, 8759085, 8757808, 8758036, 8758952, 8757649]

size = max(x_maxs) - min(x_mins), max(y_maxs) - min(y_mins)
out = Image.new("RGBA", size, "white")

images = [convert_transparent(Image.open(file)) for file in files]
data = zip(x_mins, y_mins, images)

for x, y, image in data:
    out.paste(image, (x - min(x_mins), y - min(y_mins)), image)

out.save('res/out.png')
