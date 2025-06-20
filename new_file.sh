#!/usr/bin/bash

SOURCE=src
INCLUDE=$SOURCE/include

C_FILE=$(echo $1).c
H_FILE=$(echo $1).h
DEFINE=$(echo $1 | tr a-z A-Z)_H

echo -e "source: $C_FILE\nheader: $H_FILE\ndefine: $DEFINE"

echo -e "#ifndef $DEFINE\n#define $DEFINE\n#endif" > $INCLUDE/$H_FILE
echo -e "#include \"$H_FILE\"" > $SOURCE/$C_FILE
