#! /bin/bash

make

echo ''

echo 'Copying libVB.so to /usr/lib, need permission...'

sudo cp ./libVB.so /usr/lib

exit 0