#!/bin/sh

ifconfig sl0 down
killall -HUP slattach
rmmod slip
