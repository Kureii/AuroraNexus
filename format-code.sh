#!/bin/bash

# Nalezení všech .cpp a .hpp souborů a spuštění clang-format
find . -iname *.cpp -o -iname *.hpp | xargs clang-format -i

# Potvrzení dokončení
echo "Kód byl naformátován dle .clang-format"
