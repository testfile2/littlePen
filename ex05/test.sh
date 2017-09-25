#!/bin/sh

echo "Making the module"
make && echo "loading the module into the kernel"
insmod main.ko
echo "making /dev/fortytwo"
NUM="$(cat /proc/misc | cut -c 1-3)"
mknod /dev/fortytwo c 10 "${NUM}"
echo "Testing Write 1: output should be invalid"
echo "lol" > /dev/fortytwo 
echo "\n\nTesting Write 2: output should be nothing"
echo "kcowle" > /dev/fortytwo 
echo "Testing Read: output should be kcowle"
cat /dev/fortytwo 
echo "\n\nCleaning up..."
echo "Removing /dev/fortytwo"
rm -rf /dev/fortytwo
echo "make cleaning"
make clean && echo "rmmoding the module"
rmmod main.ko
