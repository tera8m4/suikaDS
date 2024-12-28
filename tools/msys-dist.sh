#!/bin/bash

if [[ ! -x melonDS.exe ]]; then
	echo "Run this script from the directory you built melonDS."
	exit 1
fi

mkdir -p dist

for lib in $(ldd melonDS.exe | grep mingw | sed "s/.*=> //" | sed "s/(.*)//"); do
	cp "${lib}" dist
done

for lib in $(ldd melonDS.exe | grep ucrt64 | sed "s/.*=> //" | sed "s/(.*)//"); do
	cp "${lib}" dist
done

cp melonDS.exe dist
windeployqt dist --no-angle
