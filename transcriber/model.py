from map_errors import LineError, DataFileError
from librarian import Librarian
from pathlib import Path
import re

lib = Librarian()
c_wrapper = lambda s : '{{{}}}'.format(s)

class Definer:

    def __init__(self, layers, macros):
        self.macros = macros
        self.layers = layers
        self.check_layers_len()
        self.keys_len = len(self.layers[0])
        self.layers_len = len(self.layers)
        self.macros_len = len(self.macros)
        self.layers_value = ','.join([str(layer.id) for layer in self.layers])

    def check_layers_len(self):
        """Checks that all layer files had the same original length. 
        If lens are different, layer map file is wrong"""
        lens = [layer.file_len for layer in self.layers]
        sample = lens[0]
        for l in lens:
            if sample != l:
                raise DataFileError

    def define_all(self):
        defines = []
        defines.append(self.define('KEYS_LEN', self.keys_len))
        defines.append(self.define('LAYERS_LEN', self.layers_len))
        defines.append(self.define('MACROS_LEN', self.macros_len))
        defines.append(self.define('NAMES', self.names()))
        defines.append(self.define('MACROS', self.macros_def()))
        defines.append(self.define('LAYERS_VAL', self.layers_value))

        for define in defines:
            print(define +'\n')
        
    def names(self):
        return c_wrapper(','.join([str(layer) for layer in self.layers]))

    def macros_def(self):
        return c_wrapper(','.join([macro.flat_quantas() for macro in self.macros]))

    def define(self, who, what):
        return '#define {} {}'.format(who, what)

class Transpose:

    """Transpose class responsible for adequating the keys in the order chosen
    with the physical wiring of it"""

    def __init__(self, path):
        self.path = path
        with self.path.open() as f:
            lines = f.read().splitlines()
        self.transpose_lines = lines
        self.transposer = {int(line) -1 : i for i, line in enumerate(lines)}
        
    def tranpose(self, lines):
        el = Line(r'\0')
        t_lines = [el for _ in range(len(self.transpose_lines))]
        for i, line in enumerate(lines):
            t_lines[self.transposer[i]] = line
        return t_lines

    
class DataFile:
    """Abstraction for data file in directory. Represents either a layer or a macro"""
    def __init__(self, path, transpose = None):
        self.path = path
        self.names = list(self.names_gen())
        self.file_len = len(self.names)
        self.id = int(path.name[1:])
        if transpose:
            self.names = transpose.transpose(self.names)

    def names_gen(self):
        with self.path.open() as f:
            for line in f.readlines():
                try:
                    l = Name(line.strip('\n'))
                except LineError:
                    print('ERROR: File {}, number of quantas does not match trigger'.format(self.path))
                    raise
                else:
                    yield l

    def flat_quantas(self):
        quantas = [str(q) for name in self.names for q in name.quantas]
        return c_wrapper(','.join(quantas))

    def __str__(self):
        return c_wrapper(','.join([str(name) for name in self.names]))
        
    def __len__(self):
        return len(self.names)


class Name:
    
    """A line in a DataFile represents a Name, that is, quantas plus trigger"""
    
    def __init__(self, line):
        self.trigger = 0
        self.txt = line
        self.quantas = []
        self.parse()
        
    def __repr__(self):
        return self.txt
    
    def __len__(self):
        return len(self.quantas)

    def __str__(self):
        s = c_wrapper(','.join([str(q) for q in self.quantas]))
        return '{{{}, {}}}'.format(self.trigger, s)
    
    def parse(self):
        """Turns line into a collection of quantas"""
        line = self.txt.split(' ')
        if len(line) == 1:
            self.quantas.append(Quanta(line[0]))
            self.trigger = lib.trigger('common')
        else:
            trigger = line[1]
            self.trigger = lib.trigger(trigger)
            if lib.line_len(trigger) != len(line[2:]):
                raise LineError(self)
            self.quantas = [Quanta(quanta) for quanta in line[2:]]

class Quanta:

    """Receives string for a 'quanta' (ie keycode, mod, layer and key type combination)
    and translates that into numerical information as used by the code.
    Returns an instance of quanta with individual data acessed as attributes.
    Implements __repr__ for nice formating and __str__ that outputs it all as
    a hex number.
    The following formats are allowed:
    A, Ctrl, SPC (simple key)
    l0, l1, l3 (layer key)
    CSAG__SPC (modifier + key)
    l0__SPC or l0__Ctrl (Layer + Key)
    l0__CSAG__KEY (layer + mod + key).
    Note: To specify the key type the above string must wraped by key_type(key_string)
    eg toggle(l5__Ctrl)"""

    lib = Librarian()
    
    def __init__(self, data):
        self.key_type = 0
        self.layer = 0
        self.mods = 0
        self.keycode = 0
        self.h_type = ""
        self.h_mods = ""
        self.h_keycode = ""
        self.is_layer = lambda element : True if re.match('[lL]\d+', element) else False
        self.parse(data)

    def __str__(self):
        """Return quanta as a hex number"""
        str = '0X{:02X}{:02X}{:02X}{:02X}'
        return str.format(self.key_type, self.layer, self.mods, self.keycode)

    def __repr__(self):
        """Explicit repr in human readable format"""
        rep = 'Type: {}, Layer: L{}, Mods: {}, Key: {}'
        rep += '\n {}'
        return rep.format(self.h_type, self.layer, self.h_mods, self.h_keycode, str(self))
        
    def parse_element(self, element):
        """ Parses any single quanta (ie layer, key or macro)"""
        if self.is_layer(element):
            self.layer = int(''.join(element[1:]))
        elif re.search('[mM]\d+', element):
            value = int(''.join(element[1:]))
            m = self.lib.macros(value)
            self.keycode = m.dim
            self.mods = m.len
        else:
            keycode = self.lib.keycode(element)
            if keycode > 255:
                self.keycode = keycode & 0x00FF
                self.mods = keycode >> 8
                self.h_mods = element
            else:
                self.keycode = keycode
                self.h_keycode = element

    def parse_mod_key(self, data):
        """ Parser for mod + key combination """
        key = self.lib.keycode(data[1])
        mods = 0
        for mod in data[0]:
            mods = mods | self.lib.mod(mod)
        self.mods = mods
        self.keycode = key
        self.h_keycode = data[1]
        self.h_mods = data[0]
    
    def parse(self, data):
        """Identifies the quanta's type and its components leading
        to the parsed quanta."""
        type_regex = r'(\w+)\((.*)\)'
        match = re.search(type_regex, data)
        if match:
            self.key_type = self.lib.quanta(match.group(1))
            self.h_type = match.group(1)
            data = match.group(2)
        else:
            self.key_type = 0
            self.h_type = "normal"

        data = data.split('__') 
        if len(data) == 1:
            self.parse_element(data[0])
        elif len(data) == 2:
            if True in [self.is_layer(element) for element in data]:
                for element in data:
                    self.parse_element(element)
            else:
                self.parse_mod_key(data)
        elif len(data) == 3:
            self.parse_element(data[0])
            self.parse_mod_key(data[1:])
