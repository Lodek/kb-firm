#!/usr/bin/python
from pathlib import Path
from librarian import Librarian
from model import Quanta, DataFile, Transpose, Definer
import argparse, logging, re

def parse_args():
    parser = argparse.ArgumentParser(description='Generates data.h for Keyboard module')
    parser.add_argument('directory', help='Directory containing layer and macro files')
    parser.add_argument('-t', '--transpose', help='Tranpose file')
    args = parser.parse_args()
    return args

def main():
    args = parse_args()
    root_p = Path(args.directory)
    transpose = Transpose(args.tranpose) if args.transpose else None
    macros = [DataFile(p) for p in root_p.iterdir() if re.search('[mM]\d+', p.name)]
    Quanta.lib.update_macros(macros)
    layers = [DataFile(p, transpose) for p in root_p.iterdir() if re.search('[lL]\d+', p.name)]
    definer = Definer(layers, macros)
    definer.define_all()

    
if __name__ == '__main__':
    main()
