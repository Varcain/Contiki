# stop remaining openocd instances
killall -9 openocd
# start new openocd and reset and init chip
/media/data/openocd-git/openocd/bin/openocd -f board/stm32f4discovery.cfg -c init -c"reset init" -c"poll"
