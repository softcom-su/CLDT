. ./env.sh
TARGET=oc2000
IMAGE=oc2000
RESULT=${TARGET_FILE_DIR}/oc${TARGET_FILE_NAME}
LANG=us
printf "RESULT=%s\n\r" ${RESULT}
. ./m.sh

scp -P 10021 ${RESULT} lazutin@localhost:/tftpboot/lazutin