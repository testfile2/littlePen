rmmod main.ko
rm -f /dev/fortytwo
make
insmod main.ko
mknod /dev/fortytwo c 10 57
chmod 777 /dev/fortytwo
echo "Testing read Now: "
cat /dev/fortytwo
echo
echo "Should see kcowle above"
echo "Testing write now"
echo lol > /dev/fortytwo && echo Fail || echo Pass
echo kcowle > /dev/fortytwo && echo Pass || echo Fail
