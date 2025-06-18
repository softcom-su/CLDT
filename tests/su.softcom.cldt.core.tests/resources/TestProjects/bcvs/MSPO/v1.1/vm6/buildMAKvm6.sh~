#!/bin/sh

clear
echo ""
echo "******Перезагружаем БЦВС-М******"

echo -e "\003" > /dev/ttyS2

echo ""
echo "******Удаляем старый образ ОС******"

rm /var/lib/tftpboot/os2000vm6_MAK

echo ""
echo "******Собираем новый образ ОС******"

FILE_NAME="600000-mspo_v1.1.bin"
FILE_NAME_TA_VM6="5FE000-ta_vm6.bin"
PATH_MSPO="bin/"

if ! [ "$1" = "" ]; then
	FILE_NAME=$1
fi

printf "***Сборка образа ТПО БЦ-61 (%s)***\n" $PATH_MSPO$FILE_NAME

if ! [ -d $PATH_MSPO ]; then
	mkdir $PATH_MSPO
fi

if [ -f "/home/osuser/oc64sm.bin" ] ; then
	rm /home/osuser/oc64sm.bin
fi

if [ -f $PATH_MSPO$FILE_NAME ] ; then
	rm $PATH_MSPO$FILE_NAME
fi

make clean
#make
./m_bin.sh

if [ ! -f oc2000 ] ; then
	clear
	echo ""
	printf "***Ошибка сборки образа МСПО...\n"
	echo ""
	exit 1
else
	cp oc2000 /var/lib/tftpboot/os2000vm6_MAK
	cp oc2000 bin/os2000vm6_MAK
	cp /home/osuser/oc64sm.bin $PATH_MSPO$FILE_NAME
	rm /home/osuser/oc64sm.bin
fi

make clean
clear

ta_vm6_4=\\xFF\\xFF\\xFF\\xFF
ta_vm6_5=\\xFF\\xFF\\xFF\\xFF
ta_vm6_6=\\xBC\\x00\\x00\\x00
ta_vm6_7=\\xBC\\x00\\x00\\x00

echo -en $ta_vm6_4$ta_vm6_5$ta_vm6_6$ta_vm6_7 > $PATH_MSPO$FILE_NAME_TA_VM6

echo ""
printf "***Сборка образа МCПО завершена корректно (%s)\n" $PATH_MSPO$FILE_NAME
echo ""
echo "******Дожидаемся перезагрузки БЦВС-М..."

sleep 4

echo ""
echo "******Загружаем новый образ в БЦВС-М..."

echo "ifconfig dc0 10.0.0.10; boot tftp://10.0.0.1/os2000vm6_MAK" > /dev/ttyS2

sleep 2

