from PIL import Image

def findFirstNoneBlackPixelX(image, width, height):
	for x in range(width):
		for y in range(height):
			pixel = image.getpixel((x, y))
			if pixel[0] != 0 or pixel[1] != 0 or pixel[2] != 0:
				return x
	return width

def findFirstNoneBlackPixelY(image, width, height):
	for y in range(height):
		for x in range(width):
			pixel = image.getpixel((x, y))
			if pixel[0] != 0 or pixel[1] != 0 or pixel[2] != 0:
				return y
	return height

def findLastNoneBlackPixelX(image, width, height):
	for x in range(width - 1, -1, -1):
		for y in range(height):
			pixel = image.getpixel((x, y))
			if pixel[0] != 0 or pixel[1] != 0 or pixel[2] != 0:
				return x
	return 0

def findLastNoneBlackPixelY(image, width, height):
	for y in range(height - 1, -1, -1):
		for x in range(width):
			pixel = image.getpixel((x, y))
			if pixel[0] != 0 or pixel[1] != 0 or pixel[2] != 0:
				return y
	return 0

def delete_black_pixels(image_path):
	image = Image.open(image_path)
	width, height = image.size
	x1 = findFirstNoneBlackPixelX(image, width, height)
	y1 = findFirstNoneBlackPixelY(image, width, height)
	x2 = findLastNoneBlackPixelX(image, width, height)
	y2 = findLastNoneBlackPixelY(image, width, height)
	image = image.crop((x1, y1, x2, y2))
	image.save(image_path)


Couleur = ["Coeur", "Carreau", "Trefle", "Pique"]
Valeur = ["As", "Sept", "Huit", "Neuf", "Dix", "Valet", "Dame", "Roi"]

for i in range(4):
	for j in range(8):
		delete_black_pixels('ressources/'+Couleur[i]+'/'+Valeur[j]+'.png')