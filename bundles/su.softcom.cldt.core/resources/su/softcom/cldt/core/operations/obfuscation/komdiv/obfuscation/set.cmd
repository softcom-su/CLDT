@echo off
set CLANG=%3%
set Path=%PATH%%CLANG%\bin;%CLANG%libs;

:: set LIB=F:\VS\17\lib\x64;F:\VS\17\lib\x64\store;F:\VS\WSDK_10.0.18362.1\lib_um\x64;F:\VS\WSDK_10.0.18362.1\lib_ucrt\x64;F:\VS\WSDK_10.0.18362.1\lib_ucrt_enclave\x64;C:\LLVM\libs;

:: set LIBPATH=%LIB%
set LIBPATH=

set I0=d:\Work\bcvs-vm6-build-with-obfuscation
set I1=..\include
::set I1=%I0%\MSPO\v1.0\vm6\src
set I2=%I0%\1890VM6\inc
set I3=%I0%\rtos\2.75.002\komdiv64\include
set I4=%I0%\rtos\2.75.002\komdiv64\include\sys
set I5=%I0%\rtos\2.75.002\komdiv64\bsp\cprio64_submic\include
set I6=%I0%\rtos\2.75.002\komdiv64\bsp\cprio64_submic\lib\include

set INCLUDE=%I2%;%I3%;%I4%;%I5%;%I6%;

set K=--target=mips64-unknown-gnu -march=mips3 -mabi=n32 -w
set D=-D__R3000__
set P=%1%
set A=komdiv
set M=komdiv3
set O=-O3
set N=all
set FLE=c3

set PYTHON=%2%
