#!/usr/bin/python3
import sys

hid_codes = {'RIGHT': 79, '|': 8241, 'HOME': 74, '[': 47, 'F12': 69, '_': 8237, 'F1': 58, ';': 51, 'GUI': 2048, '0': 39, '*': 8229, 'F6': 63, 'CTRL': 256, 'E': 8200, 'v': 25, '+': 8238, 's': 22, '.': 55, 'R': 8213, 'q': 20, 'W': 8218, 'L': 8207, '{': 8239, 'F9': 66, 'r': 21, '`': 53, 'b': 5, 'TAB': 43, 'e': 8, 'Q': 8212, '/': 56, '&': 8228, 'a': 4, 'PAGEDOWN': 78, 'ESC': 41, '7': 36, '}': 8240, 'F8': 65, '5': 34, 'F': 8201, 'o': 18, 'ENTER': 40, ',': 54, 'M': 8208, 'LEFT': 80, 'END': 77, 'X': 8219, '#': 8224, 'UP': 82, 'P': 8211, 'DEL': 76, 'l': 15, 'SHIFT': 512, 'f': 9, 't': 23, '$': 8225, 'G': 8202, ':': 8243, 'PAGEUP': 75, '8': 37, '3': 32, 'BACKSPC': 42, 'x': 27, '2': 31, '-': 45, 'minus': 45, 'F3': 60, 'z': 29, '>': 8247, 'A': 8196, 'PRINTSCREEN': 70, '4': 33, 'CAPS': 57, '?': 8248, 'h': 11, 'N': 8209, 'd': 7, 'c': 6, 'SCROLLLOCK': 71, 'PAUSE': 72, 'T': 8215, '(': 8230, 'J': 8205, 'D': 8199, '!': 8222, 'SPC': 44, 'O': 8210, 'ALT': 1024, '1': 30, 'Z': 8221, 'F5': 62, 'u': 24, 'j': 13, '%': 8226, 'F11': 68, '<': 8246, '6': 35, 'H': 8203, 'i': 12, '\\': 49, 'U': 8216, 'INSERT': 73, 'F7': 64, 'p': 19, '@': 8223, '9': 38, 'w': 26, 'C': 8198, 'F4': 61, ')': 8231, '~': 8245, 'F2': 59, ']': 48, 'V': 8217, 'F10': 67, 'I': 8204, 'm': 16, 'B': 8197, '"': 8244, '=': 46, 'DOWN': 81, '^': 8227, 'S': 8214, 'K': 8206, 'g': 10, 'Y': 8220, "'": 52, 'y': 28, 'n': 17, 'k': 14, '\\0' : 0}

behaviors_codes = {'C': '3', 'N-D': '2', 'N-H': '1', 'N-D-H': '4', 'N': '0'}

mod_codes = {'C' : 0x0100, 'S': 0x0200, 'A':0x0400, 'G':0x0800}

def mod_parser(mods):
	mod_value = 0;
	for mod in mods:
		mod_value = mod_value | mod_codes[mod]
	return mod_value


def keycode_gen(key):
	#nescessary otherwise user couldn't use - in mapping declarations
	if len(key) == 1 : # single character in line (A, B, C, 1, _)
		keycode = hid_codes[key]
	else:
		key = key.split('-')
		if len(key) == 1:
			if (key[0][0] == 'L' and key[0] != 'LEFT'):
				key = key[0].strip('L')
				key= int(key)
				keycode = key << 16
			else:
				keycode = hid_codes[key[0]]
		else:
			mod = mod_parser(key[0])
			keycode = hid_codes[key[1]]
			keycode = keycode | mod
	return keycode

def transpose_layer(layer):
	new = []
	for number in transpose:
		if number == '0':
			new.append('\\0')
		else:
			new.append(layer[int(number)-1])
	return new

layers = []

if sys.argv[len(sys.argv)-1] == 'transpose':
	with open(sys.argv[len(sys.argv)-1], 'r') as transpose_file:
		transpose = transpose_file.readlines()
		transpose =  [number.strip('\n') for number in transpose]
	transpose_file.close()

	for x in range(1,(len(sys.argv)-1)):
		with open(sys.argv[x], 'r') as layer_file:
			layer = layer_file.readlines()
			layer = [key.strip('\n') for key in layer]
		layer_file.close()

		layer = transpose_layer(layer)
		layers.append(layer)

if len(layers) == 0:
	for x in range(1,len(sys.argv)):
		with open(sys.argv[x], 'r') as layer_file:
			layer = layer_file.readlines()
			layer = [key.strip('\n') for key in layer]
		layer_file.close()
		layers.append(layer)

list_behaviors = []
list_indexes = []
list_keycodes = []
for layer in layers:
	layer = [line.split(' ') for line in layer]
	keycodes = []
	behaviors = []
	indexes = []

	for line in layer:
		indexes.append(len(keycodes))
		
		if line[0] != "//":
			keycode = keycode_gen(line[0])
			behaviors.append(behaviors_codes['N'])
			keycodes.append(keycode)
		else:
			behaviors.append(behaviors_codes[line[1].upper()])
			behavior = line[1].split('-')
			counter = 0
			for i in behavior:
				keycode = keycode_gen(line[2+counter])
				if i.isupper():
					keycode = keycode | 0x01000000
				keycodes.append(keycode)
				counter += 1
	list_keycodes.append(keycodes)
	list_behaviors.append(behaviors)
	list_indexes.append(indexes)

behaviorthing = '#define BEHAVIORS '
for behavior in list_behaviors:
	behaviorstr = ''
	for b in behavior:
		behaviorstr = behaviorstr+'{},'.format(b)
	behaviorstr = behaviorstr[:-1]
	behaviorthing = behaviorthing+'{'+'{}'.format(behaviorstr)+'},'
behaviorthing = behaviorthing[:-1]
print(behaviorthing)

print('')


indexthing = '#define INDEXES '
for index in list_indexes:
	indexstr = ''
	for b in index:
		indexstr = indexstr+'{},'.format(b)
	indexstr = indexstr[:-1]
	indexthing = indexthing+'{'+'{}'.format(indexstr)+'},'
indexthing = indexthing[:-1]
print(indexthing)

print('')
count =0
lnames = []
for unicorn in list_keycodes:
	layer_dec = '#define L{} const long l{}'.format(count,count) +'[NUM_KEYS] PROGMEM = {'
	lnames.append('l{}'.format(count))
	layer_str = ''
	for horn in unicorn:
		layer_str = layer_str+'0x{:08x},'.format(horn)
	layer_str = layer_str[:-1]
	layer_dec += '{}'.format(layer_str) + '};'
	print(layer_dec)
	print('')
	count += 1

mapstr = '#define MAP '
for l in lnames:
	mapstr += '{},'.format(l)
mapstr = mapstr[:-1]
print('')
print(mapstr)
