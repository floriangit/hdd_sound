1990s again! Play mechanical HDD sounds in Linux linked to real disk activity.

REQUIREMENTS
------------
libpulse-dev
In Ubuntu just:
# sudo apt install libpulse-dev

BUILD
-----
# make

USAGE
-----
1) find your partition which HDD activities shall be soundified
# mount | grep "\ /\ " to see which is your root (/) partition
# ls /sys/block/

2) start the utility!
# ./hdd_sound sda & # to soundify partition sda and start this as background task

