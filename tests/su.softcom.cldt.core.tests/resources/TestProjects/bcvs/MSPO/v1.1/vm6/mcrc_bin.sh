LANG=us
. ./env.sh
TARGET=-e\ INCLUDE_CRC=1\ oc2000_crc.bin
IMAGE=oc2000_crc.bin
RESULT=${TARGET_FILE_DIR}/oc${TARGET_FILE_NAME}.bin
export TARGET IMAGE RESULT
. ./m.sh
