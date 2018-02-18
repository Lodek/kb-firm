#!/usr/bin/python
import os, re, sys
from keycodes import hid_keys


behaviors = {'C': '3', 'ND': '2', 'NH': '1', 'NDH': '4', 'N': '0'}

moddic = {'C' : 0x0100, 'S': 0x0200, 'A':0x0400, 'G':0x0800}

class Key:
    def __init__(self, behavior, keycodes):
        self.behavior = behavior
        self.keycodes = keycodes


class Layer:
    def __init__(self, name, file_path, keys=0, indexes=0, keycodes=0, behaviors=0):
        self.keys = keys
        self.name = name
        self.file_path = file_path
        self.indexes=indexes
        self.keycodes=keycodes
        self.behaviors=behaviors


def gen_layers(layers):
    """ 
    layers is a list with the file names of the layer files ie [l0,l1,l3,l5]
    gen_layers open each file (using the directory passed as argument), read the lines of each file and returns a list of lists with the contents for all layers
    """
    new_layers = []
    for layer in layers:
        nl = Layer(name=layer, file_path=path+'/'+layer)
        with open(nl.file_path, 'r') as lf:
            nl.keys = [line.strip('\n') for line in lf.readlines()]
        new_layers.append(nl)
    return new_layers

def transpose_layer(layer,transpose):
    """
    tranpose layer expect a list that corresponds to a layer file and the tranpose list
    for each item in tranpose, it appends the key at line number x-1 from the layer to that position on the list (i.e)
    Layer file =
    a
    b
    c

    Tranpose file =
    2
    1
    3

    Result=
    b
    a
    c

    Note the layer layer[linenum-1], since lists start at index 0, this is nescessary. In the tranpose file I've defined \0 to mean fill. 
    Fill can be useful when the matrix has coordinates not mapped to any key.
    The return is the new layer generated
    """
    layer.keys = [layer.keys[linenum-1] if linenum != 0 else '\\0' for linenum in transpose]
    return layer


def line_parser(line):
    """
    line parser expects a line as written in the layer file. It can have 4 major formats
    // -> means a clear key (1)
    KEY (i.e A or Ctrl) -> Simple key (2)
    CSAG__KEY -> Key + modifier. (Note the __ as spacer (3)
    // BEHAVIOR KEY1 KEY2 ... (ie // ndh esc caps control) -> keys with special behavior. // also works as the escape sequence (4)

    Given an line of that format, line_parser returns a key object for that line. 
    The key object has as attribute keycode (HID Format)  and the behavior of that key (defined on the firmware)
    """
    line = line.split(' ') #space is used as divider
    if len(line) == 1:
        #line will have len 1 if it fits format 1, 2 or 3
        #Out of those, format 1 ('//') needs special handling (since it is a clear key) thus the if.
        key = Key(behavior=behaviors['N'], keycodes=[key_parser(line[0])]) if line[0] != '//' else Key(behavior=behaviors['C'], keycodes=[])
    else:
        #line will have format 4. line[1] should be the behavior, line[2]... are the keycodes
        behavior = behaviors[line[1].upper()] #behavior dics expect behavior to be upper case
        toggle = 0x01000000 #numerical value of toggle (as defined in firmware)
        #loops thourgh the behavior string ( defined in behaviors dictionary). Each behavior character corresponds to an expected keycode.
        #If behavior character is capitalized, that signifies a toggle
        # Toggle example: keycode = 0x00000004 ; key with toggle = 0x01000004
        keycodes = [key_parser(line[2+index]) | toggle if b.isupper() else key_parser(line[2+index]) for index, b in enumerate(line[1])]
        key = Key(behavior=behavior, keycodes=keycodes)
    return key


def key_parser(key):
    """ 
    Takes key and checks dictionaries to generated key according to HID scan codes
    key may have 3 formats:
    A, Ctrl, SPC (simple key)
    CSAG__SPC (modifier + key)
    l0, l1, l3 (layer key)
    returns key hid scan code formatted for the firmware
    """
    key = key.split('__') # '__' used as separator between mods and keys
    if len(key) == 1:
        #one liner checks for layer key using specified regex
        #if it fails the regex test, it assumes it to be a key in the hid_keys dictionary and returns it.
        return int(key[0][1:]) << 16 if re.match('[lL][0-9]+', key[0]) is not None else hid_keys[key[0]]
    else:
        return mod_value(key[0]) | hid_keys[key[1]]


def mod_value(mod):
    """
    mod is expected to be a string where each letter corresponds to a hid modifier.
    ie CSG - means Ctrl + Shift + GUI(Super) 
       AC - Alt + Ctrl
    Notice, modifiers can be in any order.
    Returns the byte that represents the value for those modifiers
    """
    val = 0
    for m in mod:
        val |= moddic[m]
    return val
        

def gen_defines(layers):
    for layer in layers:
        b = []
        k = []
        i = []
        index = 0
        for key in layer.keys:
            keyb = key.behavior
            b.append(keyb)
            i.append(index)
            k.extend(key.keycodes)
            if keyb == '0': index += 1
            elif keyb == '1': index += 2
            elif keyb == '2': index += 2
            elif keyb == '3': index += 0
            elif keyb == '4': index += 3
        layer.behaviors = b
        layer.keycodes = k
        layer.indexes = i

def c_array(layer):
    string = '{'
    for key in layer:
        string += '{}, '.format(key)
    string = string[:-2]+'}'
    return string

def c_array_hex(layer):
    string = '{'
    for key in layer:
        string += '0x{:08x}, '.format(int(key))
    string = string[:-2]+'}'
    return string

    
def main():
    ls = os.listdir(path) #list containing all files in the directory

    layers = [re.match('[0-9]+', l).string for l in ls if re.match('[0-9]+', l) is not None] #filters ls for layer files. Each regex match is added to the list
    layers = sorted(layers, key=int)
    layers = gen_layers(layers)

    if 'transpose' in ls: #if there is a transpose file in the directory
        with open(path+'/transpose', 'r') as tf:
            transpose = [line.strip('\n') for line in tf.readlines()]
            transpose = [int(line) for line in transpose]
            #transpose is a list of numbers, each number corresponds to the line number on the layer file
        layers = [transpose_layer(layer, transpose) for layer in layers]
        
    for layer in layers:
        layer.keys = [line_parser(line) for line in layer.keys]
    
    gen_defines(layers)

    behaviors_matrix = ''
    indexes_matrix = ''
    maps = ''
    for layer in layers:
        layer.keycodes = c_array_hex(layer.keycodes)+';'
        behaviors_matrix += '{},'.format(c_array(layer.behaviors))
        indexes_matrix += '{},'.format(c_array(layer.indexes))
        maps += 'l'+layer.name+','
    behaviors_matrix = behaviors_matrix[:-1]
    indexes_matrix = indexes_matrix[:-1]
    print('#define BEHAVIORS {}'.format(behaviors_matrix))
    print('#define INDEXES {}'.format(indexes_matrix))
    for layer in layers:
          print('#define L{} const long l{}[] PROGMEM = {}'.format(layer.name, layer.name, layer.keycodes))
    print('#define MAP {}'.format(maps[:-1]))
          
path = sys.argv[1] #gets directory with layer files
if __name__ == '__main__':
    main()
