#!/bin/sh

# SLIP settings
LOCAL_IP=192.168.1.2
REMOTE_IP=172.16.0.1
NETMASKNO=255.255.0.0
BAUDRATE=115200
SLIP_DEV=/dev/ttyUSB0

killall -9 slattach
rmmod slip
modprobe slip
#setserial $SLIP_DEV low_latency
slattach -mdv -s $BAUDRATE -p slip $SLIP_DEV &
sleep 1 # allow time for slip device to appear
ifconfig sl0 $LOCAL_IP pointopoint $REMOTE_IP netmask $NETMASKNO up
route add -net 172.16.0.0 netmask 255.255.0.0 dev sl0
