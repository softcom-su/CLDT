LANG=us
. ./env.sh
TARGET=oc2000bsp.bin
IMAGE=oc2000bsp.bin
RESULT=${TARGET_FILE_DIR}/oc${TARGET_FILE_NAME}.bin
export TARGET IMAGE RESULT
. ./m.sh
