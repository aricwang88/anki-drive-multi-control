#! /bin/bash

(while cat "$1"; do : Nothing; done) | sudo /home/treenson/Desktop/anki/drive-sdk/build/dist/bin/vehicle-tool --adapter=hci1 --device="$2"
