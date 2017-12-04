#!/bin/bash

SRC_FILE="$1"
SRC_PATTERN="$2"
DST_PATTERN="$3"
DIR="${SRC_FILE%.*}"
DEBDIR="$DIR/DEBIAN"
CONTROL="$DEBDIR/control"
RES_PKG="./tmp.deb"

set -eu

# modify metadata
dpkg-deb -x "$SRC_FILE" "$DIR"
dpkg-deb -e "$SRC_FILE" "$DEBDIR"
sed -i "s/$SRC_PATTERN/$DST_PATTERN/" "$CONTROL"
dpkg-deb -b "$DIR" "$RES_PKG"

dpkg-name "$RES_PKG"
# we're assuming that the original file had a different name than the new file
rm -r "$SRC_FILE" "$DIR"
