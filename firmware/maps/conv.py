#!/usr/bin/python
import sys

hid_codes = {'RIGHT': 79, '|': 8241, 'HOME': 74, '[': 47, 'F12': 69, '_': 8237, 'F1': 58, ';': 51, 'GUI': 2048, '0': 39, '*': 8229, 'F6': 63, 'CTRL': 256, 'E': 8200, 'v': 25, '+': 8238, 's': 22, '.': 55, 'R': 8213, 'q': 20, 'W': 8218, 'L': 8207, '{': 8239, 'F9': 66, 'r': 21, '`': 53, 'b': 5, 'TAB': 43, 'e': 8, 'Q': 8212, '/': 56, '&': 8228, 'a': 4, 'PAGEDOWN': 78, 'ESC': 41, '7': 36, '}': 8240, 'F8': 65, '5': 34, 'F': 8201, 'o': 18, 'ENTER': 40, ',': 54, 'M': 8208, 'LEFT': 80, 'END': 77, 'X': 8219, '#': 8224, 'UP': 82, 'P': 8211, 'DEL': 76, 'l': 15, 'SHIFT': 512, 'f': 9, 't': 23, '$': 8225, 'G': 8202, ':': 8243, 'PAGEUP': 75, '8': 37, '3': 32, 'BACKSPC': 42, 'x': 27, '2': 31, 'minus': 45, 'F3': 60, 'z': 29, '>': 8247, 'A': 8196, 'PRINTSCREEN': 70, '4': 33, 'CAPS': 57, '?': 8248, 'h': 11, 'N': 8209, 'd': 7, 'c': 6, 'SCROLLLOCK': 71, 'PAUSE': 72, 'T': 8215, '(': 8230, 'J': 8205, 'D': 8199, '!': 8222, 'SPC': 44, 'O': 8210, 'ALT': 1024, '1': 30, 'Z': 8221, 'F5': 62, 'u': 24, 'j': 13, '%': 8226, 'F11': 68, '<': 8246, '6': 35, 'H': 8203, 'i': 12, '\\': 49, 'U': 8216, 'INSERT': 73, 'F7': 64, 'p': 19, '@': 8223, '9': 38, 'w': 26, 'C': 8198, 'F4': 61, ')': 8231, '~': 8245, 'F2': 59, ']': 48, 'V': 8217, 'F10': 67, 'I': 8204, 'm': 16, 'B': 8197, '"': 8244, '=': 46, 'DOWN': 81, '^': 8227, 'S': 8214, 'K': 8206, 'g': 10, 'Y': 8220, "'": 52, 'y': 28, 'n': 17, 'k': 14, '\\0' : 0}

behaviors = {'C': '3', 'N-D': '2', 'N-H': '1', 'N-D-H': '4', 'N': '0'}

mod_codes = {'C' : 0x0100, 'S': 0x0200, 'A':0x0400, 'G':0x0800}

def mod_parser(mods):
	mod_value = 0;
	for mod in mods:
		mod_value = mod_value | mod_codes[mod]
	return mod_value


def keycode_gen(key):
	#nescessary otherwise user couldn't use - in mapping declarations
	if len(purple) == 1 : # single character in line (A, B, C, 1, _)
		keycode = hid_codes[key]
	else:
		key = key.split('-')
		if len(key) == 1:
			keycode = hid_codes[key[0]]
		else:
			mod = mod_parser(key[0])
			keycode = hid_codes[key[1]]
			keycode = keycode | mod
	return keycode


with open(sys.argv[1], 'r') as mapping_file:
	purple = mapping_file.readlines()
mapping_file.close()

purple = [blue.strip('\n') for blue in purple]
purple = [blue.split(' ') for blue in purple]

keycodes = []

for line in purple:
	if line[0] != "//":
		keycode = keycode_gen(line[0])
		keycodes.append("{}{:08x}".format(behaviors['N'], keycode))
	else:
		behavior_code = behaviors[line[1].upper()]
		keycode_str = behavior_code
		behavior = line[1].split('-')
		counter = 0
		for i in behavior:
			keycode = keycode_gen(line[2+counter])
			if i.isupper():
				keycode = keycode | 0x01000000
			keycode_str = keycode_str + '0x{:08x}'.format(keycode)
			counter += 1
		keycodes.append(keycode_str)
output_st='{'
for ey in keycodes:
	output_st = output_st + '"{}",'.format(ey)
output_st = output_st[:-1]
output_st = output_st + '}'
print (output_st)
