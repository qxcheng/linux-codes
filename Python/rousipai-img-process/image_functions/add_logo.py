#!/usr/bin/env python3
# -*- coding: utf-8 -*-
__author__ = 'Bo Lin'

from PIL import Image
import numpy as np
import cv2


# 锦上添花 添加TIC logo
def add_logo(image, logo="bz", logo_margin=(30, 30)):
    print(logo, 'logo')
    if logo == "tic":
        logo_image = Image.open("../examples/logo/tic.png")
        logo_image = logo_image.resize((int(logo_image.size[0] / 3), int(logo_image.size[1] / 3)), Image.ANTIALIAS)
    elif logo == "bz":
        logo_image = Image.open("../examples/logo/bz.png")
        logo_image = logo_image.resize((int(logo_image.size[0] / 10), int(logo_image.size[1] / 10)), Image.ANTIALIAS)
    else:
        logo_image = Image.open(logo)
    if image.mode == "RGB":
        image_rgba = cv2.cvtColor(np.array(image), cv2.COLOR_RGB2RGBA)
        img = Image.fromarray(image_rgba)
    elif image.mode == "RGBA":
        img = image
    elif image.mode == "P":
        img = image.convert("RGBA")
    else:
        print("wrong image type: " + str(image.mode))
        return None

    top_margin = logo_margin[0]
    left_margin = logo_margin[1]
    logo_canvas = Image.new("RGBA", image.size, (255, 255, 255, 0))
    logo_canvas.paste(logo_image, (left_margin, top_margin))

    logo_canvas_new = Image.fromarray(np.array(logo_canvas))
    image_logo = Image.alpha_composite(img, logo_canvas_new)
    return image_logo


if __name__ == "__main__":
    image_i = Image.open("../examples/images/1.jpg")
    status, image_final = add_logo(image_i)
    image_final.show()
