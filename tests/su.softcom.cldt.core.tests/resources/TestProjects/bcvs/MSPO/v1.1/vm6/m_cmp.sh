LANG=us
. ./env.sh
TARGET=oc2000comp.bin
IMAGE=oc2000comp.bin
RESULT=${TARGET_FILE_DIR}/oc${TARGET_FILE_NAME}c.bin
. ./m.sh
#scp -P 12347  ${RESULT}  root@193.232.173.215:/tftpboot/lazutin
#scp  ${RESULT}  osuser@172.16.1.161:/home/osuser