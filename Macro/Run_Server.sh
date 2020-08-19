#!/bin/bash

source ${QC_SW_PATH}/Macro/TTY_Finder.sh $1

rm -f ${QC_SW_PATH}/Macro/FIFO
${QC_SW_PATH}/Bin/Server $tty $mode

