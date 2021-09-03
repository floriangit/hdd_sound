## 1990s again! Play mechanical HDD sounds in Linux linked to real disk activity.

REQUIREMENTS
------------
* libpulse-dev
* In Ubuntu just:
`sudo apt install libpulse-dev`

BUILD
-----
`make`

USAGE
-----
* Find your partition which HDD activities shall be soundified: `mount | grep "\ /\ "` to see which is your root (/) partition
* `ls /sys/block/`

* start the executable as a daemon, e.g.:
`./hdd_sound sda & # to soundify partition sda and start this as background task`

