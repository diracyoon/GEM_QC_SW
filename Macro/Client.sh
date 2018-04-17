#!/bin/bash

rm -f ${QC_SW_PATH}/Macro/FIFO${3}
${QC_SW_PATH}/Code/Client/Client $1 $2 $3 $4
