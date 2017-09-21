#!/bin/sh
#set -x

module="main"
device="main"
mode="666"

# invoke insmod and tell it where the globals live
/sbin/insmod $module

minor=`cat /proc/misc | awk "\\$2==\"$module\" {print \\$1}"`

rm -f /dev/${device}
mknod /dev/${device} c 10 $minor
chmod $mode  /dev/${device}

