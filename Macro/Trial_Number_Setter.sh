#!/bin/bash

##set trial number ##
dir_output=${QC_SW_PATH}/Output/${1}/${2}

if [ ! -d $dir_output ];
then
    echo "Make directory for the new foil" 
    mkdir $dir_output
fi

file_count=`ls $dir_output | grep result | wc -l`
trial_number=`expr $file_count`

echo "The allocated trial number=${trial_number}"

#${QC_SW_PATH}/Bin/QC_Long $foilid $trial_number $channel $rh $temp $tester
