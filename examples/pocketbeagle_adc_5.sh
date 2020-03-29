#!/bin/sh

#Description:
#This shell script acquires ADC input 5 reading, convert it to a hex-string and 
#sends it to SigFox cloud using sigfox_comm tool.
#
#Board used for this example: Pocket Beaglebone (http://beagleboard.org/pocket)
#
#IMPORTANT: 
#1) Execute this script as a super user
#2) Here it's considered sigfox_comm tool is in the same folder of this script
#3) This script considers SigFox modem is attached to UART /dev/ttyO4
#
#Author: Pedro Bertoleti

#get ADC input 5 reading into a variable
adc_reading="$(sudo cat /sys/bus/iio/devices/iio:device0/in_voltage5_raw)"
echo "ADC reading (decimal): "$adc_reading
adc_reading_hex_string="$(printf '%04x\n' $adc_reading)"
echo "ADC reading (hex-string): "$adc_reading_hex_string

#send the adc reading (formatted to a hex-string) to sigfox cloud 
sigfox_comm_output="$(sudo ./sigfox_comm /dev/ttyO4 $adc_reading_hex_string)"

echo "--- Data sent to SigFox Cloud. Good bye! ---"
echo ""