#!/bin/bash

SRC_FILE="$1"
SRC_PATTERN="$2"
DST_PATTERN="$3"
DIR="${SRC_FILE%.*}"
SED="sed s/$SRC_PATTERN/$DST_PATTERN/"

set -eu

mkdir "$DIR"
rpmrebuild \
	--batch \
	--change-spec-whole="$SED" \
	--directory "$DIR" \
	--package "$SRC_FILE"

RES_PKG="$(ls $DIR/*/*.rpm)"
BASENAME=$(basename "$RES_PKG")
mv "$RES_PKG" .
rm -r "$DIR"
NEW_NAME=$($SED <<< "$BASENAME")
if [ "$NEW_NAME" != "$BASENAME" ]; then
	mv "$BASENAME" "$NEW_NAME"
fi
if [ "$NEW_NAME" != "$SRC_FILE" ]; then
	rm "$SRC_FILE"
fi
