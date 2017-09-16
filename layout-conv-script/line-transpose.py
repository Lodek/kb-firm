import sys

with open(sys.argv[1], 'r') as layer_file:
	keycodes = layer_file.readlines()
layer_file.close()

with open(sys.argv[2], 'r') as lines_file:
	transpose = lines_file.readlines()
lines_file.close()

transpose =  [number.strip('\n') for number in transpose]

new = []
for number in transpose:
	if int(number) == 0:
		new.append('0x000000000x00000000')
	else:
		new.append(keycodes[int(number-1)])

for keycode in new:
	print (keycode)
