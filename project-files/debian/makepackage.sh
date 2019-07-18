#!/bin/bash

DEB_NAME="embroidermodder_2.0.0-1_amd64.deb"

echo "Copying built binary from ../../embroidermodder2"
mkdir -p usr/share/embroidermodder2
cp -r ../../embroidermodder2/{embroidermodder2,commands,help,icons,images,samples,translations} usr/share/embroidermodder2/

echo "Calculating installed size"
INSTALL_SIZE=$(du -s --block-size 1024 . | sed "s/\s.*//")
sed -i "s/Installed-Size: 6000/Installed-Size: $INSTALL_SIZE/" DEBIAN/control

echo "Building .deb file"
dpkg-deb -b . "$DEB_NAME"

echo "Done: $DEB_NAME"
