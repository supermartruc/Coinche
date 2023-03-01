from PIL import Image

def replace_black_pixels(image_path):
	# Open the image and get its dimensions
	image = Image.open(image_path)
	width, height = image.size

	# Create a new transparent image with the same dimensions
	new_image = Image.new('RGBA', (width, height), (0, 0, 0, 0))

	# Loop through each pixel in the original image and copy it to the new image,
	# unless it's a black pixel at the edge of the image
	for x in range(width):
		for y in range(height):
			pixel = image.getpixel((x, y))
			if (x <= 4 or x >= width - 5) or (y <= 4 or y >= height - 5):
				if pixel[0] != 0 or pixel[1] != 0 or pixel[2] != 0:
					new_image.putpixel((x, y), pixel)
			else:
				new_image.putpixel((x, y), pixel)
	# Save the new image
	new_image.save(image_path)


Couleur = ["Coeur", "Carreau", "Trefle", "Pique"]
Valeur = ["As", "Sept", "Huit", "Neuf", "Dix", "Valet", "Dame", "Roi"]

for i in range(4):
	for j in range(8):
		replace_black_pixels('ressources/'+Couleur[i]+'/'+Valeur[j]+'.png')

replace_black_pixels('ressources/Pique/Dame.png')