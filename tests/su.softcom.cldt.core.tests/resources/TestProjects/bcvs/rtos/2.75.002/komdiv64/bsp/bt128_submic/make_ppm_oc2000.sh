#!/bin/sh
today=`date +%m%d%y`

cd ../..

tar chf ppm${BSP}.tar --exclude *~ --exclude booter --exclude boardconfig ./bsp/${BSP}
tar uhf ppm${BSP}.tar ./bsp/${BSP}/boardconfig/osconfig.tcl ./bsp/${BSP}/boardconfig/osconfig.lc.koi8-r ./bsp/${BSP}/boardconfig/osconfig.lc.utf-8
tar uhf ppm${BSP}.tar ./bin/size.sh
tar uhf ppm${BSP}.tar ./bin/crc32.c
tar uhf ppm${BSP}.tar ./lib/bsp${BSP}.a

pushd .
cd ./target/${BSP}
make clean
rm -f .config crc32.c crc32u.c
popd

tar uhf ppm${BSP}.tar ./target/${BSP}/config --exclude *~
#tar uf ppm${BSP}.tar ./target/${BSP}/booter
tar uhf ppm${BSP}.tar ./target/${BSP}/examples --exclude *~
tar uhf ppm${BSP}.tar ./target/${BSP}/configbrd.c ./target/${BSP}/configbrd.h
tar uhf ppm${BSP}.tar ./target/${BSP}/dependbrd.h
tar uhf ppm${BSP}.tar ./target/${BSP}/*.sh --exclude *~
tar uhf ppm${BSP}.tar ./target/${BSP}/makefile
tar uhf ppm${BSP}.tar ./target/${BSP}/maketarget.bsp

gzip -c ppm${BSP}.tar > ppm${BSP}.${today}.tgz

rm ppm${BSP}.tar

