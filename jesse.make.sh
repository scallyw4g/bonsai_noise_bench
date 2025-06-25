#! /usr/bin/env bash

OPT="-O2"

./make.sh RunPoof
[ $? -ne 0 ] && exit 1

./make.sh $OPT BuildExecutables
