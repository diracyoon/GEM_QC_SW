#!/bin/bash

#check channel is owned by other client
chk_channel=`ps -au | grep  ${QC_SW_PATH}/Bin/Preparation_QC_Long | grep -v grep | awk '{print $14}' | grep $1 | wc -l`
if [ $chk_channel -eq 1 ];
then
    pid=`ps -au | grep  ${QC_SW_PATH}/Bin/Preparation_QC_Long | grep -v grep | awk '{if ($14 == '"$1"') print $2}'`
    echo "Error!! The channel is owned by the process PID $pid!!"
    exit
fi

chk_channel=`ps -au | grep  ${QC_SW_PATH}/Bin/QC_Long | grep -v grep | awk '{print $14}' | grep $1 | wc -l`
if [ $chk_channel -eq 1 ];
then
    pid=`ps -au | grep  ${QC_SW_PATH}/Bin/QC_Long | grep -v grep | awk '{if ($14 == '"$1"') print $2}'`
    echo "Error!! The channel is owned by the process PID $pid!!"
    exit
fi
