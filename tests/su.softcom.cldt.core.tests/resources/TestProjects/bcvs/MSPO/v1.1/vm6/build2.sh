#!/bin/bash

ROOT=$(realpath $(dirname $0))
OSBASE=${ROOT}/../../../rtos/2.75.002/komdiv64
BSP=cprio64_submic
cd $1

USER_INCLUDES=(
	-I${ROOT}/../../../1890VM6/inc
)
USER_OBJECTS=(
	${ROOT}/../../../1890VM6/lib1890VM6.o
)
USER_SOURCES=(
	$(find ${ROOT}/src -name "*.c")
	${OSBASE}/bsp/oc2000.c
	${OSBASE}/bsp/${BSP}/cpuLib.S
	${OSBASE}/bsp/${BSP}/atrace.S
)

CC=/usr/baget-tools/H-linux86/bin/bt23h-gcc
if [ "$#" -eq 2 ]
then
	CC=$2 
fi

LD=/usr/baget-tools/H-linux86/bin/bt23h-ld
OBJDUMP=/usr/baget-tools/H-linux86/bin/bt23h-objdump
CFLAGS=(
	-g -O2 -G0 -pipe -nostdlib -mno-fused-madd
	-fshort-wchar -fno-common -fno-builtin-sqrt -fno-strict-aliasing -fno-merge-constants
	-Wall -Wno-pointer-sign -Wno-trigraphs
	-k64rio
	-DCPU=R4000
	-DROM_TEXT_ADRS=0x98300000
	-DRAM_LOW_ADRS=0x80010000
	-DSTARTPOINT=0x80010000
	-DRAM_COMP_ADRS
	-DCOMPRESS
	-DK64SMP
	-DK64RIO
	-DCPRIO64
	-DSUBMICRON
	-DFOR_SUBMICRON
	-DNAME_MAX=80
	-DCLOCK_MAX=4
	${USER_INCLUDES[@]}
	-I${OSBASE}/bsp
	-I${OSBASE}/bsp/${BSP}
	-I${OSBASE}/bsp/${BSP}/include
	-I${OSBASE}/bsp/${BSP}/lib/include
	-I${OSBASE}/include
	-I${OSBASE}/include/opt
	-I${OSBASE}/locale
	-I${OSBASE}/oc-include
	-I${ROOT}
)


IMAGE_DATE=-DIMAGE_DATE="\"$(LC_TIME="en" date)\""

for SRC in ${USER_SOURCES[@]} ; do
	${CC} ${CFLAGS[@]} "${IMAGE_DATE}" -o $(basename ${SRC%.*}).o -c ${SRC}
done

${LD} -r -o tmp *.o ${USER_OBJECTS[@]} \
	-'(' ${OSBASE}/lib/bsp${BSP}.a ${OSBASE}/lib/oc2000komdiv64.a ${OSBASE}/lib/libmK64.a -')'

${OBJDUMP} --syms tmp > out
cp ${OSBASE}/lib/oooR4000.def out1
${OSBASE}/bin/elfhR4000 tmp >> out1
gawk -f ${OSBASE}/lib/def.awk out1 out | sort | gawk -f ${OSBASE}/lib/sort.awk -v cpu=R4000 > sym_tbl.s

${CC} ${CFLAGS[@]} -D_ASM -x assembler-with-cpp -c -o cpuinit.o ${OSBASE}/bsp/${BSP}/cpuinit.s
${CC} ${CFLAGS[@]} -D_ASM -x assembler-with-cpp -c -o sym_tbl.o sym_tbl.s

${CC} -nostartfiles -N -fno-common -fshort-wchar -e cpuInit -Xlinker -Ttext -Xlinker 0x80010000 \
	-o oc2000 cpuinit.o ${OSBASE}/lib/setdef0R4000.o tmp ${OSBASE}/lib/setdef1R4000.o sym_tbl.o -lgcc

rm -f *.o tmp out out1 sym_tbl.s
