with open('layer.txt', 'r') as layer_file:
	keycodes = layer_file.readlines()
layer_file.close()

with open('line-transpose.txt', 'r') as lines_file:
	transpose = lines_file.readlines()
lines_file.close()

transpose =  [number.strip('\n') for number in transpose]

new_list = [keycodes[int(number)-1] for number in transpose]

with open('out.txt', 'w') as out_file:
	for keycode in new_list:
		out_file.write(keycode)
out_file.close()
