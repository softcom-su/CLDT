#!/bin/sh
BCVSBASE=/mnt/c/Users/victo/Desktop/bcvs
BCVSCPU=1890VM6
BASE=$BCVSBASE/$BCVSCPU

rm -fR $BASE
mkdir -p $BASE
cp -R ../$BCVSCPU $BCVSBASE
rm -f $BASE/install.sh $BASE/makefile $BASE/uninstall.sh


##RR=1
##echo $RR
##RR=$(($RR+1))
##echo $RR


