#!/bin/bash

vendor_id=21e1
device_id=3

if [ $1 == "NORMAL" ];
then
    for num in {0..9}
    do
	res=`grep PRODUCT /sys/class/tty/ttyACM${num}/../../uevent`
	
	if [[ $res = *${vendor_id}/${device_id}* ]];
	then
	    tty=ttyACM${num}
	    break
	fi
	
    done
    echo $tty
else
    tty="test_port"
fi

rm -f ${QC_SW_PATH}/Macro/FIFO
${QC_SW_PATH}/Bin/Server $tty $1

