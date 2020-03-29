#!/bin/sh

#Description:
#This shell script acquires CPU temperature reading, convert it to a hex-string and
#sends it to SigFox cloud using sigfox_comm tool.
#
#Board used for this example: Raspberry Pi Zero W (https://www.raspberrypi.org/blog/raspberry-pi-zero-w-joins-family/)
#
#IMPORTANT:
#1) Execute this script as a super user
#2) Here it's considered sigfox_comm tool is in the same folder of this script
#3) This script considers SigFox modem is attached to a USB-UART converter in /dev/ttyUSB0
#
#Author: Pedro Bertoleti

#get CPU temperature reading into a variable
cpu_temp_reading="$(cat /sys/class/thermal/thermal_zone0/temp)"
echo "CPU temperature reading (decimal): "$cpu_temp_reading
cpu_temp_reading_hex_string="$(printf '%04x\n' $cpu_temp_reading)"
echo "CPU temperature reading (hex-string): "$cpu_temp_reading_hex_string

#send CPU temperature reading (formatted to a hex-string) to sigfox cloud
sigfox_comm_output="$(sudo ./sigfox_comm /dev/ttyUSB0 $cpu_temp_reading_hex_string)"

echo "--- Data sent to SigFox Cloud. Good bye! ---"
echo ""
