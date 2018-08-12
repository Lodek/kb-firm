import re
from collections import namedtuple

class Librarian:

    """Librarian is responsible for information lookup such as: HID Keycodes,
    modifiers code, triggers, expected quantas after trigger and etc.
    Librarian contains a macro dictionary which allows it to generate
    the correct quanta for the given macro. """

    def __init__(self):
        self.triggers_dict = globals()['triggers']
        self.triggers_len = globals()['triggers_len']
        self.mods_dict = globals()['mods_dict']
        self.quanta_types = globals()['quanta_types']
        self.hid_keycodes = globals()['hid_keycodes']
        self.macros_dict = {}

        self.keycode = lambda keycode : self.returner_base(keycode, self.hid_keycodes)
        self.mod = lambda m : self.returner_base(m, self.mods_dict)
        self.line_len = lambda n : self.triggers_len[n]
        self.trigger = lambda t : self.triggers_dict[t]
        self.quanta = lambda q : self.quanta_types[q]
        self.macros = lambda m : self.macros_dict[m]

    def returner_base(self, key, dict):
        """DRY for lookup functions"""
        try:
            value = dict[key]
        except KeyError:
            value = dict[key.lower()]
        return value

    def update_macros(self, root_p, macros):
        """Generates the Macro Dict. It relates a macro number with its
        dimmension in the Data matrix"""
        layers_len = len([p for p in root_p.iterdir() if re.search('[lL]\d+', p.name)])
        MacroInfo = namedtuple('MacroInfo', ['dim', 'len'])
        d = {macro.id : MacroInfo(i+layers_len, len(macro)) for i, macro in enumerate(macros)}
        self.macros_dict = d


triggers = {'common' : 0, 'hold' : 1, 'doubletap' : 2, 'dth' : 3}
triggers_len = {'common' : 1, 'hold' : 2, 'doubletap' : 2, 'dth' : 3}
quanta_types = {'normal' : 0, 'toggle' : 1, 'macro' : 2, 'dead' : 3, 'absolute' : 4}
mods_dict = {'c' : 0x01, 's': 0x02, 'a':0x04, 'g':0x08}
hid_keycodes = {
    'right': 79,
    '|': 8241,
    'home': 74,
    '[': 47,
    'f12': 69,
    '_': 8237,
    'f1': 58,
    ';': 51,
    'gui': 2048,
    '0': 39,
    '*': 8229,
    'f6': 63,
    'ctrl': 256,
    'E': 8200,
    'v': 25,
    '+': 8238,
    's': 22,
    '.': 55,
    'R': 8213,
    'q': 20,
    'W': 8218,
    'L': 8207,
    '{': 8239,
    'f9': 66,
    'r': 21,
    '`': 53,
    'b': 5,
    'tab': 43,
    'e': 8,
    'Q': 8212,
    '/': 56,
    '&': 8228,
    'a': 4,
    'pagedown': 78,
    'esc': 41,
    '7': 36,
    '}': 8240,
    'f8': 65,
    '5': 34,
    'F': 8201,
    'o': 18,
    'enter': 40,
    ',': 54,
    'M': 8208,
    'left': 80,
    'end': 77,
    'X': 8219,
    '#': 8224,
    'up': 82,
    'P': 8211,
    'del': 76,
    'l': 15,
    'shift': 512,
    'f': 9,
    't': 23,
    '$': 8225,
    'G': 8202,
    ':': 8243,
    'pageup': 75,
    '8': 37,
    '3': 32,
    'backspc': 42,
    'x': 27,
    '2': 31,
    '-': 45,
    'minus': 45,
    'f3': 60,
    'z': 29,
    '>': 8247,
    'A': 8196,
    'printscreen': 70,
    'print': 70,
    '4': 33,
    'caps': 57,
    '?': 8248,
    'h': 11,
    'N': 8209,
    'd': 7,
    'c': 6,
    'scrolllock': 71,
    'scroll': 71,
    'pause': 72,
    'T': 8215,
    '(': 8230,
    'J': 8205,
    'D': 8199,
    '!': 8222,
    'spc': 44,
    'O': 8210,
    'alt': 1024,
    '1': 30,
    'Z': 8221,
    'f5': 62,
    'u': 24,
    'j': 13,
    '%': 8226,
    'f11': 68,
    '<': 8246,
    '6': 35,
    'H': 8203,
    'i': 12,
    '\\': 49,
    'U': 8216,
    'insert': 73,
    'ins': 73,
    'f7': 64,
    'p': 19,
    '@': 8223,
    '9': 38,
    'w': 26,
    'C': 8198,
    'f4': 61,
    ')': 8231,
    '~': 8245,
    'f2': 59,
    ']': 48,
    'V': 8217,
    'f10': 67,
    'I': 8204,
    'm': 16,
    'B': 8197,
    '"': 8244,
    '=': 46,
    'down': 81,
    '^': 8227,
    'S': 8214,
    'K': 8206,
    'g': 10,
    'Y': 8220,
    "'": 52,
    'y': 28,
    'n': 17,
    'k': 14,
    '\\0' : 0,
    'iso_tilde' : 50,
    'grave' : 53,
    'iso_\\' : 100,
    'rctrl' : 4096,
    'rshift' : 8192,
    'ralt' : 16384,
    'rgui' : 32768,
    'br?' : 135,
}
