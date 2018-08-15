#!/usr/bin/zsh
CWD=$(pwd)
TRANSCRIBER="../../../transcriber/transcriber.py"
CHEF="../base-chef"
python "$TRANSCRIBER" --noprog maps > "$CHEF/maps.h"
cd "$CHEF" && make && mv main "$CWD"


