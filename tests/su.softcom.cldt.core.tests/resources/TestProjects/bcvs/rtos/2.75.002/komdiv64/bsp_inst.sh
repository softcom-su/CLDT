#!/bin/sh
#
#    BSP installation script for OC PB's izd 2,3,4,5,6
#    Must be executed either at OC PB  home directory (such as /usr/oc2000)
#              or in any location with additional argument - path to OSBASE
#
#    
#    Example: sh bsp_inst.sh ppm23202.xxyyzz.tgz [directory]


if [ "$1" = "" ] ; then
	echo -e "You must specify BSP archive name!!!\n\r"
	exit 1
fi

if [ -z $2 ]; then
     printf "\n\rInstalling in current directory %s\n\r" `pwd`
else
     if [ -d $2 ]; then
         printf "\n\rInstalling in %s\n" $2
	 cp $1 $2
	 cd $2
     else
         printf "%s don't exist !\n" $2
	 exit 1
     fi
fi

OSBASE=`pwd`

function fbsp () { BSP=$3 ; }
fbsp `tar -tzf $1 | grep  './bsp/' | sed y=/=\ =`
if [ ! "$BSP" ]; then
	echo -e "Wrong BSP archive name!!!\n\r"
	exit 1
fi
printf "\n\rBSP=%s\n\r" $BSP

#------------------------------------------------
if [ -d bsp/$BSP ]; then
  printf "Saving old  %s/bsp/%s in %s/bsp/%s\n" $OSBASE $BSP $OSBASE old$BSP
  if [ -d bsp/old$BSP ]; then 
    printf "     Replacing old copy %s/bsp/%s \n" $OSBASE old$BSP
  else
    mkdir bsp/old$BSP
  fi
  cp -R bsp/$BSP/* bsp/old$BSP
  printf "Removing old %s/bsp/%s \n" $OSBASE $BSP
  rm -f -R bsp/$BSP
fi  

#---------------------------------------------------------
printf "\nUncompressing %s and extracting BSP's files...\n" $1
tar xzf $1
if [ $? != 0 ] ; then
	printf "Untaring files from %s failed!!!\n\r" $1
	exit 1
fi
#--------------------------------------------------------
# extraction version/release/beta value from string like this
#             #define OS_VERSION_MINOR 50
function fver () { v=$3 ; }   # returns 3rd element of list
#---------------- Check OS version  -------------------
cd $OSBASE/include
if [ -e private/version.h  ]; then
  ver=private/version.h
elif [ -e version.h  ]; then
  ver=version.h
else
  printf "No file %s/include/version.h or %s/include/private/version.h  !!!\n\r" $OSBASE $OSBASE
  exit 0
fi

fver `cat -v $ver | sed y/^/" "/ | grep -F OS_MAJOR`
VER_MAJ=$v
if [ $v != 2 ]; then
    printf "Wrong version of OC PB !!!\n\r"
    exit 0
fi

fver `cat -v $ver | sed y/^/" "/ | grep -F OS_MINOR`
VER_MIN=$v

if [ $v -lt 18 ]; then
   printf "Wrong version of OC PB !!!\n\r"
   exit 0
fi

fver `cat -v $ver | sed y/^/" "/ | grep -F OS_RELEASE`
VER_RLS=$v

if [ $VER_MIN -lt 10 ]; then                 # we have to show 2 as 02 for OS_MINOR
  OS_VERSION=${VER_MAJ}0${VER_MIN} ;
else
  OS_VERSION=${VER_MAJ}${VER_MIN} ;
fi
export OS_VERSION

#----------------Set PREF  -----------
cd $OSBASE
#PREF=bt23d-
if [ $OS_VERSION -ge 273 ]; then
  if [  $OS_VERSION -gt 273 ]; then
     PREF=bt23h-
  else
    if [ $VER_RLS -ge 13 ]; then
       PREF=bt23h-
    else
       PREF=bt23f-
    fi
  fi
else
  if [ $OS_VERSION -ge 270 ]; then
    PREF=bt23f-
  else
    if [ $OS_VERSION -ge 240 ]; then
      PREF=bt23d-
    else
      PREF=bt23c- 
    fi
  fi
fi
printf "\nSetting right prefix for GNU tools: %s \n" $PREF
#---------------------------------------------------------
#if [ `2>&1 bt23c-gcc -v | grep -c -e version` -ne 0 ]; then
#  PREFREC=bt23c-
#else
#  PREFREC=bt23b-
#fi
#printf "\nSetting right objcopy prefix for making rec-file: %s \n" $PREFREC

#---------------------------------------------------------
printf "\nUpdating makefile's\n"
cd $OSBASE/target/$BSP

list=`find ./ -name makefile`
for i in $list; do
	dn=`dirname $i`
	gawk -f  $OSBASE/bsp/$BSP/i.awk -v ocbase=$OSBASE -v pref=$PREF -v dn=$dn -v mp=""  $i 	
	mv $dn/makefile-1 $i
done

cd $OSBASE
if [ -d ./target/$BSPb ]; then
  cd $OSBASE/target/$BSPb

  list=`find ./ -name makefile`
  for i in $list; do
	dn=`dirname $i`
	gawk -f  $OSBASE/bsp/$BSP/i.awk -v ocbase=$OSBASE -v pref=$PREF -v dn=$dn -v mp="" $i 	
	mv $dn/makefile-1 $i
  done
fi

cd $OSBASE
if [ -d ./target/${BSP}-01 ]; then           # bt83b
  cd $OSBASE/target/$BSP-01

  list=`find ./ -name makefile`
  for i in $list; do
	dn=`dirname $i`
	gawk -f  $OSBASE/bsp/$BSP/i.awk -v ocbase=$OSBASE -v pref=$PREF -v dn=$dn -v mp="" $i 	
	mv $dn/makefile-1 $i
  done
fi

cd $OSBASE
if [ -d ./target/$BSP-02 ]; then           # bt83b
  cd $OSBASE/target/$BSP-02

  list=`find ./ -name makefile`
  for i in $list; do
	dn=`dirname $i`
	gawk -f  $OSBASE/bsp/$BSP/i.awk -v ocbase=$OSBASE -v pref=$PREF -v dn=$dn -v mp="" $i 	
	mv $dn/makefile-1 $i
  done
fi

if [ $BSP != "x86" ]; then
  #---------------------------------------------------------
  printf "\nMaking some bin's\n"
  cd $OSBASE/bin
  rm -f crc32
  make crc32
fi
#------------------------ creating in TARGET sys_version file
printf "\nMaking file sys_version in TARGET\n"
cd $OSBASE/bsp/$BSP/include
if [ -f bspversion.h ]; then
  fver `cat -v bspversion.h | sed y/^/" "/ | grep -F BSP_VERSION_BETA`
  vbeta=$v
  fver `cat -v bspversion.h | sed y/^/" "/ | grep -F BSP_VERSION_MAJOR`
  v1=$v;
  fver `cat -v bspversion.h | sed y/^/" "/ | grep -F BSP_VERSION_MINOR`
else
  fver `cat -v board.h | sed y/^/" "/ | grep -F BSP_VERSION_MAJOR`
  v1=$v;
  fver `cat -v board.h | sed y/^/" "/ | grep -F BSP_VERSION_MINOR`
fi
printf "BSP   version %i." $v1 > $OSBASE/target/$BSP/sys_version
printf "%i" $v >> $OSBASE/target/$BSP/sys_version
if [ $vbeta == 1 ]; then
  printf "beta" $vbeta >> $OSBASE/target/$BSP/sys_version
fi
printf "\n" >> $OSBASE/target/$BSP/sys_version
cd $OSBASE

#---------------- OS version to sys_version ----------------------
cd $OSBASE/include
fver `cat -v $ver | sed y/^/" "/ | grep -F OS_MAJOR`
printf "OC PB version %i." $v >> $OSBASE/target/$BSP/sys_version
fver `cat -v $ver | sed y/^/" "/ | grep -F OS_MINOR`
printf "%02i" $v >> $OSBASE/target/$BSP/sys_version
izd=$v

fver `cat -v $ver | sed y/^/" "/ | grep -F OS_RELEASE`
printf ".%02i\n" $v >> $OSBASE/target/$BSP/sys_version

printf "Selecting proper library\n\r"

cd $OSBASE

if [ $izd -gt 70 ]; then
  if [ -f lib/bsp${BSP}.a_izd_6 ]; then
    mv lib/bsp${BSP}.a_izd_6 lib/bsp${BSP}.a
    if [ -f lib/bsp${BSP}.a_izd_3 ]; then rm lib/bsp${BSP}.a_izd_3 ; fi
    if [ -f lib/bsp${BSP}.a_izd_4 ]; then rm lib/bsp${BSP}.a_izd_4 ; fi
    if [ -f lib/bsp${BSP}.a_izd_5 ]; then rm lib/bsp${BSP}.a_izd_5 ; fi
  else
    if [ -f lib/bsp${BSP}.a ]; then printf "Only one lib presented, so taking it !\n\r"
    else
       printf "No lib for OC izd. 6 !!!\n\r"
       exit 
    fi
  fi
else
  if [ $izd -gt 60 ]; then
    if [ -f lib/bsp${BSP}.a_izd_5 ]; then
      mv lib/bsp${BSP}.a_izd_5 lib/bsp${BSP}.a
      if [ -f lib/bsp${BSP}.a_izd_3 ]; then rm lib/bsp${BSP}.a_izd_3 ; fi
      if [ -f lib/bsp${BSP}.a_izd_4 ]; then rm lib/bsp${BSP}.a_izd_4 ; fi
      if [ -f lib/bsp${BSP}.a_izd_6 ]; then rm lib/bsp${BSP}.a_izd_6 ; fi
    else
      printf "No lib for OC izd. 5 !!!\n\r"
      exit 
    fi
  else
    if [ $izd -gt 50 ]; then
      if [ -f lib/bsp${BSP}.a_izd_4 ]; then
        mv lib/bsp${BSP}.a_izd_4 lib/bsp${BSP}.a
        if [ -f lib/bsp${BSP}.a_izd_3 ]; then rm lib/bsp${BSP}.a_izd_3 ; fi
        if [ -f lib/bsp${BSP}.a_izd_5 ]; then rm lib/bsp${BSP}.a_izd_5 ; fi
        if [ -f lib/bsp${BSP}.a_izd_6 ]; then rm lib/bsp${BSP}.a_izd_6 ; fi
      else
        printf "No lib for OC izd. 4 !!!\n\r"
        exit
      fi
    else
      if [ $izd -gt 18 ]; then
        if [ -f lib/bsp${BSP}.a_izd_3 ]; then
          mv lib/bsp${BSP}.a_izd_3 lib/bsp${BSP}.a
          if [ -f lib/bsp${BSP}.a_izd_4 ]; then rm lib/bsp${BSP}.a_izd_4 ; fi
          if [ -f lib/bsp${BSP}.a_izd_5 ]; then rm lib/bsp${BSP}.a_izd_5 ; fi
          if [ -f lib/bsp${BSP}.a_izd_6 ]; then rm lib/bsp${BSP}.a_izd_6 ; fi
        fi
      else
        mv lib/bsp${BSP}.a_izd_2 lib/bsp${BSP}.a
      fi
    fi
  fi
fi

printf "\n  %s successfully installed to %s \n\n" $1 $OSBASE




