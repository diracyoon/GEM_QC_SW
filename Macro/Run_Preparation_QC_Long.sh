#!/bin/bash

#check server is running or not
jobname_server=${QC_SW_PATH}/Bin/Server
chk_server=`ps -au | grep $jobname_server | grep -v grep | wc -l`

if [ $chk_server -eq 1 ];
then
    echo "Checked server is running."
else
    echo "Error!! Server is not running. Run server first. Stop the process!!"
    exit
fi
echo

#interactive input argument
echo -n "Please input foil id: "
read foilid

echo -n "Please input channel number: "
read channel

echo -n "Please input relative humidity: "
read rh

echo -n "Please input temperature: "
read temp

echo -n "Please input tester name: "
read tester

echo
echo "Check input variable."
echo -n "Foilid = "
echo -n $foilid
echo -n ", channel number = "
echo -n $channel
echo -n ", relative humidity = "
echo -n $rh
echo -n ", temperature = "
echo $temp;
echo -n "Are they correct? Say y or n "
read chk

if [ $chk == "y" ];
then
    echo "OK, let's proceed."
else
    echo "Error!! Stop the process!!"
    exit
fi
echo

#check channel is owned by other client
chk_channel=`ps -au | grep  ${QC_SW_PATH}/Bin/Preparation_QC_Long | grep -v grep | awk '{print $14}' | grep $channel | wc -l`
if [ $chk_channel -eq 1 ];
then
    pid=`ps -au | grep  ${QC_SW_PATH}/Bin/Preparation_QC_Long | grep -v grep | awk '{if ($14 == '"$channel"') print $2}'`
    echo "Error!! The channel is owned by the process PID $pid!!"
    exit
fi

chk_channel=`ps -au | grep  ${QC_SW_PATH}/Bin/QC_Long | grep -v grep | awk '{print $14}' | grep $channel | wc -l`
if [ $chk_channel -eq 1 ];
then
    pid=`ps -au | grep  ${QC_SW_PATH}/Bin/QC_Long | grep -v grep | awk '{if ($14 == '"$channel"') print $2}'`
    echo "Error!! The channel is owned by the process PID $pid!!"
    exit
fi

echo "Checked the channel ${channel} is free. Let's proceed."
echo

##set runnumber##
dir_output=${QC_SW_PATH}/Output/Preparation_QC_Long
file_count=`ls $dir_output | grep result | wc -l`
runnumber=`expr $file_count + 1`

echo "Assigned runnumber = "${runnumber}
echo

rm -f ${QC_SW_PATH}/Macro/FIFO${channel}
${QC_SW_PATH}/Bin/Preparation_QC_Long $runnumber $foilid $channel $rh $temp $tester
