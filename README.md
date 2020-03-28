#sigfox_comm tool - official repository

##Overview
This is the official repository of sigfox_comm toll, a toll (for Linux / embedded Linux) to communicate to WSSFM11R2D SigFox modem, in order to make it possible to send data from your Linux / Embedded Linux system to SigFox Cloud.


##sigfox_comm usage

First of all, ensure your WSSFM11R2D SigFox modem is attached to one of the UART or USB-UART ports of your Linux / Embedded Linux system (example: /dev/ttyO4 or /dev/ttyUSB0). 

**Waning**: WSSFM11R2D SigFox modem maximum allowed voltage level is 3.3V. DO NOT USE 5V VOLTAGE LEVEL as power supply and/or UART communication

Then, clone this repository to your Linux / Embedded Linux system, go to source_code sub-folder and compile it using the following command:


```
make
```

It can take some seconds to compile. Then, run sigfox_comm tool as following, replacing **UART_FILE_PATH** for UART or USB-UART port WSSFM11R2D SigFox modem is attached in and replacing **DATA_ARRAY_TO_SEND** for the Hex-String data to be sent to SigFox Cloud (for example: 010203). You'll probably need to run this tool as super user, otherwise you won't be able to open write or read data to/from UART.


```
sudo sigfox_comm UART_FILE_PATH DATA_ARRAY_TO_SEND
```

And that's it! Some seconds after this command execution, your data (DATA_ARRAY_TO_SEND) will be at SigFox Cloud.


**Important:** SigFox allows 12 bytes as maximum hex (raw) data size to be sent. As this tool uses a hex-string to represent the data to be sent to SigFox Cloud, it should have 24 bytes as maximum size. If a longer hex-string is passed as argument to the tool, it'll be automatically truncated to a 24 bytes hex-string.


##Where can I run this tool?

In any Linux System / embedded Linux system you want to, as long as you can attach WSSFM11R2D SigFox modem to a available UART or USB-UART port.


##Tests executed

This tool has been tested in the following Linux / Embedded Linux systems:

* Generic notebook running Ubuntu 16.04LTS Linux Operational System, using a 3.3V USB-Serial converter
* Pocket Beaglebone board running Debian GNU/Linux 9 (stretch) Linux Operational System


