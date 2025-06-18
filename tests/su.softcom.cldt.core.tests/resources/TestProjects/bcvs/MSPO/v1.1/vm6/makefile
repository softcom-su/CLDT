BSP		:= cprio64_submic
CPU		:= R4000
ARCH		= komdiv64
CPU_TUNE	:= K64SMP
pref		:= bt23h-
OSBASE	:= /home/osuser/SubMicL/bcvs/rtos/2.75.002/komdiv64
ROM_TEXT_ADRS	:= 0x98300000		# flash resident image .text base
#RAM_COMP_ADRS	:= 0xa0e00000		# uncompress image .text base
include maketarget.bsp
CC_DEFINES += -DK64SMP -DK64RIO -DCPRIO64 -k64rio -DSUBMICRON -DFOR_SUBMICRON\
        -fno-builtin-sqrt -mno-fused-madd -Wno-pointer-sign\
        -DIMAGE_DATE="\"$(shell LC_TIME="en" date)\"" 
