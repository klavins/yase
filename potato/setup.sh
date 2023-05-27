#!/bin/bash
cd /home/klavins/libretech-overlays
cp -r overlays/*.dtbo /lib/firmware
./overlay.sh add libretech-cc-i2c-ao
./overlay.sh add libretech-cc-i2c-b
