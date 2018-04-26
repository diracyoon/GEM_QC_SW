#!/bin/bash

## Enter the run number
echo -e "Run : \c"
read word1

## Enter the foil name
echo -e "Foil : \c"
read word2

filename=$word1"_"$word2
echo "$filename"

## Copy & modify the template script
cp graph_template.cc graph_$filename.cc
sed -i "s/InputFileName/$filename.result/g" ./graph_$filename.cc
sed -i "s/graph_template/graph_$filename/g" ./graph_$filename.cc

## Start monitoring
root -q graph_$filename.cc
