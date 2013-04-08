#!/bin/sh

# SLIP settings
LOCAL_IP=172.16.0.2
REMOTE_IP=172.16.0.1
NETMASKNO=255.255.0.0
BAUDRATE=115200
SLIP_DEV=/dev/ttyUSB0

#setserial $SLIP_DEV low_latency
slattach -mdv -s $BAUDRATE -p slip $SLIP_DEV &
sleep 1 # allow time for slip device to appear
ifconfig sl0 $LOCAL_IP pointopoint $REMOTE_IP netmask $NETMASKNO up

