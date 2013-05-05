# stop remaining openocd instances
killall -9 openocd
# start new openocd and reset and init chip
/media/data/openocd-git/openocd/bin/openocd -f board/ek-lm4f120xl.cfg -c init -c"reset init" -c"poll"
