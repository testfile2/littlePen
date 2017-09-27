mount -t debugfs none /sys/kernel/debug/
make
insmod main.ko
chown 444 /sys/kernel/debug/fortytwo/
echo "Testing ID";
echo "Testing write: with bad output";
echo "lol" > /sys/kernel/debug/fortytwo/id
echo "Testing write: with good output"
echo "kcowle" > /sys/kernel/debug/fortytwo/id
echo "testing read"
cat /sys/kernel/debug/fortytwo/id
printf "\n"
echo "--------------------------------------------------------------------------"
echo "Testing jiffies";
cat /sys/kernel/debug/fortytwo/jiffies
printf "\n"
echo "--------------------------------------------------------------------------"
echo "Testing foo";
cat /sys/kernel/debug/fortytwo/foo
echo "kcowle" > /sys/kernel/debug/fortytwo/foo
cat /sys/kernel/debug/fortytwo/foo
echo "This is the second test" > /sys/kernel/debug/fortytwo/foo
cat /sys/kernel/debug/fortytwo/foo
