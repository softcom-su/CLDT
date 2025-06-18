@echo off
set F1=%1%
set PathToSourceFile=%2%
set Project=%3%
clang %PathToSourceFile% -O2 -pipe -nostdlib -fshort-wchar -fno-common -fno-builtin-sqrt -fno-strict-aliasing -fno-merge-constants -Wall -Wno-pointer-sign -Wno-trigraphs  -I%Project%/MSPO/v1.1/vm6/../../../1890VM6/inc -I%Project%/MSPO/v1.1/vm6 -I%Project%/plug/../rtos/2.75.002/komdiv64/bsp/cprio64_submic/lib/include -I%Project%/plug/../rtos/2.75.002/komdiv64/oc-include -I%Project%/plug/../rtos/2.75.002/komdiv64/include -I%Project%/plug/../rtos/2.75.002/komdiv64/include/opt -I%Project%/plug/../rtos/2.75.002/komdiv64/bsp -I%Project%/plug/../rtos/2.75.002/komdiv64/bsp/cprio64_submic -I%Project%/plug/../rtos/2.75.002/komdiv64/bsp/cprio64_submic/include -I%Project%/plug/../rtos/2.75.002/komdiv64/locale -DCPU=R4000 -DROM_TEXT_ADRS=0x98300000 -DRAM_LOW_ADRS=0x80010000 -DSTARTPOINT=0x80010000 -DRAM_COMP_ADRS -DCOMPRESS -DK64SMP -DK64RIO -DCPRIO64 -DSUBMICRON -DFOR_SUBMICRON -DNAME_MAX=80 -DCLOCK_MAX=4 -DSIGQUEUE_MAX=32 --target=mips64-unknown-gnu -nostdlib -emit-llvm -S -march=mips3 -mabi=n32 -D__R3000__ -DKOMDIV 
llc -relocation-model=static -filetype=asm -mattr=noabicalls %F1%.ll -o %F1%.0.S
%PYTHON% %P%\s2f14.py %F1%.0.S %F1%.f.json
%PYTHON% %P%\f2l.py %F1%.f.json %F1%.l.json
@echo on
