#!/bin/bash

tty=`ls -l /sys/bus/usb-serial/devices | awk '{print $9}'`
echo $tty

rm -f ${QC_SW_PATH}/Macro/FIFO
${QC_SW_PATH}/Code/Server/Server $tty

