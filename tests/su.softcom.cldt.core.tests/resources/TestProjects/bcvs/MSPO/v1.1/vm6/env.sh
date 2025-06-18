# Script creates in directory ${TARGET_FILE_DIR}
#    OS image file with name oc${TARGET_FILE_MAME} or oc${TARGET_FILE_MAME}.bin 
#    or oc${TARGET_FILE_MAME}c.bin or oc${TARGET_FILE_MAME}r.bin

TARGET_FILE_DIR=~osuser          # home directory of osuser
TARGET_FILE_NAME=64sm             # file name 'root'

RAM_COMP_ADRS=0xa0f00000	 # uncompress image .text base
export ROM_TEXT_ADRS RAM_COMP_ADRS
