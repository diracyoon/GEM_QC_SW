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
