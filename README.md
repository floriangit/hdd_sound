## 1990s again! Play mechanical HDD sounds in Linux linked to real disk activity.


https://github.com/floriangit/hdd_sound/assets/90066055/e971762e-cdfb-4f43-8119-9067709cedc5



REQUIREMENTS
------------
* libpulse-dev
* In Ubuntu just:
`sudo apt install libpulse-dev`

BUILD
-----
`make`

USAGEffmpeg -i sample.avi -q:a 0 -map a sample.mp3
-----
* Find your partition which HDD activities shall be soundified: `mount | grep "\ /\ "` to see which is your root (/) partition
* `ls /sys/block/`

* start the executable as a daemon, e.g.:
`./hdd_sound sda & # to soundify partition sda and start this as background task`

