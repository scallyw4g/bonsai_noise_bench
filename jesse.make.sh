#! /usr/bin/env bash

OPT="-O2"

# ./make.sh RunPoof
# [ $? -ne 0 ] && exit 1

./make.sh $OPT BuildFastNoise2
# ./make.sh $OPT BuildAll
