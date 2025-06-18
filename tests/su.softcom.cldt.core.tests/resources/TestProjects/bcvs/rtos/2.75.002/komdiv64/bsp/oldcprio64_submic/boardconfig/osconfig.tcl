proc dialog___BOARD_CONFIG { w } {
set wd [create_dialog $w 10 0 __BOARD_CONFIG 0 0 0 0]
dialog___BOARD_CONFIG_0 $wd.top
create_int $wd.top 19 2 BOARD_TIMER_HZ -1 10
}
# dialog___BOARD_CONFIG end
proc dialog___BOARD_CONFIG_0 { w } {
set wd [create_dialog $w 18 0 __BOARD_CONFIG_0 0 1 1 2]
create_bool $wd.top 2 "INCLUDE_FLASH" -1 1 2
create_bool $wd.top 3 "INCLUDE_NVRAM" -1 2 2
create_bool $wd.top 4 "INCLUDE_DE" -1 3 2
create_bool $wd.top 5 "INCLUDE_RGB" -1 4 2
create_bool $wd.top 6 "INCLUDE_ARINC" -1 5 2
create_bool $wd.top 7 "INCLUDE_QBUS" -1 6 2
create_bool $wd.top 8 "INCLUDE_BTM401" -1 7 2
create_bool $wd.top 9 "INCLUDE_SCSI" -1 8 2
create_bool $wd.top 10 "INCLUDE_LPT" -1 9 2
create_bool $wd.top 11 "INCLUDE_PRINTER" -1 10 2
create_bool $wd.top 12 "INCLUDE_LS120" -1 11 2
create_bool $wd.top 13 "INCLUDE_IDE" -1 12 2
create_bool $wd.top 14 "INCLUDE_PMC_PORTS" -1 13 2
create_bool $wd.top 15 "INCLUDE_AIC" -1 14 2
create_bool $wd.top 16 "INCLUDE_VI" -1 15 2
create_bool $wd.top 17 "INCLUDE_RIO" -1 16 2
create_bool $wd.top 18 "NEW_K64RIO_DRV" -1 17 2
}
# dialog___BOARD_CONFIG_0 end
proc dialog___AUX { w } {
set wd [create_dialog $w 2 0 __AUX 0 20 0 0]
}
# dialog___AUX end
proc dialog_AUX_CONFIG { w } {
global current_line thl; set current_line 0 
set wd [create_dialog $w 6 2 AUX_CONFIG 1 21 0 0]
set m [create_menu $wd.top 22 1 AUX_NAME 0 1]
set m [create_menu $wd.top 23 2 AUX_RBUF 1 1]
set m [create_menu $wd.top 24 3 AUX_WBUF 2 1]
set m [create_menu $wd.top 25 4 AUX_SPEED 3 1]
set m [create_menu $wd.top 26 5 AUX_INT_ENABLE 4 0]
for {set i 0} { $i < 2 } { incr i } {
checkbutton $thl.b$i -variable AUX_CONFIG$i
$thl add $i -itemtype window -window $thl.b$i
set ::arrvars {}
$thl item create  $i 1 -text [set ::AUX_NAME$i] 
lappend ::arrvars AUX_NAME
$thl item create  $i 2 -text [set ::AUX_RBUF$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars AUX_RBUF
$thl item create  $i 3 -text [set ::AUX_WBUF$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars AUX_WBUF
$thl item create  $i 4 -text [set ::AUX_SPEED$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars AUX_SPEED
$thl item create  $i 5 -text [set ::AUX_INT_ENABLE$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars AUX_INT_ENABLE
}
$thl selection set 0
bc $wd 0
}
# dialog_AUX_CONFIG end
proc dialog___DE_CONFIG { w } {
set wd [create_dialog $w 3 0 __DE_CONFIG 1 27 0 0]
dialog_DE0_INCLUDE $wd.top
dialog_DE1_INCLUDE $wd.top
}
# dialog___DE_CONFIG end
proc dialog_DE0_INCLUDE { w } {
set wd [create_dialog $w 8 0 DE0_INCLUDE 0 28 1 0]
set m [create_menu $wd.top 29 1 DE0_CSR11 -1 0]
set m [create_menu $wd.top 30 2 DE0_TTM -1 0]
create_bool $wd.top 31 "DE0_10HD" -1 3 0
create_bool $wd.top 32 "DE0_10FD" -1 4 0
create_bool $wd.top 33 "DE0_100HD" -1 5 0
create_bool $wd.top 34 "DE0_100FD" -1 6 0
create_bool $wd.top 35 "DE0_RESET" -1 7 0
}
# dialog_DE0_INCLUDE end
proc dialog_DE1_INCLUDE { w } {
set wd [create_dialog $w 8 0 DE1_INCLUDE 1 36 1 0]
set m [create_menu $wd.top 37 1 DE1_CSR11 -1 0]
set m [create_menu $wd.top 38 2 DE1_TTM -1 0]
create_bool $wd.top 39 "DE1_10HD" -1 3 0
create_bool $wd.top 40 "DE1_10FD" -1 4 0
create_bool $wd.top 41 "DE1_100HD" -1 5 0
create_bool $wd.top 42 "DE1_100FD" -1 6 0
create_bool $wd.top 43 "DE1_RESET" -1 7 0
}
# dialog_DE1_INCLUDE end
proc dialog___RGB_CONFIG { w } {
set wd [create_dialog $w 11 0 __RGB_CONFIG 1 44 0 0]
set m [create_menu $wd.top 45 1 COLOR_FG -1 0]
set m [create_menu $wd.top 46 2 COLOR_BG -1 0]
create_int $wd.top 47 3 TAB_SIZE -1 10
create_int $wd.top 48 4 RGB_MQ_DEPTH -1 10
create_int $wd.top 49 5 RGB_THREAD_PRI -1 10
create_bool $wd.top 50 "RGB_INHERIT_MODE" -1 6 0
create_bool $wd.top 51 "RGB_CURSOR_BLINK" -1 7 0
create_bool $wd.top 52 "RGB_FLAT_PANEL" -1 8 0
set m [create_menu $wd.top 53 9 RGB_MODE -1 0]
set m [create_menu $wd.top 54 10 FP_MODE -1 0]
}
# dialog___RGB_CONFIG end
proc dialog___IDE_CONFIG { w } {
set wd [create_dialog $w 3 0 __IDE_CONFIG 1 55 0 0]
dialog___IDE_CONFIG1 $wd.top
}
# dialog___IDE_CONFIG end
proc dialog___IDE_CONFIG1 { w } {
set wd [create_dialog $w 5 0 __IDE_CONFIG1 0 56 1 0]
create_bool $wd.top 57 "IDE_RESET" -1 1 0
create_bool $wd.top 58 "IDE_SLOW_IO" -1 2 0
set m [create_menu $wd.top 59 3 IDE_PRI -1 1]
dialog_IDE_PARAMS $wd.top
}
# dialog___IDE_CONFIG1 end
proc dialog_IDE_PARAMS { w } {
global current_line thl; set current_line 0 
set wd [create_dialog $w 7 4 IDE_PARAMS 1 60 1 0]
set m [create_menu $wd.top 61 1 IDE_NAME 0 1]
set m [create_menu $wd.top 62 2 IDE_CTRL 1 0]
set m [create_menu $wd.top 63 3 IDE_CHANNEL 2 0]
set m [create_menu $wd.top 64 4 IDE_DRIVE 3 0]
set m [create_menu $wd.top 65 5 IDE_DMA 4 0]
create_bool $wd.top 66 "IDE_MBR" 5 6 0
for {set i 0} { $i < 4 } { incr i } {
checkbutton $thl.b$i -variable IDE_PARAMS$i
$thl add $i -itemtype window -window $thl.b$i
set ::arrvars {}
$thl item create  $i 1 -text [set ::IDE_NAME$i] 
lappend ::arrvars IDE_NAME
$thl item create  $i 2 -text [set ::IDE_CTRL$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars IDE_CTRL
$thl item create  $i 3 -text [set ::IDE_CHANNEL$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars IDE_CHANNEL
$thl item create  $i 4 -text [set ::IDE_DRIVE$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars IDE_DRIVE
$thl item create  $i 5 -text [set ::IDE_DMA$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars IDE_DMA
$thl item create  $i 6 -itemtype imagetext -style [tixDisplayStyle imagetext -anchor e] -image [lindex  $::ck_list [set ::IDE_MBR$i]]
lappend ::arrvars IDE_MBR
}
$thl selection set 0
bc $wd 0
}
# dialog_IDE_PARAMS end
proc dialog___INCLUDE_BTM405_IDE { w } {
set wd [create_dialog $w 3 0 __INCLUDE_BTM405_IDE 1 67 0 0]
dialog___BTM405_IDE0 $wd.top
dialog___BTM405_IDE1 $wd.top
}
# dialog___INCLUDE_BTM405_IDE end
proc dialog___BTM405_IDE0 { w } {
set wd [create_dialog $w 5 0 __BTM405_IDE0 0 68 1 0]
set m [create_menu $wd.top 69 1 BTM405_CTRL0_CH0_PIO -1 1]
set m [create_menu $wd.top 70 2 BTM405_CTRL0_CH0_DMA -1 1]
set m [create_menu $wd.top 71 3 BTM405_CTRL0_CH1_PIO -1 1]
set m [create_menu $wd.top 72 4 BTM405_CTRL0_CH1_DMA -1 1]
}
# dialog___BTM405_IDE0 end
proc dialog___BTM405_IDE1 { w } {
set wd [create_dialog $w 5 0 __BTM405_IDE1 0 73 1 0]
set m [create_menu $wd.top 74 1 BTM405_CTRL1_CH0_PIO -1 1]
set m [create_menu $wd.top 75 2 BTM405_CTRL1_CH0_DMA -1 1]
set m [create_menu $wd.top 76 3 BTM405_CTRL1_CH1_PIO -1 1]
set m [create_menu $wd.top 77 4 BTM405_CTRL1_CH1_DMA -1 1]
}
# dialog___BTM405_IDE1 end
proc dialog___SCSI_CONFIG { w } {
set wd [create_dialog $w 4 0 __SCSI_CONFIG 1 78 0 0]
dialog___INCLUDE_BTM406 $wd.top
}
# dialog___SCSI_CONFIG end
proc dialog___INCLUDE_BTM406 { w } {
set wd [create_dialog $w 4 0 __INCLUDE_BTM406 0 79 1 0]
create_int $wd.top 80 1 BTM406_IO_PRI -1 10
set m [create_menu $wd.top 81 2 BTM406_BQ_PRI -1 1]
set m [create_menu $wd.top 82 3 BTM406_ID -1 0]
}
# dialog___INCLUDE_BTM406 end
proc dialog_SCSI_AUTO_CONFIG { w } {
set wd [create_dialog $w 10 0 SCSI_AUTO_CONFIG 1 83 0 0]
create_bool $wd.top 84 "SCSI_AUTO_LUNS" -1 1 0
create_string $wd.top 85 2 SCSI_AUTO_NAME -1
create_bool $wd.top 86 "SCSI_AUTO_DISCONNECT" -1 3 0
create_bool $wd.top 87 "SCSI_AUTO_SYNC" -1 4 0
create_bool $wd.top 88 "SCSI_AUTO_WIDE" -1 5 0
create_bool $wd.top 89 "SCSI_AUTO_TAGGED" -1 6 0
create_int $wd.top 90 7 SCSI_AUTO_TOUT -1 10
create_int $wd.top 91 8 SCSI_AUTO_SPINUP -1 10
create_bool $wd.top 92 "SCSI_AUTO_MBR" -1 9 0
}
# dialog_SCSI_AUTO_CONFIG end
proc dialog_SCSI_MAN_CONFIG { w } {
global current_line thl; set current_line 0 
set wd [create_dialog $w 11 7 SCSI_MAN_CONFIG 1 93 0 0]
create_string $wd.top 94 1 SCSI_MAN_NAME 0
create_int $wd.top 95 2 SCSI_MAN_ID 1 10
create_int $wd.top 96 3 SCSI_MAN_LUN 2 10
create_bool $wd.top 97 "SCSI_MAN_DISCONNECT" 3 4 0
create_bool $wd.top 98 "SCSI_MAN_SYNC" 4 5 0
create_bool $wd.top 99 "SCSI_MAN_WIDE" 5 6 0
create_bool $wd.top 100 "SCSI_MAN_TAGGED" 6 7 0
create_int $wd.top 101 8 SCSI_MAN_TOUT 7 10
create_int $wd.top 102 9 SCSI_MAN_SPINUP 8 10
create_bool $wd.top 103 "SCSI_MAN_MBR" 9 10 0
for {set i 0} { $i < 7 } { incr i } {
checkbutton $thl.b$i -variable SCSI_MAN_CONFIG$i
$thl add $i -itemtype window -window $thl.b$i
set ::arrvars {}
$thl item create  $i 1 -text [set ::SCSI_MAN_NAME$i] 
lappend ::arrvars SCSI_MAN_NAME
$thl item create  $i 2 -text [set ::SCSI_MAN_ID$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars SCSI_MAN_ID
$thl item create  $i 3 -text [set ::SCSI_MAN_LUN$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars SCSI_MAN_LUN
$thl item create  $i 4 -itemtype imagetext -style [tixDisplayStyle imagetext -anchor e] -image [lindex  $::ck_list [set ::SCSI_MAN_DISCONNECT$i]]
lappend ::arrvars SCSI_MAN_DISCONNECT
$thl item create  $i 5 -itemtype imagetext -style [tixDisplayStyle imagetext -anchor e] -image [lindex  $::ck_list [set ::SCSI_MAN_SYNC$i]]
lappend ::arrvars SCSI_MAN_SYNC
$thl item create  $i 6 -itemtype imagetext -style [tixDisplayStyle imagetext -anchor e] -image [lindex  $::ck_list [set ::SCSI_MAN_WIDE$i]]
lappend ::arrvars SCSI_MAN_WIDE
$thl item create  $i 7 -itemtype imagetext -style [tixDisplayStyle imagetext -anchor e] -image [lindex  $::ck_list [set ::SCSI_MAN_TAGGED$i]]
lappend ::arrvars SCSI_MAN_TAGGED
$thl item create  $i 8 -text [set ::SCSI_MAN_TOUT$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars SCSI_MAN_TOUT
$thl item create  $i 9 -text [set ::SCSI_MAN_SPINUP$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars SCSI_MAN_SPINUP
$thl item create  $i 10 -itemtype imagetext -style [tixDisplayStyle imagetext -anchor e] -image [lindex  $::ck_list [set ::SCSI_MAN_MBR$i]]
lappend ::arrvars SCSI_MAN_MBR
}
$thl selection set 0
bc $wd 0
}
# dialog_SCSI_MAN_CONFIG end
proc dialog___BSP_MISC { w } {
set wd [create_dialog $w 3 0 __BSP_MISC 0 104 0 0]
dialog___BSP_MISC1 $wd.top
dialog___CRC_ERROR $wd.top
}
# dialog___BSP_MISC end
proc dialog___BSP_MISC1 { w } {
set wd [create_dialog $w 8 0 __BSP_MISC1 0 105 1 0]
create_bool $wd.top 106 "INCLUDE_LOGGING" -1 1 0
create_int $wd.top 107 2 LOG_MSG_MAX -1 10
create_bool $wd.top 108 "BAD_INTS_LOGGING" -1 3 0
set m [create_menu $wd.top 109 4 WARM_REBOOT -1 1]
create_bool $wd.top 110 "INCLUDE_IDLE" -1 5 0
create_int $wd.top 111 6 IDLE_PRIORITY -1 10
create_int $wd.top 112 7 IDLE_DELAY -1 10
}
# dialog___BSP_MISC1 end
proc dialog___CRC_ERROR { w } {
set wd [create_dialog $w 4 0 __CRC_ERROR 0 113 1 0]
set m [create_menu $wd.top 114 1 CRC_ERROR_BEHAVIOR -1 0]
set m [create_menu $wd.top 115 2 CRC_REBOOT_ENTRY -1 1]
set m [create_menu $wd.top 116 3 CRC_NVRAM_ADDRESS -1 1]
}
# dialog___CRC_ERROR end
proc dialog__RIO_CONFIG { w } {
set wd [create_dialog $w 8 0 _RIO_CONFIG 0 117 0 0]
create_int $wd.top 118 1 RIO_SEND_THR_PRIORITY -1 10
create_int $wd.top 119 2 RIO_TIMEOUT -1 10
dialog_INCLUDE_RREG $wd.top
create_int $wd.top 377 4 RREG_TIMEOUT -1 10
create_int $wd.top 378 5 RREG_THREAD_PRI -1 10
create_int $wd.top 379 6 RREG_ID_MAX -1 10
dialog_INCLUDE_RMSG_NET $wd.top
}
# dialog__RIO_CONFIG end
proc dialog_INCLUDE_RREG { w } {
set wd [create_dialog $w 257 0 INCLUDE_RREG 1 120 1 16]
create_bool $wd.top 121 "RREG_ID00" -1 1 16
create_bool $wd.top 122 "RREG_ID01" -1 2 16
create_bool $wd.top 123 "RREG_ID02" -1 3 16
create_bool $wd.top 124 "RREG_ID03" -1 4 16
create_bool $wd.top 125 "RREG_ID04" -1 5 16
create_bool $wd.top 126 "RREG_ID05" -1 6 16
create_bool $wd.top 127 "RREG_ID06" -1 7 16
create_bool $wd.top 128 "RREG_ID07" -1 8 16
create_bool $wd.top 129 "RREG_ID08" -1 9 16
create_bool $wd.top 130 "RREG_ID09" -1 10 16
create_bool $wd.top 131 "RREG_ID0A" -1 11 16
create_bool $wd.top 132 "RREG_ID0B" -1 12 16
create_bool $wd.top 133 "RREG_ID0C" -1 13 16
create_bool $wd.top 134 "RREG_ID0D" -1 14 16
create_bool $wd.top 135 "RREG_ID0E" -1 15 16
create_bool $wd.top 136 "RREG_ID0F" -1 16 16
create_bool $wd.top 137 "RREG_ID10" -1 17 16
create_bool $wd.top 138 "RREG_ID11" -1 18 16
create_bool $wd.top 139 "RREG_ID12" -1 19 16
create_bool $wd.top 140 "RREG_ID13" -1 20 16
create_bool $wd.top 141 "RREG_ID14" -1 21 16
create_bool $wd.top 142 "RREG_ID15" -1 22 16
create_bool $wd.top 143 "RREG_ID16" -1 23 16
create_bool $wd.top 144 "RREG_ID17" -1 24 16
create_bool $wd.top 145 "RREG_ID18" -1 25 16
create_bool $wd.top 146 "RREG_ID19" -1 26 16
create_bool $wd.top 147 "RREG_ID1A" -1 27 16
create_bool $wd.top 148 "RREG_ID1B" -1 28 16
create_bool $wd.top 149 "RREG_ID1C" -1 29 16
create_bool $wd.top 150 "RREG_ID1D" -1 30 16
create_bool $wd.top 151 "RREG_ID1E" -1 31 16
create_bool $wd.top 152 "RREG_ID1F" -1 32 16
create_bool $wd.top 153 "RREG_ID20" -1 33 16
create_bool $wd.top 154 "RREG_ID21" -1 34 16
create_bool $wd.top 155 "RREG_ID22" -1 35 16
create_bool $wd.top 156 "RREG_ID23" -1 36 16
create_bool $wd.top 157 "RREG_ID24" -1 37 16
create_bool $wd.top 158 "RREG_ID25" -1 38 16
create_bool $wd.top 159 "RREG_ID26" -1 39 16
create_bool $wd.top 160 "RREG_ID27" -1 40 16
create_bool $wd.top 161 "RREG_ID28" -1 41 16
create_bool $wd.top 162 "RREG_ID29" -1 42 16
create_bool $wd.top 163 "RREG_ID2A" -1 43 16
create_bool $wd.top 164 "RREG_ID2B" -1 44 16
create_bool $wd.top 165 "RREG_ID2C" -1 45 16
create_bool $wd.top 166 "RREG_ID2D" -1 46 16
create_bool $wd.top 167 "RREG_ID2E" -1 47 16
create_bool $wd.top 168 "RREG_ID2F" -1 48 16
create_bool $wd.top 169 "RREG_ID30" -1 49 16
create_bool $wd.top 170 "RREG_ID31" -1 50 16
create_bool $wd.top 171 "RREG_ID32" -1 51 16
create_bool $wd.top 172 "RREG_ID33" -1 52 16
create_bool $wd.top 173 "RREG_ID34" -1 53 16
create_bool $wd.top 174 "RREG_ID35" -1 54 16
create_bool $wd.top 175 "RREG_ID36" -1 55 16
create_bool $wd.top 176 "RREG_ID37" -1 56 16
create_bool $wd.top 177 "RREG_ID38" -1 57 16
create_bool $wd.top 178 "RREG_ID39" -1 58 16
create_bool $wd.top 179 "RREG_ID3A" -1 59 16
create_bool $wd.top 180 "RREG_ID3B" -1 60 16
create_bool $wd.top 181 "RREG_ID3C" -1 61 16
create_bool $wd.top 182 "RREG_ID3D" -1 62 16
create_bool $wd.top 183 "RREG_ID3E" -1 63 16
create_bool $wd.top 184 "RREG_ID3F" -1 64 16
create_bool $wd.top 185 "RREG_ID40" -1 65 16
create_bool $wd.top 186 "RREG_ID41" -1 66 16
create_bool $wd.top 187 "RREG_ID42" -1 67 16
create_bool $wd.top 188 "RREG_ID43" -1 68 16
create_bool $wd.top 189 "RREG_ID44" -1 69 16
create_bool $wd.top 190 "RREG_ID45" -1 70 16
create_bool $wd.top 191 "RREG_ID46" -1 71 16
create_bool $wd.top 192 "RREG_ID47" -1 72 16
create_bool $wd.top 193 "RREG_ID48" -1 73 16
create_bool $wd.top 194 "RREG_ID49" -1 74 16
create_bool $wd.top 195 "RREG_ID4A" -1 75 16
create_bool $wd.top 196 "RREG_ID4B" -1 76 16
create_bool $wd.top 197 "RREG_ID4C" -1 77 16
create_bool $wd.top 198 "RREG_ID4D" -1 78 16
create_bool $wd.top 199 "RREG_ID4E" -1 79 16
create_bool $wd.top 200 "RREG_ID4F" -1 80 16
create_bool $wd.top 201 "RREG_ID50" -1 81 16
create_bool $wd.top 202 "RREG_ID51" -1 82 16
create_bool $wd.top 203 "RREG_ID52" -1 83 16
create_bool $wd.top 204 "RREG_ID53" -1 84 16
create_bool $wd.top 205 "RREG_ID54" -1 85 16
create_bool $wd.top 206 "RREG_ID55" -1 86 16
create_bool $wd.top 207 "RREG_ID56" -1 87 16
create_bool $wd.top 208 "RREG_ID57" -1 88 16
create_bool $wd.top 209 "RREG_ID58" -1 89 16
create_bool $wd.top 210 "RREG_ID59" -1 90 16
create_bool $wd.top 211 "RREG_ID5A" -1 91 16
create_bool $wd.top 212 "RREG_ID5B" -1 92 16
create_bool $wd.top 213 "RREG_ID5C" -1 93 16
create_bool $wd.top 214 "RREG_ID5D" -1 94 16
create_bool $wd.top 215 "RREG_ID5E" -1 95 16
create_bool $wd.top 216 "RREG_ID5F" -1 96 16
create_bool $wd.top 217 "RREG_ID60" -1 97 16
create_bool $wd.top 218 "RREG_ID61" -1 98 16
create_bool $wd.top 219 "RREG_ID62" -1 99 16
create_bool $wd.top 220 "RREG_ID63" -1 100 16
create_bool $wd.top 221 "RREG_ID64" -1 101 16
create_bool $wd.top 222 "RREG_ID65" -1 102 16
create_bool $wd.top 223 "RREG_ID66" -1 103 16
create_bool $wd.top 224 "RREG_ID67" -1 104 16
create_bool $wd.top 225 "RREG_ID68" -1 105 16
create_bool $wd.top 226 "RREG_ID69" -1 106 16
create_bool $wd.top 227 "RREG_ID6A" -1 107 16
create_bool $wd.top 228 "RREG_ID6B" -1 108 16
create_bool $wd.top 229 "RREG_ID6C" -1 109 16
create_bool $wd.top 230 "RREG_ID6D" -1 110 16
create_bool $wd.top 231 "RREG_ID6E" -1 111 16
create_bool $wd.top 232 "RREG_ID6F" -1 112 16
create_bool $wd.top 233 "RREG_ID70" -1 113 16
create_bool $wd.top 234 "RREG_ID71" -1 114 16
create_bool $wd.top 235 "RREG_ID72" -1 115 16
create_bool $wd.top 236 "RREG_ID73" -1 116 16
create_bool $wd.top 237 "RREG_ID74" -1 117 16
create_bool $wd.top 238 "RREG_ID75" -1 118 16
create_bool $wd.top 239 "RREG_ID76" -1 119 16
create_bool $wd.top 240 "RREG_ID77" -1 120 16
create_bool $wd.top 241 "RREG_ID78" -1 121 16
create_bool $wd.top 242 "RREG_ID79" -1 122 16
create_bool $wd.top 243 "RREG_ID7A" -1 123 16
create_bool $wd.top 244 "RREG_ID7B" -1 124 16
create_bool $wd.top 245 "RREG_ID7C" -1 125 16
create_bool $wd.top 246 "RREG_ID7D" -1 126 16
create_bool $wd.top 247 "RREG_ID7E" -1 127 16
create_bool $wd.top 248 "RREG_ID7F" -1 128 16
create_bool $wd.top 249 "RREG_ID80" -1 129 16
create_bool $wd.top 250 "RREG_ID81" -1 130 16
create_bool $wd.top 251 "RREG_ID82" -1 131 16
create_bool $wd.top 252 "RREG_ID83" -1 132 16
create_bool $wd.top 253 "RREG_ID84" -1 133 16
create_bool $wd.top 254 "RREG_ID85" -1 134 16
create_bool $wd.top 255 "RREG_ID86" -1 135 16
create_bool $wd.top 256 "RREG_ID87" -1 136 16
create_bool $wd.top 257 "RREG_ID88" -1 137 16
create_bool $wd.top 258 "RREG_ID89" -1 138 16
create_bool $wd.top 259 "RREG_ID8A" -1 139 16
create_bool $wd.top 260 "RREG_ID8B" -1 140 16
create_bool $wd.top 261 "RREG_ID8C" -1 141 16
create_bool $wd.top 262 "RREG_ID8D" -1 142 16
create_bool $wd.top 263 "RREG_ID8E" -1 143 16
create_bool $wd.top 264 "RREG_ID8F" -1 144 16
create_bool $wd.top 265 "RREG_ID90" -1 145 16
create_bool $wd.top 266 "RREG_ID91" -1 146 16
create_bool $wd.top 267 "RREG_ID92" -1 147 16
create_bool $wd.top 268 "RREG_ID93" -1 148 16
create_bool $wd.top 269 "RREG_ID94" -1 149 16
create_bool $wd.top 270 "RREG_ID95" -1 150 16
create_bool $wd.top 271 "RREG_ID96" -1 151 16
create_bool $wd.top 272 "RREG_ID97" -1 152 16
create_bool $wd.top 273 "RREG_ID98" -1 153 16
create_bool $wd.top 274 "RREG_ID99" -1 154 16
create_bool $wd.top 275 "RREG_ID9A" -1 155 16
create_bool $wd.top 276 "RREG_ID9B" -1 156 16
create_bool $wd.top 277 "RREG_ID9C" -1 157 16
create_bool $wd.top 278 "RREG_ID9D" -1 158 16
create_bool $wd.top 279 "RREG_ID9E" -1 159 16
create_bool $wd.top 280 "RREG_ID9F" -1 160 16
create_bool $wd.top 281 "RREG_IDA0" -1 161 16
create_bool $wd.top 282 "RREG_IDA1" -1 162 16
create_bool $wd.top 283 "RREG_IDA2" -1 163 16
create_bool $wd.top 284 "RREG_IDA3" -1 164 16
create_bool $wd.top 285 "RREG_IDA4" -1 165 16
create_bool $wd.top 286 "RREG_IDA5" -1 166 16
create_bool $wd.top 287 "RREG_IDA6" -1 167 16
create_bool $wd.top 288 "RREG_IDA7" -1 168 16
create_bool $wd.top 289 "RREG_IDA8" -1 169 16
create_bool $wd.top 290 "RREG_IDA9" -1 170 16
create_bool $wd.top 291 "RREG_IDAA" -1 171 16
create_bool $wd.top 292 "RREG_IDAB" -1 172 16
create_bool $wd.top 293 "RREG_IDAC" -1 173 16
create_bool $wd.top 294 "RREG_IDAD" -1 174 16
create_bool $wd.top 295 "RREG_IDAE" -1 175 16
create_bool $wd.top 296 "RREG_IDAF" -1 176 16
create_bool $wd.top 297 "RREG_IDB0" -1 177 16
create_bool $wd.top 298 "RREG_IDB1" -1 178 16
create_bool $wd.top 299 "RREG_IDB2" -1 179 16
create_bool $wd.top 300 "RREG_IDB3" -1 180 16
create_bool $wd.top 301 "RREG_IDB4" -1 181 16
create_bool $wd.top 302 "RREG_IDB5" -1 182 16
create_bool $wd.top 303 "RREG_IDB6" -1 183 16
create_bool $wd.top 304 "RREG_IDB7" -1 184 16
create_bool $wd.top 305 "RREG_IDB8" -1 185 16
create_bool $wd.top 306 "RREG_IDB9" -1 186 16
create_bool $wd.top 307 "RREG_IDBA" -1 187 16
create_bool $wd.top 308 "RREG_IDBB" -1 188 16
create_bool $wd.top 309 "RREG_IDBC" -1 189 16
create_bool $wd.top 310 "RREG_IDBD" -1 190 16
create_bool $wd.top 311 "RREG_IDBE" -1 191 16
create_bool $wd.top 312 "RREG_IDBF" -1 192 16
create_bool $wd.top 313 "RREG_IDC0" -1 193 16
create_bool $wd.top 314 "RREG_IDC1" -1 194 16
create_bool $wd.top 315 "RREG_IDC2" -1 195 16
create_bool $wd.top 316 "RREG_IDC3" -1 196 16
create_bool $wd.top 317 "RREG_IDC4" -1 197 16
create_bool $wd.top 318 "RREG_IDC5" -1 198 16
create_bool $wd.top 319 "RREG_IDC6" -1 199 16
create_bool $wd.top 320 "RREG_IDC7" -1 200 16
create_bool $wd.top 321 "RREG_IDC8" -1 201 16
create_bool $wd.top 322 "RREG_IDC9" -1 202 16
create_bool $wd.top 323 "RREG_IDCA" -1 203 16
create_bool $wd.top 324 "RREG_IDCB" -1 204 16
create_bool $wd.top 325 "RREG_IDCC" -1 205 16
create_bool $wd.top 326 "RREG_IDCD" -1 206 16
create_bool $wd.top 327 "RREG_IDCE" -1 207 16
create_bool $wd.top 328 "RREG_IDCF" -1 208 16
create_bool $wd.top 329 "RREG_IDD0" -1 209 16
create_bool $wd.top 330 "RREG_IDD1" -1 210 16
create_bool $wd.top 331 "RREG_IDD2" -1 211 16
create_bool $wd.top 332 "RREG_IDD3" -1 212 16
create_bool $wd.top 333 "RREG_IDD4" -1 213 16
create_bool $wd.top 334 "RREG_IDD5" -1 214 16
create_bool $wd.top 335 "RREG_IDD6" -1 215 16
create_bool $wd.top 336 "RREG_IDD7" -1 216 16
create_bool $wd.top 337 "RREG_IDD8" -1 217 16
create_bool $wd.top 338 "RREG_IDD9" -1 218 16
create_bool $wd.top 339 "RREG_IDDA" -1 219 16
create_bool $wd.top 340 "RREG_IDDB" -1 220 16
create_bool $wd.top 341 "RREG_IDDC" -1 221 16
create_bool $wd.top 342 "RREG_IDDD" -1 222 16
create_bool $wd.top 343 "RREG_IDDE" -1 223 16
create_bool $wd.top 344 "RREG_IDDF" -1 224 16
create_bool $wd.top 345 "RREG_IDE0" -1 225 16
create_bool $wd.top 346 "RREG_IDE1" -1 226 16
create_bool $wd.top 347 "RREG_IDE2" -1 227 16
create_bool $wd.top 348 "RREG_IDE3" -1 228 16
create_bool $wd.top 349 "RREG_IDE4" -1 229 16
create_bool $wd.top 350 "RREG_IDE5" -1 230 16
create_bool $wd.top 351 "RREG_IDE6" -1 231 16
create_bool $wd.top 352 "RREG_IDE7" -1 232 16
create_bool $wd.top 353 "RREG_IDE8" -1 233 16
create_bool $wd.top 354 "RREG_IDE9" -1 234 16
create_bool $wd.top 355 "RREG_IDEA" -1 235 16
create_bool $wd.top 356 "RREG_IDEB" -1 236 16
create_bool $wd.top 357 "RREG_IDEC" -1 237 16
create_bool $wd.top 358 "RREG_IDED" -1 238 16
create_bool $wd.top 359 "RREG_IDEE" -1 239 16
create_bool $wd.top 360 "RREG_IDEF" -1 240 16
create_bool $wd.top 361 "RREG_IDF0" -1 241 16
create_bool $wd.top 362 "RREG_IDF1" -1 242 16
create_bool $wd.top 363 "RREG_IDF2" -1 243 16
create_bool $wd.top 364 "RREG_IDF3" -1 244 16
create_bool $wd.top 365 "RREG_IDF4" -1 245 16
create_bool $wd.top 366 "RREG_IDF5" -1 246 16
create_bool $wd.top 367 "RREG_IDF6" -1 247 16
create_bool $wd.top 368 "RREG_IDF7" -1 248 16
create_bool $wd.top 369 "RREG_IDF8" -1 249 16
create_bool $wd.top 370 "RREG_IDF9" -1 250 16
create_bool $wd.top 371 "RREG_IDFA" -1 251 16
create_bool $wd.top 372 "RREG_IDFB" -1 252 16
create_bool $wd.top 373 "RREG_IDFC" -1 253 16
create_bool $wd.top 374 "RREG_IDFD" -1 254 16
create_bool $wd.top 375 "RREG_IDFE" -1 255 16
create_bool $wd.top 376 "RREG_IDFF" -1 256 16
}
# dialog_INCLUDE_RREG end
proc dialog_INCLUDE_RMSG_NET { w } {
set wd [create_dialog $w 3 0 INCLUDE_RMSG_NET 1 380 1 0]
create_int $wd.top 381 1 RMSG_NET_RX_SIZE -1 10
create_int $wd.top 382 2 RMSG_CHAN_RX_SIZE -1 10
}
# dialog_INCLUDE_RMSG_NET end
#### =================== dialog part end ======== 
#### =================== dialog part end ======== 
#### =================== dialog part end ======== 
#### =================== dialog part end ======== 
#### =================== dialog part end ======== 
set i ""
addvar __BOARD_CONFIG$i $i {x_configbrd.h} u  "1" 0 {} {} {} {} {} {} {} {} {Board support package configuration} __BOARD_CONFIG {} 0 bool
addvar __BOARD_CONFIG_0$i $i {x_configbrd.h} u  "1" 0 {} {} {} {} {} {} {} {} {Device drivers} __BOARD_CONFIG.__BOARD_CONFIG_0 {} 0 bool
addvar INCLUDE_FLASH$i $i {x_configbrd.h} u  "1" 0 {$INCLUDE_POSIX_SEMAPHORES==1} {} {} {} {Flash read, erase, write functions} {} {} {} {Flash} __BOARD_CONFIG.__BOARD_CONFIG_0.INCLUDE_FLASH {} 0 bool
addvar INCLUDE_NVRAM$i $i {x_configbrd.h} u  "1" 1 {} {} {} {} {Nonvolatile memory} {} {} {} {NVRAM} __BOARD_CONFIG.__BOARD_CONFIG_0.INCLUDE_NVRAM {} 0 bool
addvar INCLUDE_DE$i $i {x_configbrd.h} u  "1" 2 {} {__DE_CONFIG DE1_INCLUDE } {} {} {Network ethernet interface driver for onboard ethernet and BTM23-403/BTM23-408} {} {} {} {Ethernet} __BOARD_CONFIG.__BOARD_CONFIG_0.INCLUDE_DE {} 0 bool
addvar INCLUDE_RGB$i $i {x_configbrd.h} u  "0" 3 {} {__RGB_CONFIG } {} {} {Duplicate console output to onboard video} {} {} {} {RGB Console} __BOARD_CONFIG.__BOARD_CONFIG_0.INCLUDE_RGB {} 0 bool
addvar INCLUDE_ARINC$i $i {x_configbrd.h} u  "0" 4 {} {} {} {} {Include driver for BTM23-404 (ARINC)} {} {} {} {BTM404} __BOARD_CONFIG.__BOARD_CONFIG_0.INCLUDE_ARINC {} 0 bool
addvar INCLUDE_QBUS$i $i {x_configbrd.h} u  "0" 5 {} {} {} {} {Include driver for BTM23-402 (QBUS)} {} {} {} {QBUS} __BOARD_CONFIG.__BOARD_CONFIG_0.INCLUDE_QBUS {} 0 bool
addvar INCLUDE_BTM401$i $i {x_configbrd.h} u  "0" 6 {} {} {} {} {Include driver for PMC Manchester interface} {} {} {} {PMC Manchester} __BOARD_CONFIG.__BOARD_CONFIG_0.INCLUDE_BTM401 {} 0 bool
addvar INCLUDE_SCSI$i $i {x_configbrd.h} u  "0" 7 {} {__SCSI_CONFIG SCSI_AUTO_TOUT } {} {} {Include driver for SCSI hard disk} {} {} {} {SCSI} __BOARD_CONFIG.__BOARD_CONFIG_0.INCLUDE_SCSI {} 0 bool
addvar INCLUDE_LPT$i $i {x_configbrd.h} u  "0" 8 {} {INCLUDE_PRINTER INCLUDE_LS120 } {} {} {Include driver for LPT of multicontroller} {} {} {} {Parallel port} __BOARD_CONFIG.__BOARD_CONFIG_0.INCLUDE_LPT {} 0 bool
addvar INCLUDE_PRINTER$i $i {x_configbrd.h} u  "0" 9 {$INCLUDE_LPT==1} {} {} {} {Include driver for printer} {} {} {} {Printer} __BOARD_CONFIG.__BOARD_CONFIG_0.INCLUDE_PRINTER {} 0 bool
addvar INCLUDE_LS120$i $i {x_configbrd.h} u  "0" 10 {$INCLUDE_LPT==1} {} {} {} {Include driver for floppy LS120} {} {} {} {Floppy LS120} __BOARD_CONFIG.__BOARD_CONFIG_0.INCLUDE_LS120 {} 0 bool
addvar INCLUDE_IDE$i $i {x_configbrd.h} u  "0" 11 {} {__IDE_CONFIG IDE_RESET IDE_SLOW_IO IDE_PRI IDE_PARAMS __INCLUDE_BTM405_IDE __BTM405_IDE0 BTM405_CTRL0_CH0_PIO BTM405_CTRL0_CH0_DMA BTM405_CTRL0_CH1_PIO BTM405_CTRL0_CH1_DMA __BTM405_IDE1 BTM405_CTRL1_CH0_PIO BTM405_CTRL1_CH0_DMA BTM405_CTRL1_CH1_PIO BTM405_CTRL1_CH1_DMA } {} {} {Include driver for IDE hard disk} {} {} {} {IDE} __BOARD_CONFIG.__BOARD_CONFIG_0.INCLUDE_IDE {} 0 bool
addvar INCLUDE_PMC_PORTS$i $i {x_configbrd.h} u  "0" 12 {} {} {} {} {Configure PMC serial ports} {} {} {} {PMC serial ports} __BOARD_CONFIG.__BOARD_CONFIG_0.INCLUDE_PMC_PORTS {} 0 bool
addvar INCLUDE_AIC$i $i {x_configbrd.h} u  "0" 13 {} {} {} {} {Include driver for sound controller} {} {} {} {SOUND} __BOARD_CONFIG.__BOARD_CONFIG_0.INCLUDE_AIC {} 0 bool
addvar INCLUDE_VI$i $i {x_configbrd.h} u  "0" 14 {} {} {} {} {Include driver for video input controller} {} {} {} {VIDEO_IN} __BOARD_CONFIG.__BOARD_CONFIG_0.INCLUDE_VI {} 0 bool
addvar INCLUDE_RIO$i $i {x_configbrd.h} u  "1" 15 {[set RIO_CONFIG $INCLUDE_RIO]+1} {NEW_K64RIO_DRV _RIO_CONFIG INCLUDE_RREG } {} {} {Include driver for RapidIO} {} {} {} {RapidIO} __BOARD_CONFIG.__BOARD_CONFIG_0.INCLUDE_RIO {} 0 bool
addvar NEW_K64RIO_DRV$i $i {x_configbrd.h} u  "1" 16 {$INCLUDE_RIO==1} {} {} {} {If set, new driver for RapidIO is used} {} {} {} {New RapidIO drv} __BOARD_CONFIG.__BOARD_CONFIG_0.NEW_K64RIO_DRV {} 0 bool
addvar BOARD_TIMER_HZ$i $i {x_configbrd.h} i  "100" 1 {} {} {} {[range $P 1 10000]} {System clock frequency in Hz} {} {} {} {System clock frequency} __BOARD_CONFIG.BOARD_TIMER_HZ {} 0 int
addvar __AUX$i $i {x_configbrd.h} u  "1" 2 {} {} {} {} {} {} {} {} {Serial ports} __BOARD_CONFIG.__AUX {} 0 bool
addvar AUX_CONFIG$i $i {x_configbrd.h} u  "1" -1 {} {} {} {} {} {} {} {} {System Controller serial ports} __BOARD_CONFIG.__AUX.AUX_CONFIG {} 0 bool
set ::current_val { {/dev/console 512 512 115200 1}                 {/dev/aux1 8192 8192 115200 0} 		}
for { set i 0 } { $i < 2 } { incr i } {
addvar AUX_CONFIG$i $i {x_configbrd.h} u  "[getdva AUX_CONFIG $i]" -1 {} {} {} {} {} {} {} {} {System Controller serial ports} __BOARD_CONFIG.__AUX.AUX_CONFIG {} 0 bool
addvar AUX_NAME$i $i {x_configbrd.h} s  "/dev/aux0" 0 {} {} {} {} {} {} {} {} {Name} __BOARD_CONFIG.__AUX.AUX_CONFIG.AUX_NAME { {/dev/aux0}  {/dev/aux1}  {/dev/aux2}  {/dev/console} } 0 text
addvar AUX_RBUF$i $i {x_configbrd.h} i  "512" 1 {} {} {} {} {Input buffer size in bytes (for SLIP must be 8192)} {} {} {} {Read buf} __BOARD_CONFIG.__AUX.AUX_CONFIG.AUX_RBUF { {512}  {8192} } 0 combo
addvar AUX_WBUF$i $i {x_configbrd.h} i  "512" 2 {} {} {} {} {Output buffer size in bytes (for SLIP must be 8192)} {} {} {} {Write buf} __BOARD_CONFIG.__AUX.AUX_CONFIG.AUX_WBUF { {512}  {8192} } 0 combo
addvar AUX_SPEED$i $i {x_configbrd.h} i  "50" 3 {} {} {} {} {} {} {} {} {Baud rate} __BOARD_CONFIG.__AUX.AUX_CONFIG.AUX_SPEED { {50}  {75}  {110}  {134}  {150}  {200}  {300}  {600}  {1200}  {1800}  {2400}  {4800}  {9600}  {19200}  {38400}  {57600}  {115200} } 0 combo
addvar AUX_INT_ENABLE$i $i {x_configbrd.h} i  "1" 4 {} {} {} {} {Enable input interrupts on device creation} {} {} {} {Int enable} __BOARD_CONFIG.__AUX.AUX_CONFIG.AUX_INT_ENABLE { {1}  {0} } 0 combo
}
set i {}
set ::current_val {}
addvar __DE_CONFIG$i $i {x_configbrd.h} u  "1" 3 {$INCLUDE_DE==1} {} {} {} {1890VG3/21143 Ethernet driver setup} {} {} {} {Ethernet} __BOARD_CONFIG.__DE_CONFIG {} 0 bool
addvar DE0_INCLUDE$i $i {x_configbrd.h} u  "1" 0 {} {} {} {} {} {} {} {} {de0 (Onboard Ethernet)} __BOARD_CONFIG.__DE_CONFIG.DE0_INCLUDE {} 0 bool
addvar DE0_CSR11$i $i {x_configbrd.h} x  "0x0" 0 {} {} {} {} {Interrupts configuration. 0 - no special modes} {} {} {} {CSR11} __BOARD_CONFIG.__DE_CONFIG.DE0_INCLUDE.DE0_CSR11 { {0x0}  {0x7ffe0000} } 0 combo
addvar DE0_TTM$i $i {x_configbrd.h} i  "0" 1 {} {} {} {} {Output start threshhold} {} {} {} {TTM} __BOARD_CONFIG.__DE_CONFIG.DE0_INCLUDE.DE0_TTM { {0}  {1}  {2}  {3}  {-1} } 0 combo
addvar DE0_10HD$i $i {x_configbrd.h} u  "1" 2 {} {} {} {} {Possible connection mode} {} {} {} {10M half duplex} __BOARD_CONFIG.__DE_CONFIG.DE0_INCLUDE.DE0_10HD {} 0 bool
addvar DE0_10FD$i $i {x_configbrd.h} u  "1" 3 {} {} {} {} {Possible connection mode} {} {} {} {10M full duplex} __BOARD_CONFIG.__DE_CONFIG.DE0_INCLUDE.DE0_10FD {} 0 bool
addvar DE0_100HD$i $i {x_configbrd.h} u  "1" 4 {} {} {} {} {Possible connection mode} {} {} {} {100M half duplex} __BOARD_CONFIG.__DE_CONFIG.DE0_INCLUDE.DE0_100HD {} 0 bool
addvar DE0_100FD$i $i {x_configbrd.h} u  "1" 5 {} {} {} {} {Possible connection mode} {} {} {} {100M full duplex} __BOARD_CONFIG.__DE_CONFIG.DE0_INCLUDE.DE0_100FD {} 0 bool
addvar DE0_RESET$i $i {x_configbrd.h} u  "0" 6 {} {} {} {} {Reset interface during driver initializing procedure} {} {} {} {Forced reset} __BOARD_CONFIG.__DE_CONFIG.DE0_INCLUDE.DE0_RESET {} 0 bool
addvar DE1_INCLUDE$i $i {x_configbrd.h} u  "0" 1 {$INCLUDE_DE==1} {} {} {} {} {} {} {} {de1 (BTM23-408/BT83-405)} __BOARD_CONFIG.__DE_CONFIG.DE1_INCLUDE {} 0 bool
addvar DE1_CSR11$i $i {x_configbrd.h} x  "0x0" 0 {} {} {} {} {Interrupts configuration. 0 - no special modes} {} {} {} {CSR11} __BOARD_CONFIG.__DE_CONFIG.DE1_INCLUDE.DE1_CSR11 { {0x0}  {0x7ffe0000} } 0 combo
addvar DE1_TTM$i $i {x_configbrd.h} i  "-1" 1 {} {} {} {} {Output start threshhold} {} {} {} {TTM} __BOARD_CONFIG.__DE_CONFIG.DE1_INCLUDE.DE1_TTM { {-1}  {0}  {1}  {2}  {3} } 0 combo
addvar DE1_10HD$i $i {x_configbrd.h} u  "1" 2 {} {} {} {} {Possible connection mode} {} {} {} {10M half duplex} __BOARD_CONFIG.__DE_CONFIG.DE1_INCLUDE.DE1_10HD {} 0 bool
addvar DE1_10FD$i $i {x_configbrd.h} u  "1" 3 {} {} {} {} {Possible connection mode} {} {} {} {10M full duplex} __BOARD_CONFIG.__DE_CONFIG.DE1_INCLUDE.DE1_10FD {} 0 bool
addvar DE1_100HD$i $i {x_configbrd.h} u  "1" 4 {} {} {} {} {Possible connection mode} {} {} {} {100M half duplex} __BOARD_CONFIG.__DE_CONFIG.DE1_INCLUDE.DE1_100HD {} 0 bool
addvar DE1_100FD$i $i {x_configbrd.h} u  "1" 5 {} {} {} {} {Possible connection mode} {} {} {} {100M full duplex} __BOARD_CONFIG.__DE_CONFIG.DE1_INCLUDE.DE1_100FD {} 0 bool
addvar DE1_RESET$i $i {x_configbrd.h} u  "0" 6 {} {} {} {} {Reset interface during driver initializing procedure} {} {} {} {Forced reset} __BOARD_CONFIG.__DE_CONFIG.DE1_INCLUDE.DE1_RESET {} 0 bool
addvar __RGB_CONFIG$i $i {x_configbrd.h} u  "1" 4 {$INCLUDE_RGB==1} {} {} {} {Console on video controller setup} {} {} {} {RGB Console} __BOARD_CONFIG.__RGB_CONFIG {} 0 bool
addvar COLOR_FG$i $i {x_configbrd.h} i  "LIGHTGREEN" 0 {} {} {} {} {} {} {} {} {Foreground color} __BOARD_CONFIG.__RGB_CONFIG.COLOR_FG { {LIGHTGREEN}  {BLACK}  {LIGHTRED}  {LIGHTGREEN}  {LIGHTBLUE}  {YELLOW}  {LIGHTMAGENTA}  {LIGHTCYAN}  {LIGHTGREY}  {LIGHTBROWN}  {BROWN}  {GREEN}  {BLUE}  {MARSH}  {MAGENTA}  {CYAN}  {DARKGREY}  {WHITE} } 0 combo
addvar COLOR_BG$i $i {x_configbrd.h} i  "BLACK" 1 {} {} {} {} {} {} {} {} {Background color} __BOARD_CONFIG.__RGB_CONFIG.COLOR_BG { {BLACK}  {BLACK}  {LIGHTRED}  {LIGHTGREEN}  {LIGHTBLUE}  {YELLOW}  {LIGHTMAGENTA}  {LIGHTCYAN}  {LIGHTGREY}  {LIGHTBROWN}  {BROWN}  {GREEN}  {BLUE}  {MARSH}  {MAGENTA}  {CYAN}  {DARKGREY}  {WHITE} } 0 combo
addvar TAB_SIZE$i $i {x_configbrd.h} i  "8" 2 {} {} {} {} {} {} {} {} {Tab size} __BOARD_CONFIG.__RGB_CONFIG.TAB_SIZE {} 0 int
addvar RGB_MQ_DEPTH$i $i {x_configbrd.h} i  "1024" 3 {} {} {} {} {Size of queue for output lines} {} {} {} {Output queue size} __BOARD_CONFIG.__RGB_CONFIG.RGB_MQ_DEPTH {} 0 int
addvar RGB_THREAD_PRI$i $i {x_configbrd.h} i  "100" 4 {} {} {} {} {Priority of thread transfering data from queue to screen} {} {} {} {Thread priority} __BOARD_CONFIG.__RGB_CONFIG.RGB_THREAD_PRI {} 0 int
addvar RGB_INHERIT_MODE$i $i {x_configbrd.h} u  "1" 5 {} {} {} {} {Inherit cursor position and frame buffer data from booter} {} {} {} {Inherit mode} __BOARD_CONFIG.__RGB_CONFIG.RGB_INHERIT_MODE {} 0 bool
addvar RGB_CURSOR_BLINK$i $i {x_configbrd.h} u  "1" 6 {} {} {} {} {Enable cursor blink. If enabled only light colors available on BTM23-502} {} {} {} {Cursor blink} __BOARD_CONFIG.__RGB_CONFIG.RGB_CURSOR_BLINK {} 0 bool
addvar RGB_FLAT_PANEL$i $i {x_configbrd.h} u  "0" 7 {} {RGB_MODE FP_MODE } {} {} {Enable Flat Panel (LCD) port on BTM23-502} {} {} {} {Use Flat Panel} __BOARD_CONFIG.__RGB_CONFIG.RGB_FLAT_PANEL {} 0 bool
addvar RGB_MODE$i $i {x_configbrd.h} i  "CRT_640x480x75" 8 {$RGB_FLAT_PANEL==0} {} {} {} {} {} {} {} {CRT Video mode} __BOARD_CONFIG.__RGB_CONFIG.RGB_MODE { {CRT_640x480x75}  {CRT_640x480x60}  {CRT_640x480x72}  {CRT_640x480x85}  {CRT_640x480x100}  {CRT_800x600x56}  {CRT_800x600x60}  {CRT_800x600x72}  {CRT_800x600x85}  {CRT_800x600x100}  {CRT_1024x768x60}  {CRT_1024x768x70}  {CRT_1024x768x76}  {CRT_1024x768x85}  {CRT_1024x768x100} } 0 combo
addvar FP_MODE$i $i {x_configbrd.h} i  "FP_RS6" 9 {$RGB_FLAT_PANEL==1} {} {} {} {} {} {} {} {LCD Video mode} __BOARD_CONFIG.__RGB_CONFIG.FP_MODE { {FP_RS6}  {FP_SHARP}  {FP_RS1} } 0 combo
addvar __IDE_CONFIG$i $i {x_configbrd.h} u  "1" 5 {$INCLUDE_IDE==1} {} {} {} {IDE inteface setup} {} {} {} {IDE} __BOARD_CONFIG.__IDE_CONFIG {} 0 bool
addvar __IDE_CONFIG1$i $i {x_configbrd.h} u  "1" 0 {} {} {} {} {} {} {} {} {Common parameters} __BOARD_CONFIG.__IDE_CONFIG.__IDE_CONFIG1 {} 0 bool
addvar IDE_RESET$i $i {x_configbrd.h} u  "0" 0 {$INCLUDE_IDE==1} {} {} {} {} {} {} {} {Reset IDE after initialization} __BOARD_CONFIG.__IDE_CONFIG.__IDE_CONFIG1.IDE_RESET {} 0 bool
addvar IDE_SLOW_IO$i $i {x_configbrd.h} u  "1" 1 {$INCLUDE_IDE==1} {} {} {} {Delay between each register access} {} {} {} {Enable slow IO access} __BOARD_CONFIG.__IDE_CONFIG.__IDE_CONFIG1.IDE_SLOW_IO {} 0 bool
addvar IDE_PRI$i $i {x_configbrd.h} i  "100" 2 {$INCLUDE_IDE==1} {} {} {} {Priority of thread supporting asynchronous input/output. -1 for synchronous input/output} {} {} {} {Thread priority} __BOARD_CONFIG.__IDE_CONFIG.__IDE_CONFIG1.IDE_PRI { {100}  {-1} } 0 int
addvar IDE_PARAMS$i $i {x_configbrd.h} u  "0" -1 {$INCLUDE_IDE==1} {} {} {} {} {} {} {} {Attached hard disk drives} __BOARD_CONFIG.__IDE_CONFIG.__IDE_CONFIG1.IDE_PARAMS {} 0 bool
set ::current_val {{/dev/ide0 0 0 IDE_MASTER 0 1} 	               {/dev/ide1 0 1 IDE_MASTER 0 1}                        {/dev/ide2 1 0 IDE_MASTER 0 1}                        {/dev/ide3 1 1 IDE_MASTER 0 1}}
for { set i 0 } { $i < 4 } { incr i } {
addvar IDE_PARAMS$i $i {x_configbrd.h} u  "[getdva IDE_PARAMS $i]" -1 {$INCLUDE_IDE==1} {} {} {} {} {} {} {} {Attached hard disk drives} __BOARD_CONFIG.__IDE_CONFIG.__IDE_CONFIG1.IDE_PARAMS {} 0 bool
addvar IDE_NAME$i $i {x_configbrd.h} s  "/dev/ide0" 0 {} {} {} {} {Drive device name} {} {} {} {Device name} __BOARD_CONFIG.__IDE_CONFIG.__IDE_CONFIG1.IDE_PARAMS.IDE_NAME { {/dev/ide0}  {/dev/ide1}  {/dev/ide2}  {/dev/ide3} } 0 text
addvar IDE_CTRL$i $i {x_configbrd.h} i  "0" 1 {} {} {} {} {Controller (BTM23-405) number (0 or 1)} {} {} {} {Controller} __BOARD_CONFIG.__IDE_CONFIG.__IDE_CONFIG1.IDE_PARAMS.IDE_CTRL { {0}  {1}  {2}  {3} } 0 int
addvar IDE_CHANNEL$i $i {x_configbrd.h} i  "0" 2 {} {} {} {} {Channel number} {} {} {} {Channel} __BOARD_CONFIG.__IDE_CONFIG.__IDE_CONFIG1.IDE_PARAMS.IDE_CHANNEL { {0}  {1} } 0 int
addvar IDE_DRIVE$i $i {x_configbrd.h} i  "IDE_MASTER" 3 {} {} {} {} {Drive type (master or slave)} {} {} {} {M/S} __BOARD_CONFIG.__IDE_CONFIG.__IDE_CONFIG1.IDE_PARAMS.IDE_DRIVE { {IDE_MASTER}  {IDE_SLAVE} } 0 combo
addvar IDE_DMA$i $i {x_configbrd.h} i  "0" 4 {} {} {} {} {Enable embedded DMA} {} {} {} {Enable DMA} __BOARD_CONFIG.__IDE_CONFIG.__IDE_CONFIG1.IDE_PARAMS.IDE_DMA { {0}  {1} } 0 combo
addvar IDE_MBR$i $i {x_configbrd.h} u  "1" 5 {} {} {} {} {Use Master Boot Record (disk slicing)} {} {} {} {Use MBR} __BOARD_CONFIG.__IDE_CONFIG.__IDE_CONFIG1.IDE_PARAMS.IDE_MBR {} 0 bool
}
set i {}
set ::current_val {}
addvar __INCLUDE_BTM405_IDE$i $i {x_configbrd.h} u  "1" 1 {$INCLUDE_IDE==1} {} {} {} {} {} {} {} {BTM23-405 IDE} __BOARD_CONFIG.__IDE_CONFIG.__INCLUDE_BTM405_IDE {} 0 bool
addvar __BTM405_IDE0$i $i {x_configbrd.h} u  "1" 0 {$INCLUDE_IDE==1} {} {} {} {BAGET_TIMINGS_DEF - default setups} {} {} {} {Controller 0 Timings} __BOARD_CONFIG.__IDE_CONFIG.__INCLUDE_BTM405_IDE.__BTM405_IDE0 {} 0 bool
addvar BTM405_CTRL0_CH0_PIO$i $i {x_configbrd.h} i  "BAGET_TIMINGS_DEF" 0 {$INCLUDE_IDE==1} {} {} {} {BAGET_TIMINGS_DEF - default setups} {} {} {} {Channel 0 PIO} __BOARD_CONFIG.__IDE_CONFIG.__INCLUDE_BTM405_IDE.__BTM405_IDE0.BTM405_CTRL0_CH0_PIO { {BAGET_TIMINGS_DEF}  {BAGET_33MHZ_PIO4}  {BAGET_25MHZ_PIO4} } 0 combo
addvar BTM405_CTRL0_CH0_DMA$i $i {x_configbrd.h} i  "BAGET_TIMINGS_DEF" 1 {$INCLUDE_IDE==1} {} {} {} {BAGET_TIMINGS_DEF - default setups} {} {} {} {Channel 0 DMA} __BOARD_CONFIG.__IDE_CONFIG.__INCLUDE_BTM405_IDE.__BTM405_IDE0.BTM405_CTRL0_CH0_DMA { {BAGET_TIMINGS_DEF}  {BAGET_33MHZ_MDMA2}  {BAGET_25MHZ_MDMA2} } 0 combo
addvar BTM405_CTRL0_CH1_PIO$i $i {x_configbrd.h} i  "BAGET_TIMINGS_DEF" 2 {$INCLUDE_IDE==1} {} {} {} {BAGET_TIMINGS_DEF - default setups} {} {} {} {Channel 1 PIO} __BOARD_CONFIG.__IDE_CONFIG.__INCLUDE_BTM405_IDE.__BTM405_IDE0.BTM405_CTRL0_CH1_PIO { {BAGET_TIMINGS_DEF}  {BAGET_33MHZ_PIO4}  {BAGET_25MHZ_PIO4} } 0 combo
addvar BTM405_CTRL0_CH1_DMA$i $i {x_configbrd.h} i  "BAGET_TIMINGS_DEF" 3 {$INCLUDE_IDE==1} {} {} {} {BAGET_TIMINGS_DEF - default setups} {} {} {} {Channel 1 DMA} __BOARD_CONFIG.__IDE_CONFIG.__INCLUDE_BTM405_IDE.__BTM405_IDE0.BTM405_CTRL0_CH1_DMA { {BAGET_TIMINGS_DEF}  {BAGET_33MHZ_MDMA2}  {BAGET_25MHZ_MDMA2} } 0 combo
addvar __BTM405_IDE1$i $i {x_configbrd.h} u  "1" 1 {$INCLUDE_IDE==1} {} {} {} {BAGET_TIMINGS_DEF - default setups} {} {} {} {Controller 1 Timings} __BOARD_CONFIG.__IDE_CONFIG.__INCLUDE_BTM405_IDE.__BTM405_IDE1 {} 0 bool
addvar BTM405_CTRL1_CH0_PIO$i $i {x_configbrd.h} i  "BAGET_TIMINGS_DEF" 0 {$INCLUDE_IDE==1} {} {} {} {BAGET_TIMINGS_DEF - default setups} {} {} {} {Channel 0 PIO} __BOARD_CONFIG.__IDE_CONFIG.__INCLUDE_BTM405_IDE.__BTM405_IDE1.BTM405_CTRL1_CH0_PIO { {BAGET_TIMINGS_DEF}  {BAGET_33MHZ_PIO4}  {BAGET_25MHZ_PIO4} } 0 combo
addvar BTM405_CTRL1_CH0_DMA$i $i {x_configbrd.h} i  "BAGET_TIMINGS_DEF" 1 {$INCLUDE_IDE==1} {} {} {} {BAGET_TIMINGS_DEF - default setups} {} {} {} {Channel 0 DMA} __BOARD_CONFIG.__IDE_CONFIG.__INCLUDE_BTM405_IDE.__BTM405_IDE1.BTM405_CTRL1_CH0_DMA { {BAGET_TIMINGS_DEF}  {BAGET_33MHZ_MDMA2}  {BAGET_25MHZ_MDMA2} } 0 combo
addvar BTM405_CTRL1_CH1_PIO$i $i {x_configbrd.h} i  "BAGET_TIMINGS_DEF" 2 {$INCLUDE_IDE==1} {} {} {} {BAGET_TIMINGS_DEF - default setups} {} {} {} {Channel 1 PIO} __BOARD_CONFIG.__IDE_CONFIG.__INCLUDE_BTM405_IDE.__BTM405_IDE1.BTM405_CTRL1_CH1_PIO { {BAGET_TIMINGS_DEF}  {BAGET_33MHZ_PIO4}  {BAGET_25MHZ_PIO4} } 0 combo
addvar BTM405_CTRL1_CH1_DMA$i $i {x_configbrd.h} i  "BAGET_TIMINGS_DEF" 3 {$INCLUDE_IDE==1} {} {} {} {BAGET_TIMINGS_DEF - default setups} {} {} {} {Channel 1 DMA} __BOARD_CONFIG.__IDE_CONFIG.__INCLUDE_BTM405_IDE.__BTM405_IDE1.BTM405_CTRL1_CH1_DMA { {BAGET_TIMINGS_DEF}  {BAGET_33MHZ_MDMA2}  {BAGET_25MHZ_MDMA2} } 0 combo
addvar __SCSI_CONFIG$i $i {x_configbrd.h} u  "1" 6 {$INCLUDE_SCSI==1} {} {} {} {} {} {} {} {SCSI controller} __BOARD_CONFIG.__SCSI_CONFIG {} 0 bool
addvar __INCLUDE_BTM406$i $i {x_configbrd.h} u  "1" 0 {} {} {} {} {} {} {} {} {SCSI threads} __BOARD_CONFIG.__SCSI_CONFIG.__INCLUDE_BTM406 {} 0 bool
addvar BTM406_IO_PRI$i $i {x_configbrd.h} i  "240" 0 {} {} {} {} {Priority of thread, supporting controller} {} {} {} {Controller thread priority} __BOARD_CONFIG.__SCSI_CONFIG.__INCLUDE_BTM406.BTM406_IO_PRI {} 0 int
addvar BTM406_BQ_PRI$i $i {x_configbrd.h} i  "100" 1 {} {} {} {} {Priority of thread for asynchronous input/output. -1 for synchronous input/output} {} {} {} {Data transfere thread priority} __BOARD_CONFIG.__SCSI_CONFIG.__INCLUDE_BTM406.BTM406_BQ_PRI { {100}  {-1} } 0 int
addvar BTM406_ID$i $i {x_configbrd.h} i  "7" 2 {} {} {} {} {Unique adapter ID on SCSI bus} {} {} {} {Adapter bus ID} __BOARD_CONFIG.__SCSI_CONFIG.__INCLUDE_BTM406.BTM406_ID { {7}  {6}  {5}  {4}  {3}  {2}  {1}  {0} } 0 int
addvar SCSI_AUTO_CONFIG$i $i {x_configbrd.h} u  "1" 1 {} {SCSI_MAN_CONFIG } {} {} {} {} {} {auto_man} {Automatic Configuration} __BOARD_CONFIG.__SCSI_CONFIG.SCSI_AUTO_CONFIG {} 0 bool
addvar SCSI_AUTO_LUNS$i $i {x_configbrd.h} u  "0" 0 {} {} {} {} {Autoconfig logical subdevices} {} {} {} {Config LUNs} __BOARD_CONFIG.__SCSI_CONFIG.SCSI_AUTO_CONFIG.SCSI_AUTO_LUNS {} 0 bool
addvar SCSI_AUTO_NAME$i $i {x_configbrd.h} s  "/dev/scsi" 1 {} {} {} {} {} {} {} {} {Device name} __BOARD_CONFIG.__SCSI_CONFIG.SCSI_AUTO_CONFIG.SCSI_AUTO_NAME {} 0 text
addvar SCSI_AUTO_DISCONNECT$i $i {x_configbrd.h} u  "1" 2 {} {} {} {} {Disconnect if no data} {} {} {} {Disconnect} __BOARD_CONFIG.__SCSI_CONFIG.SCSI_AUTO_CONFIG.SCSI_AUTO_DISCONNECT {} 0 bool
addvar SCSI_AUTO_SYNC$i $i {x_configbrd.h} u  "1" 3 {} {} {} {} {Enable synchronous mode on SCSI bus - transfere data in blocks} {} {} {} {Synchronous mode} __BOARD_CONFIG.__SCSI_CONFIG.SCSI_AUTO_CONFIG.SCSI_AUTO_SYNC {} 0 bool
addvar SCSI_AUTO_WIDE$i $i {x_configbrd.h} u  "1" 4 {} {} {} {} {Transfere data by 16 bits words} {} {} {} {Wide mode} __BOARD_CONFIG.__SCSI_CONFIG.SCSI_AUTO_CONFIG.SCSI_AUTO_WIDE {} 0 bool
addvar SCSI_AUTO_TAGGED$i $i {x_configbrd.h} u  "1" 5 {} {} {} {} {Enable requests queueing on SCSI-bus} {} {} {} {Tagged} __BOARD_CONFIG.__SCSI_CONFIG.SCSI_AUTO_CONFIG.SCSI_AUTO_TAGGED {} 0 bool
addvar SCSI_AUTO_TOUT$i $i {x_configbrd.h} i  "0" 6 {$INCLUDE_SCSI==1} {} {} {} {Timeout interval for input/output request} {} {} {} {Request timeout} __BOARD_CONFIG.__SCSI_CONFIG.SCSI_AUTO_CONFIG.SCSI_AUTO_TOUT {} 0 int
addvar SCSI_AUTO_SPINUP$i $i {x_configbrd.h} i  "0" 7 {} {} {} {} {Disk spinup timeout} {} {} {} {Spinup timeout} __BOARD_CONFIG.__SCSI_CONFIG.SCSI_AUTO_CONFIG.SCSI_AUTO_SPINUP {} 0 int
addvar SCSI_AUTO_MBR$i $i {x_configbrd.h} u  "1" 8 {} {} {} {} {Use Master Boot Record (disk slicing)} {} {} {} {Use MBR} __BOARD_CONFIG.__SCSI_CONFIG.SCSI_AUTO_CONFIG.SCSI_AUTO_MBR {} 0 bool
addvar SCSI_MAN_CONFIG$i $i {x_configbrd.h} u  "0" -1 {$SCSI_AUTO_CONFIG==0} {} {} {} {} {} {} {auto_man} {Manual configuration} __BOARD_CONFIG.__SCSI_CONFIG.SCSI_MAN_CONFIG {} 0 bool
set ::current_val {{/dev/scsi0 0 0 1 1 1 1 0 0 1} 		      {/dev/scsi1 1 0 1 1 1 1 0 0 1}                       {/dev/scsi2 2 0 1 1 1 1 0 0 1}                       {/dev/scsi3 3 0 1 1 1 1 0 0 1} 		      {/dev/scsi4 4 0 1 1 1 1 0 0 1}                       {/dev/scsi5 5 0 1 1 1 1 0 0 1}                       {/dev/scsi6 6 0 1 1 1 1 0 0 1}}
for { set i 0 } { $i < 7 } { incr i } {
addvar SCSI_MAN_CONFIG$i $i {x_configbrd.h} u  "[getdva SCSI_MAN_CONFIG $i]" -1 {$SCSI_AUTO_CONFIG==0} {} {} {} {} {} {} {auto_man} {Manual configuration} __BOARD_CONFIG.__SCSI_CONFIG.SCSI_MAN_CONFIG {} 0 bool
addvar SCSI_MAN_NAME$i $i {x_configbrd.h} s  "" 0 {} {} {} {} {} {} {} {} {Device name} __BOARD_CONFIG.__SCSI_CONFIG.SCSI_MAN_CONFIG.SCSI_MAN_NAME {} 0 text
addvar SCSI_MAN_ID$i $i {x_configbrd.h} i  "0" 1 {} {} {} {} {ID of the device on SCSI BUS} {} {} {} {ID} __BOARD_CONFIG.__SCSI_CONFIG.SCSI_MAN_CONFIG.SCSI_MAN_ID {} 0 int
addvar SCSI_MAN_LUN$i $i {x_configbrd.h} i  "0" 2 {} {} {} {} {Subdevice number} {} {} {} {LUN} __BOARD_CONFIG.__SCSI_CONFIG.SCSI_MAN_CONFIG.SCSI_MAN_LUN {} 0 int
addvar SCSI_MAN_DISCONNECT$i $i {x_configbrd.h} u  "1" 3 {} {} {} {} {Disconnect if no data} {} {} {} {Dscnct} __BOARD_CONFIG.__SCSI_CONFIG.SCSI_MAN_CONFIG.SCSI_MAN_DISCONNECT {} 0 bool
addvar SCSI_MAN_SYNC$i $i {x_configbrd.h} u  "1" 4 {} {} {} {} {Enable synchronous mode on SCSI bus - transfere data in blocks} {} {} {} {Sync} __BOARD_CONFIG.__SCSI_CONFIG.SCSI_MAN_CONFIG.SCSI_MAN_SYNC {} 0 bool
addvar SCSI_MAN_WIDE$i $i {x_configbrd.h} u  "1" 5 {} {} {} {} {Transfere data by 16 bits words} {} {} {} {Wide mode} __BOARD_CONFIG.__SCSI_CONFIG.SCSI_MAN_CONFIG.SCSI_MAN_WIDE {} 0 bool
addvar SCSI_MAN_TAGGED$i $i {x_configbrd.h} u  "1" 6 {} {} {} {} {Enable requests queueing on SCSI-bus} {} {} {} {Tagged} __BOARD_CONFIG.__SCSI_CONFIG.SCSI_MAN_CONFIG.SCSI_MAN_TAGGED {} 0 bool
addvar SCSI_MAN_TOUT$i $i {x_configbrd.h} i  "0" 7 {} {} {} {} {Timeout interval for input/output request} {} {} {} {RTout} __BOARD_CONFIG.__SCSI_CONFIG.SCSI_MAN_CONFIG.SCSI_MAN_TOUT {} 0 int
addvar SCSI_MAN_SPINUP$i $i {x_configbrd.h} i  "0" 8 {} {} {} {} {Disk spinup timeout} {} {} {} {STout} __BOARD_CONFIG.__SCSI_CONFIG.SCSI_MAN_CONFIG.SCSI_MAN_SPINUP {} 0 int
addvar SCSI_MAN_MBR$i $i {x_configbrd.h} u  "1" 9 {} {} {} {} {Use Master Boot Record (disk slicing)} {} {} {} {Use MBR} __BOARD_CONFIG.__SCSI_CONFIG.SCSI_MAN_CONFIG.SCSI_MAN_MBR {} 0 bool
}
set i {}
set ::current_val {}
addvar __BSP_MISC$i $i {x_configbrd.h} u  "1" 7 {} {} {} {} {} {} {} {} {Miscellaneous} __BOARD_CONFIG.__BSP_MISC {} 0 bool
addvar __BSP_MISC1$i $i {x_configbrd.h} u  "1" 0 {} {} {} {} {} {} {} {} {Additional libraries and flags} __BOARD_CONFIG.__BSP_MISC.__BSP_MISC1 {} 0 bool
addvar INCLUDE_LOGGING$i $i {x_configbrd.h} u  "1" 0 {} {LOG_MSG_MAX BAD_INTS_LOGGING } {} {} {Message output library for interrupt handlers} {} {} {} {INCLUDE_LOGGING} __BOARD_CONFIG.__BSP_MISC.__BSP_MISC1.INCLUDE_LOGGING {} 0 bool
addvar LOG_MSG_MAX$i $i {x_configbrd.h} i  "100" 1 {$INCLUDE_LOGGING==1} {} {} {} {} {} {} {} {LOG_MSG_MAX} __BOARD_CONFIG.__BSP_MISC.__BSP_MISC1.LOG_MSG_MAX {} 0 int
addvar BAD_INTS_LOGGING$i $i {x_configbrd.h} u  "1" 2 {$INCLUDE_LOGGING==1} {} {} {} {Output messages about some bad events: bus errors, etc.} {} {} {} {BAD_INTS_LOGGING} __BOARD_CONFIG.__BSP_MISC.__BSP_MISC1.BAD_INTS_LOGGING {} 0 bool
addvar WARM_REBOOT$i $i {x_configbrd.h} d  "0xbfc00000" 3 {} {} {} {} {^X jump entry. Note: flash based at 0xbd000000, prime at 0xbfc00000} {} {} {} {Reboot entry} __BOARD_CONFIG.__BSP_MISC.__BSP_MISC1.WARM_REBOOT { {0xbfc00000}  {0xbd000000} } 0 text
addvar INCLUDE_IDLE$i $i {x_configbrd.h} u  "0" 4 {} {IDLE_PRIORITY IDLE_DELAY } {} {} {CPU load measurements library} {} {} {} {INCLUDE_IDLE} __BOARD_CONFIG.__BSP_MISC.__BSP_MISC1.INCLUDE_IDLE {} 0 bool
addvar IDLE_PRIORITY$i $i {x_configbrd.h} i  "0" 5 {$INCLUDE_IDLE==1} {} {} {} {} {} {} {} {IDLE_PRIORITY} __BOARD_CONFIG.__BSP_MISC.__BSP_MISC1.IDLE_PRIORITY {} 0 int
addvar IDLE_DELAY$i $i {x_configbrd.h} i  "32" 6 {$INCLUDE_IDLE==1} {} {} {} {} {} {} {} {IDLE_DELAY} __BOARD_CONFIG.__BSP_MISC.__BSP_MISC1.IDLE_DELAY {} 0 int
addvar __CRC_ERROR$i $i {x_configbrd.h} u  "1" 1 {} {} {} {} {Check OS image loading using check sum} {} {} {} {CRC} __BOARD_CONFIG.__BSP_MISC.__CRC_ERROR {} 0 bool
addvar CRC_ERROR_BEHAVIOR$i $i {x_configbrd.h} i  "1" 0 {} {} {} {} {Ignore, reboot or stop} {} {} {} {Error processing} __BOARD_CONFIG.__BSP_MISC.__CRC_ERROR.CRC_ERROR_BEHAVIOR { {CRC_IGNORE}  {CRC_REBOOT}  {CRC_STOP} } 0 int
addvar CRC_REBOOT_ENTRY$i $i {x_configbrd.h} d  "0xbfc00000" 1 {} {} {} {} {Entry point for reboot} {} {} {} {Reboot entry} __BOARD_CONFIG.__BSP_MISC.__CRC_ERROR.CRC_REBOOT_ENTRY { {0xbfc00000}  {0xbd000000} } 0 text
addvar CRC_NVRAM_ADDRESS$i $i {x_configbrd.h} d  "0xa0000600" 2 {} {} {} {} {Address to store check results (Special scratch register by default)} {} {} {} {Check result address} __BOARD_CONFIG.__BSP_MISC.__CRC_ERROR.CRC_NVRAM_ADDRESS { {0xa0000600} } 0 text
addvar _RIO_CONFIG$i $i {x_configbrd.h} u  "1" 8 {$INCLUDE_RIO==1} {} {} {} {} {} {} {} {Rapid IO Configuration} __BOARD_CONFIG._RIO_CONFIG {} 0 bool
addvar RIO_SEND_THR_PRIORITY$i $i {x_configbrd.h} i  "120" 0 {} {} {} {} {} {} {} {} {RIO send thread priority} __BOARD_CONFIG._RIO_CONFIG.RIO_SEND_THR_PRIORITY {} 0 int
addvar RIO_TIMEOUT$i $i {x_configbrd.h} i  "10" 1 {} {} {} {} {} {} {} {} {RIO request/data timeout (sec)} __BOARD_CONFIG._RIO_CONFIG.RIO_TIMEOUT {} 0 int
addvar INCLUDE_RREG$i $i {x_configbrd.h} u  "1" 2 {$INCLUDE_RIO==1} {RREG_TIMEOUT RREG_THREAD_PRI RREG_ID_MAX INCLUDE_RMSG_NET } {} {} {} {} {} {} {RIO ID Registration Table} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG {} 0 bool
addvar RREG_ID00$i $i {x_configbrd.h} x  "0" 0 {} {} {} {} {} {} {} {} {0} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID00 {} 0 bool
addvar RREG_ID01$i $i {x_configbrd.h} x  "0" 1 {} {} {} {} {} {} {} {} {1} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID01 {} 0 bool
addvar RREG_ID02$i $i {x_configbrd.h} x  "0" 2 {} {} {} {} {} {} {} {} {2} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID02 {} 0 bool
addvar RREG_ID03$i $i {x_configbrd.h} x  "0" 3 {} {} {} {} {} {} {} {} {3} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID03 {} 0 bool
addvar RREG_ID04$i $i {x_configbrd.h} x  "0" 4 {} {} {} {} {} {} {} {} {4} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID04 {} 0 bool
addvar RREG_ID05$i $i {x_configbrd.h} x  "0" 5 {} {} {} {} {} {} {} {} {5} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID05 {} 0 bool
addvar RREG_ID06$i $i {x_configbrd.h} x  "0" 6 {} {} {} {} {} {} {} {} {6} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID06 {} 0 bool
addvar RREG_ID07$i $i {x_configbrd.h} x  "0" 7 {} {} {} {} {} {} {} {} {7} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID07 {} 0 bool
addvar RREG_ID08$i $i {x_configbrd.h} x  "0" 8 {} {} {} {} {} {} {} {} {8} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID08 {} 0 bool
addvar RREG_ID09$i $i {x_configbrd.h} x  "0" 9 {} {} {} {} {} {} {} {} {9} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID09 {} 0 bool
addvar RREG_ID0A$i $i {x_configbrd.h} x  "0" 10 {} {} {} {} {} {} {} {} {A} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID0A {} 0 bool
addvar RREG_ID0B$i $i {x_configbrd.h} x  "0" 11 {} {} {} {} {} {} {} {} {B} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID0B {} 0 bool
addvar RREG_ID0C$i $i {x_configbrd.h} x  "0" 12 {} {} {} {} {} {} {} {} {C} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID0C {} 0 bool
addvar RREG_ID0D$i $i {x_configbrd.h} x  "0" 13 {} {} {} {} {} {} {} {} {D} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID0D {} 0 bool
addvar RREG_ID0E$i $i {x_configbrd.h} x  "0" 14 {} {} {} {} {} {} {} {} {E} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID0E {} 0 bool
addvar RREG_ID0F$i $i {x_configbrd.h} x  "0" 15 {} {} {} {} {} {} {} {} {F} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID0F {} 0 bool
addvar RREG_ID10$i $i {x_configbrd.h} x  "0" 16 {} {} {} {} {} {} {} {} {10} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID10 {} 0 bool
addvar RREG_ID11$i $i {x_configbrd.h} x  "0" 17 {} {} {} {} {} {} {} {} {11} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID11 {} 0 bool
addvar RREG_ID12$i $i {x_configbrd.h} x  "0" 18 {} {} {} {} {} {} {} {} {12} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID12 {} 0 bool
addvar RREG_ID13$i $i {x_configbrd.h} x  "0" 19 {} {} {} {} {} {} {} {} {13} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID13 {} 0 bool
addvar RREG_ID14$i $i {x_configbrd.h} x  "0" 20 {} {} {} {} {} {} {} {} {14} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID14 {} 0 bool
addvar RREG_ID15$i $i {x_configbrd.h} x  "0" 21 {} {} {} {} {} {} {} {} {15} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID15 {} 0 bool
addvar RREG_ID16$i $i {x_configbrd.h} x  "0" 22 {} {} {} {} {} {} {} {} {16} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID16 {} 0 bool
addvar RREG_ID17$i $i {x_configbrd.h} x  "0" 23 {} {} {} {} {} {} {} {} {17} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID17 {} 0 bool
addvar RREG_ID18$i $i {x_configbrd.h} x  "0" 24 {} {} {} {} {} {} {} {} {18} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID18 {} 0 bool
addvar RREG_ID19$i $i {x_configbrd.h} x  "0" 25 {} {} {} {} {} {} {} {} {19} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID19 {} 0 bool
addvar RREG_ID1A$i $i {x_configbrd.h} x  "0" 26 {} {} {} {} {} {} {} {} {1A} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID1A {} 0 bool
addvar RREG_ID1B$i $i {x_configbrd.h} x  "0" 27 {} {} {} {} {} {} {} {} {1B} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID1B {} 0 bool
addvar RREG_ID1C$i $i {x_configbrd.h} x  "0" 28 {} {} {} {} {} {} {} {} {1C} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID1C {} 0 bool
addvar RREG_ID1D$i $i {x_configbrd.h} x  "0" 29 {} {} {} {} {} {} {} {} {1D} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID1D {} 0 bool
addvar RREG_ID1E$i $i {x_configbrd.h} x  "0" 30 {} {} {} {} {} {} {} {} {1E} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID1E {} 0 bool
addvar RREG_ID1F$i $i {x_configbrd.h} x  "0" 31 {} {} {} {} {} {} {} {} {1F} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID1F {} 0 bool
addvar RREG_ID20$i $i {x_configbrd.h} x  "0" 32 {} {} {} {} {} {} {} {} {20} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID20 {} 0 bool
addvar RREG_ID21$i $i {x_configbrd.h} x  "0" 33 {} {} {} {} {} {} {} {} {21} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID21 {} 0 bool
addvar RREG_ID22$i $i {x_configbrd.h} x  "0" 34 {} {} {} {} {} {} {} {} {22} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID22 {} 0 bool
addvar RREG_ID23$i $i {x_configbrd.h} x  "0" 35 {} {} {} {} {} {} {} {} {23} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID23 {} 0 bool
addvar RREG_ID24$i $i {x_configbrd.h} x  "0" 36 {} {} {} {} {} {} {} {} {24} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID24 {} 0 bool
addvar RREG_ID25$i $i {x_configbrd.h} x  "0" 37 {} {} {} {} {} {} {} {} {25} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID25 {} 0 bool
addvar RREG_ID26$i $i {x_configbrd.h} x  "0" 38 {} {} {} {} {} {} {} {} {26} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID26 {} 0 bool
addvar RREG_ID27$i $i {x_configbrd.h} x  "0" 39 {} {} {} {} {} {} {} {} {27} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID27 {} 0 bool
addvar RREG_ID28$i $i {x_configbrd.h} x  "0" 40 {} {} {} {} {} {} {} {} {28} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID28 {} 0 bool
addvar RREG_ID29$i $i {x_configbrd.h} x  "0" 41 {} {} {} {} {} {} {} {} {29} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID29 {} 0 bool
addvar RREG_ID2A$i $i {x_configbrd.h} x  "0" 42 {} {} {} {} {} {} {} {} {2A} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID2A {} 0 bool
addvar RREG_ID2B$i $i {x_configbrd.h} x  "0" 43 {} {} {} {} {} {} {} {} {2B} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID2B {} 0 bool
addvar RREG_ID2C$i $i {x_configbrd.h} x  "0" 44 {} {} {} {} {} {} {} {} {2C} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID2C {} 0 bool
addvar RREG_ID2D$i $i {x_configbrd.h} x  "0" 45 {} {} {} {} {} {} {} {} {2D} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID2D {} 0 bool
addvar RREG_ID2E$i $i {x_configbrd.h} x  "0" 46 {} {} {} {} {} {} {} {} {2E} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID2E {} 0 bool
addvar RREG_ID2F$i $i {x_configbrd.h} x  "0" 47 {} {} {} {} {} {} {} {} {2F} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID2F {} 0 bool
addvar RREG_ID30$i $i {x_configbrd.h} x  "0" 48 {} {} {} {} {} {} {} {} {30} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID30 {} 0 bool
addvar RREG_ID31$i $i {x_configbrd.h} x  "0" 49 {} {} {} {} {} {} {} {} {31} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID31 {} 0 bool
addvar RREG_ID32$i $i {x_configbrd.h} x  "0" 50 {} {} {} {} {} {} {} {} {32} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID32 {} 0 bool
addvar RREG_ID33$i $i {x_configbrd.h} x  "0" 51 {} {} {} {} {} {} {} {} {33} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID33 {} 0 bool
addvar RREG_ID34$i $i {x_configbrd.h} x  "0" 52 {} {} {} {} {} {} {} {} {34} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID34 {} 0 bool
addvar RREG_ID35$i $i {x_configbrd.h} x  "0" 53 {} {} {} {} {} {} {} {} {35} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID35 {} 0 bool
addvar RREG_ID36$i $i {x_configbrd.h} x  "0" 54 {} {} {} {} {} {} {} {} {36} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID36 {} 0 bool
addvar RREG_ID37$i $i {x_configbrd.h} x  "0" 55 {} {} {} {} {} {} {} {} {37} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID37 {} 0 bool
addvar RREG_ID38$i $i {x_configbrd.h} x  "0" 56 {} {} {} {} {} {} {} {} {38} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID38 {} 0 bool
addvar RREG_ID39$i $i {x_configbrd.h} x  "0" 57 {} {} {} {} {} {} {} {} {39} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID39 {} 0 bool
addvar RREG_ID3A$i $i {x_configbrd.h} x  "0" 58 {} {} {} {} {} {} {} {} {3A} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID3A {} 0 bool
addvar RREG_ID3B$i $i {x_configbrd.h} x  "0" 59 {} {} {} {} {} {} {} {} {3B} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID3B {} 0 bool
addvar RREG_ID3C$i $i {x_configbrd.h} x  "0" 60 {} {} {} {} {} {} {} {} {3C} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID3C {} 0 bool
addvar RREG_ID3D$i $i {x_configbrd.h} x  "0" 61 {} {} {} {} {} {} {} {} {3D} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID3D {} 0 bool
addvar RREG_ID3E$i $i {x_configbrd.h} x  "0" 62 {} {} {} {} {} {} {} {} {3E} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID3E {} 0 bool
addvar RREG_ID3F$i $i {x_configbrd.h} x  "0" 63 {} {} {} {} {} {} {} {} {3F} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID3F {} 0 bool
addvar RREG_ID40$i $i {x_configbrd.h} x  "0" 64 {} {} {} {} {} {} {} {} {40} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID40 {} 0 bool
addvar RREG_ID41$i $i {x_configbrd.h} x  "0" 65 {} {} {} {} {} {} {} {} {41} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID41 {} 0 bool
addvar RREG_ID42$i $i {x_configbrd.h} x  "0" 66 {} {} {} {} {} {} {} {} {42} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID42 {} 0 bool
addvar RREG_ID43$i $i {x_configbrd.h} x  "0" 67 {} {} {} {} {} {} {} {} {43} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID43 {} 0 bool
addvar RREG_ID44$i $i {x_configbrd.h} x  "0" 68 {} {} {} {} {} {} {} {} {44} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID44 {} 0 bool
addvar RREG_ID45$i $i {x_configbrd.h} x  "0" 69 {} {} {} {} {} {} {} {} {45} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID45 {} 0 bool
addvar RREG_ID46$i $i {x_configbrd.h} x  "0" 70 {} {} {} {} {} {} {} {} {46} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID46 {} 0 bool
addvar RREG_ID47$i $i {x_configbrd.h} x  "0" 71 {} {} {} {} {} {} {} {} {47} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID47 {} 0 bool
addvar RREG_ID48$i $i {x_configbrd.h} x  "0" 72 {} {} {} {} {} {} {} {} {48} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID48 {} 0 bool
addvar RREG_ID49$i $i {x_configbrd.h} x  "0" 73 {} {} {} {} {} {} {} {} {49} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID49 {} 0 bool
addvar RREG_ID4A$i $i {x_configbrd.h} x  "0" 74 {} {} {} {} {} {} {} {} {4A} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID4A {} 0 bool
addvar RREG_ID4B$i $i {x_configbrd.h} x  "0" 75 {} {} {} {} {} {} {} {} {4B} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID4B {} 0 bool
addvar RREG_ID4C$i $i {x_configbrd.h} x  "0" 76 {} {} {} {} {} {} {} {} {4C} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID4C {} 0 bool
addvar RREG_ID4D$i $i {x_configbrd.h} x  "0" 77 {} {} {} {} {} {} {} {} {4D} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID4D {} 0 bool
addvar RREG_ID4E$i $i {x_configbrd.h} x  "0" 78 {} {} {} {} {} {} {} {} {4E} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID4E {} 0 bool
addvar RREG_ID4F$i $i {x_configbrd.h} x  "0" 79 {} {} {} {} {} {} {} {} {4F} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID4F {} 0 bool
addvar RREG_ID50$i $i {x_configbrd.h} x  "0" 80 {} {} {} {} {} {} {} {} {50} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID50 {} 0 bool
addvar RREG_ID51$i $i {x_configbrd.h} x  "0" 81 {} {} {} {} {} {} {} {} {51} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID51 {} 0 bool
addvar RREG_ID52$i $i {x_configbrd.h} x  "0" 82 {} {} {} {} {} {} {} {} {52} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID52 {} 0 bool
addvar RREG_ID53$i $i {x_configbrd.h} x  "0" 83 {} {} {} {} {} {} {} {} {53} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID53 {} 0 bool
addvar RREG_ID54$i $i {x_configbrd.h} x  "0" 84 {} {} {} {} {} {} {} {} {54} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID54 {} 0 bool
addvar RREG_ID55$i $i {x_configbrd.h} x  "0" 85 {} {} {} {} {} {} {} {} {55} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID55 {} 0 bool
addvar RREG_ID56$i $i {x_configbrd.h} x  "0" 86 {} {} {} {} {} {} {} {} {56} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID56 {} 0 bool
addvar RREG_ID57$i $i {x_configbrd.h} x  "0" 87 {} {} {} {} {} {} {} {} {57} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID57 {} 0 bool
addvar RREG_ID58$i $i {x_configbrd.h} x  "0" 88 {} {} {} {} {} {} {} {} {58} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID58 {} 0 bool
addvar RREG_ID59$i $i {x_configbrd.h} x  "0" 89 {} {} {} {} {} {} {} {} {59} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID59 {} 0 bool
addvar RREG_ID5A$i $i {x_configbrd.h} x  "0" 90 {} {} {} {} {} {} {} {} {5A} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID5A {} 0 bool
addvar RREG_ID5B$i $i {x_configbrd.h} x  "0" 91 {} {} {} {} {} {} {} {} {5B} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID5B {} 0 bool
addvar RREG_ID5C$i $i {x_configbrd.h} x  "0" 92 {} {} {} {} {} {} {} {} {5C} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID5C {} 0 bool
addvar RREG_ID5D$i $i {x_configbrd.h} x  "0" 93 {} {} {} {} {} {} {} {} {5D} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID5D {} 0 bool
addvar RREG_ID5E$i $i {x_configbrd.h} x  "0" 94 {} {} {} {} {} {} {} {} {5E} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID5E {} 0 bool
addvar RREG_ID5F$i $i {x_configbrd.h} x  "0" 95 {} {} {} {} {} {} {} {} {5F} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID5F {} 0 bool
addvar RREG_ID60$i $i {x_configbrd.h} x  "0" 96 {} {} {} {} {} {} {} {} {60} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID60 {} 0 bool
addvar RREG_ID61$i $i {x_configbrd.h} x  "0" 97 {} {} {} {} {} {} {} {} {61} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID61 {} 0 bool
addvar RREG_ID62$i $i {x_configbrd.h} x  "0" 98 {} {} {} {} {} {} {} {} {62} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID62 {} 0 bool
addvar RREG_ID63$i $i {x_configbrd.h} x  "0" 99 {} {} {} {} {} {} {} {} {63} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID63 {} 0 bool
addvar RREG_ID64$i $i {x_configbrd.h} x  "0" 100 {} {} {} {} {} {} {} {} {64} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID64 {} 0 bool
addvar RREG_ID65$i $i {x_configbrd.h} x  "0" 101 {} {} {} {} {} {} {} {} {65} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID65 {} 0 bool
addvar RREG_ID66$i $i {x_configbrd.h} x  "0" 102 {} {} {} {} {} {} {} {} {66} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID66 {} 0 bool
addvar RREG_ID67$i $i {x_configbrd.h} x  "0" 103 {} {} {} {} {} {} {} {} {67} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID67 {} 0 bool
addvar RREG_ID68$i $i {x_configbrd.h} x  "0" 104 {} {} {} {} {} {} {} {} {68} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID68 {} 0 bool
addvar RREG_ID69$i $i {x_configbrd.h} x  "0" 105 {} {} {} {} {} {} {} {} {69} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID69 {} 0 bool
addvar RREG_ID6A$i $i {x_configbrd.h} x  "0" 106 {} {} {} {} {} {} {} {} {6A} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID6A {} 0 bool
addvar RREG_ID6B$i $i {x_configbrd.h} x  "0" 107 {} {} {} {} {} {} {} {} {6B} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID6B {} 0 bool
addvar RREG_ID6C$i $i {x_configbrd.h} x  "0" 108 {} {} {} {} {} {} {} {} {6C} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID6C {} 0 bool
addvar RREG_ID6D$i $i {x_configbrd.h} x  "0" 109 {} {} {} {} {} {} {} {} {6D} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID6D {} 0 bool
addvar RREG_ID6E$i $i {x_configbrd.h} x  "0" 110 {} {} {} {} {} {} {} {} {6E} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID6E {} 0 bool
addvar RREG_ID6F$i $i {x_configbrd.h} x  "0" 111 {} {} {} {} {} {} {} {} {6F} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID6F {} 0 bool
addvar RREG_ID70$i $i {x_configbrd.h} x  "0" 112 {} {} {} {} {} {} {} {} {70} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID70 {} 0 bool
addvar RREG_ID71$i $i {x_configbrd.h} x  "0" 113 {} {} {} {} {} {} {} {} {71} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID71 {} 0 bool
addvar RREG_ID72$i $i {x_configbrd.h} x  "0" 114 {} {} {} {} {} {} {} {} {72} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID72 {} 0 bool
addvar RREG_ID73$i $i {x_configbrd.h} x  "0" 115 {} {} {} {} {} {} {} {} {73} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID73 {} 0 bool
addvar RREG_ID74$i $i {x_configbrd.h} x  "0" 116 {} {} {} {} {} {} {} {} {74} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID74 {} 0 bool
addvar RREG_ID75$i $i {x_configbrd.h} x  "0" 117 {} {} {} {} {} {} {} {} {75} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID75 {} 0 bool
addvar RREG_ID76$i $i {x_configbrd.h} x  "0" 118 {} {} {} {} {} {} {} {} {76} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID76 {} 0 bool
addvar RREG_ID77$i $i {x_configbrd.h} x  "0" 119 {} {} {} {} {} {} {} {} {77} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID77 {} 0 bool
addvar RREG_ID78$i $i {x_configbrd.h} x  "0" 120 {} {} {} {} {} {} {} {} {78} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID78 {} 0 bool
addvar RREG_ID79$i $i {x_configbrd.h} x  "0" 121 {} {} {} {} {} {} {} {} {79} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID79 {} 0 bool
addvar RREG_ID7A$i $i {x_configbrd.h} x  "0" 122 {} {} {} {} {} {} {} {} {7A} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID7A {} 0 bool
addvar RREG_ID7B$i $i {x_configbrd.h} x  "0" 123 {} {} {} {} {} {} {} {} {7B} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID7B {} 0 bool
addvar RREG_ID7C$i $i {x_configbrd.h} x  "0" 124 {} {} {} {} {} {} {} {} {7C} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID7C {} 0 bool
addvar RREG_ID7D$i $i {x_configbrd.h} x  "0" 125 {} {} {} {} {} {} {} {} {7D} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID7D {} 0 bool
addvar RREG_ID7E$i $i {x_configbrd.h} x  "0" 126 {} {} {} {} {} {} {} {} {7E} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID7E {} 0 bool
addvar RREG_ID7F$i $i {x_configbrd.h} x  "0" 127 {} {} {} {} {} {} {} {} {7F} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID7F {} 0 bool
addvar RREG_ID80$i $i {x_configbrd.h} x  "0" 128 {} {} {} {} {} {} {} {} {80} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID80 {} 0 bool
addvar RREG_ID81$i $i {x_configbrd.h} x  "0" 129 {} {} {} {} {} {} {} {} {81} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID81 {} 0 bool
addvar RREG_ID82$i $i {x_configbrd.h} x  "0" 130 {} {} {} {} {} {} {} {} {82} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID82 {} 0 bool
addvar RREG_ID83$i $i {x_configbrd.h} x  "0" 131 {} {} {} {} {} {} {} {} {83} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID83 {} 0 bool
addvar RREG_ID84$i $i {x_configbrd.h} x  "0" 132 {} {} {} {} {} {} {} {} {84} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID84 {} 0 bool
addvar RREG_ID85$i $i {x_configbrd.h} x  "0" 133 {} {} {} {} {} {} {} {} {85} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID85 {} 0 bool
addvar RREG_ID86$i $i {x_configbrd.h} x  "0" 134 {} {} {} {} {} {} {} {} {86} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID86 {} 0 bool
addvar RREG_ID87$i $i {x_configbrd.h} x  "0" 135 {} {} {} {} {} {} {} {} {87} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID87 {} 0 bool
addvar RREG_ID88$i $i {x_configbrd.h} x  "0" 136 {} {} {} {} {} {} {} {} {88} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID88 {} 0 bool
addvar RREG_ID89$i $i {x_configbrd.h} x  "0" 137 {} {} {} {} {} {} {} {} {89} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID89 {} 0 bool
addvar RREG_ID8A$i $i {x_configbrd.h} x  "0" 138 {} {} {} {} {} {} {} {} {8A} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID8A {} 0 bool
addvar RREG_ID8B$i $i {x_configbrd.h} x  "0" 139 {} {} {} {} {} {} {} {} {8B} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID8B {} 0 bool
addvar RREG_ID8C$i $i {x_configbrd.h} x  "0" 140 {} {} {} {} {} {} {} {} {8C} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID8C {} 0 bool
addvar RREG_ID8D$i $i {x_configbrd.h} x  "0" 141 {} {} {} {} {} {} {} {} {8D} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID8D {} 0 bool
addvar RREG_ID8E$i $i {x_configbrd.h} x  "0" 142 {} {} {} {} {} {} {} {} {8E} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID8E {} 0 bool
addvar RREG_ID8F$i $i {x_configbrd.h} x  "0" 143 {} {} {} {} {} {} {} {} {8F} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID8F {} 0 bool
addvar RREG_ID90$i $i {x_configbrd.h} x  "0" 144 {} {} {} {} {} {} {} {} {90} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID90 {} 0 bool
addvar RREG_ID91$i $i {x_configbrd.h} x  "0" 145 {} {} {} {} {} {} {} {} {91} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID91 {} 0 bool
addvar RREG_ID92$i $i {x_configbrd.h} x  "0" 146 {} {} {} {} {} {} {} {} {92} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID92 {} 0 bool
addvar RREG_ID93$i $i {x_configbrd.h} x  "0" 147 {} {} {} {} {} {} {} {} {93} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID93 {} 0 bool
addvar RREG_ID94$i $i {x_configbrd.h} x  "0" 148 {} {} {} {} {} {} {} {} {94} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID94 {} 0 bool
addvar RREG_ID95$i $i {x_configbrd.h} x  "0" 149 {} {} {} {} {} {} {} {} {95} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID95 {} 0 bool
addvar RREG_ID96$i $i {x_configbrd.h} x  "0" 150 {} {} {} {} {} {} {} {} {96} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID96 {} 0 bool
addvar RREG_ID97$i $i {x_configbrd.h} x  "0" 151 {} {} {} {} {} {} {} {} {97} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID97 {} 0 bool
addvar RREG_ID98$i $i {x_configbrd.h} x  "0" 152 {} {} {} {} {} {} {} {} {98} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID98 {} 0 bool
addvar RREG_ID99$i $i {x_configbrd.h} x  "0" 153 {} {} {} {} {} {} {} {} {99} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID99 {} 0 bool
addvar RREG_ID9A$i $i {x_configbrd.h} x  "0" 154 {} {} {} {} {} {} {} {} {9A} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID9A {} 0 bool
addvar RREG_ID9B$i $i {x_configbrd.h} x  "0" 155 {} {} {} {} {} {} {} {} {9B} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID9B {} 0 bool
addvar RREG_ID9C$i $i {x_configbrd.h} x  "0" 156 {} {} {} {} {} {} {} {} {9C} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID9C {} 0 bool
addvar RREG_ID9D$i $i {x_configbrd.h} x  "0" 157 {} {} {} {} {} {} {} {} {9D} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID9D {} 0 bool
addvar RREG_ID9E$i $i {x_configbrd.h} x  "0" 158 {} {} {} {} {} {} {} {} {9E} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID9E {} 0 bool
addvar RREG_ID9F$i $i {x_configbrd.h} x  "0" 159 {} {} {} {} {} {} {} {} {9F} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_ID9F {} 0 bool
addvar RREG_IDA0$i $i {x_configbrd.h} x  "0" 160 {} {} {} {} {} {} {} {} {A0} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDA0 {} 0 bool
addvar RREG_IDA1$i $i {x_configbrd.h} x  "0" 161 {} {} {} {} {} {} {} {} {A1} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDA1 {} 0 bool
addvar RREG_IDA2$i $i {x_configbrd.h} x  "0" 162 {} {} {} {} {} {} {} {} {A2} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDA2 {} 0 bool
addvar RREG_IDA3$i $i {x_configbrd.h} x  "0" 163 {} {} {} {} {} {} {} {} {A3} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDA3 {} 0 bool
addvar RREG_IDA4$i $i {x_configbrd.h} x  "0" 164 {} {} {} {} {} {} {} {} {A4} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDA4 {} 0 bool
addvar RREG_IDA5$i $i {x_configbrd.h} x  "0" 165 {} {} {} {} {} {} {} {} {A5} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDA5 {} 0 bool
addvar RREG_IDA6$i $i {x_configbrd.h} x  "0" 166 {} {} {} {} {} {} {} {} {A6} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDA6 {} 0 bool
addvar RREG_IDA7$i $i {x_configbrd.h} x  "0" 167 {} {} {} {} {} {} {} {} {A7} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDA7 {} 0 bool
addvar RREG_IDA8$i $i {x_configbrd.h} x  "0" 168 {} {} {} {} {} {} {} {} {A8} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDA8 {} 0 bool
addvar RREG_IDA9$i $i {x_configbrd.h} x  "0" 169 {} {} {} {} {} {} {} {} {A9} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDA9 {} 0 bool
addvar RREG_IDAA$i $i {x_configbrd.h} x  "0" 170 {} {} {} {} {} {} {} {} {AA} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDAA {} 0 bool
addvar RREG_IDAB$i $i {x_configbrd.h} x  "0" 171 {} {} {} {} {} {} {} {} {AB} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDAB {} 0 bool
addvar RREG_IDAC$i $i {x_configbrd.h} x  "0" 172 {} {} {} {} {} {} {} {} {AC} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDAC {} 0 bool
addvar RREG_IDAD$i $i {x_configbrd.h} x  "0" 173 {} {} {} {} {} {} {} {} {AD} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDAD {} 0 bool
addvar RREG_IDAE$i $i {x_configbrd.h} x  "0" 174 {} {} {} {} {} {} {} {} {AE} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDAE {} 0 bool
addvar RREG_IDAF$i $i {x_configbrd.h} x  "0" 175 {} {} {} {} {} {} {} {} {AF} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDAF {} 0 bool
addvar RREG_IDB0$i $i {x_configbrd.h} x  "0" 176 {} {} {} {} {} {} {} {} {B0} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDB0 {} 0 bool
addvar RREG_IDB1$i $i {x_configbrd.h} x  "0" 177 {} {} {} {} {} {} {} {} {B1} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDB1 {} 0 bool
addvar RREG_IDB2$i $i {x_configbrd.h} x  "0" 178 {} {} {} {} {} {} {} {} {B2} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDB2 {} 0 bool
addvar RREG_IDB3$i $i {x_configbrd.h} x  "0" 179 {} {} {} {} {} {} {} {} {B3} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDB3 {} 0 bool
addvar RREG_IDB4$i $i {x_configbrd.h} x  "0" 180 {} {} {} {} {} {} {} {} {B4} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDB4 {} 0 bool
addvar RREG_IDB5$i $i {x_configbrd.h} x  "0" 181 {} {} {} {} {} {} {} {} {B5} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDB5 {} 0 bool
addvar RREG_IDB6$i $i {x_configbrd.h} x  "0" 182 {} {} {} {} {} {} {} {} {B6} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDB6 {} 0 bool
addvar RREG_IDB7$i $i {x_configbrd.h} x  "0" 183 {} {} {} {} {} {} {} {} {B7} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDB7 {} 0 bool
addvar RREG_IDB8$i $i {x_configbrd.h} x  "0" 184 {} {} {} {} {} {} {} {} {B8} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDB8 {} 0 bool
addvar RREG_IDB9$i $i {x_configbrd.h} x  "0" 185 {} {} {} {} {} {} {} {} {B9} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDB9 {} 0 bool
addvar RREG_IDBA$i $i {x_configbrd.h} x  "0" 186 {} {} {} {} {} {} {} {} {BA} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDBA {} 0 bool
addvar RREG_IDBB$i $i {x_configbrd.h} x  "0" 187 {} {} {} {} {} {} {} {} {BB} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDBB {} 0 bool
addvar RREG_IDBC$i $i {x_configbrd.h} x  "0" 188 {} {} {} {} {} {} {} {} {BC} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDBC {} 0 bool
addvar RREG_IDBD$i $i {x_configbrd.h} x  "0" 189 {} {} {} {} {} {} {} {} {BD} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDBD {} 0 bool
addvar RREG_IDBE$i $i {x_configbrd.h} x  "0" 190 {} {} {} {} {} {} {} {} {BE} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDBE {} 0 bool
addvar RREG_IDBF$i $i {x_configbrd.h} x  "0" 191 {} {} {} {} {} {} {} {} {BF} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDBF {} 0 bool
addvar RREG_IDC0$i $i {x_configbrd.h} x  "0" 192 {} {} {} {} {} {} {} {} {C0} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDC0 {} 0 bool
addvar RREG_IDC1$i $i {x_configbrd.h} x  "0" 193 {} {} {} {} {} {} {} {} {C1} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDC1 {} 0 bool
addvar RREG_IDC2$i $i {x_configbrd.h} x  "0" 194 {} {} {} {} {} {} {} {} {C2} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDC2 {} 0 bool
addvar RREG_IDC3$i $i {x_configbrd.h} x  "0" 195 {} {} {} {} {} {} {} {} {C3} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDC3 {} 0 bool
addvar RREG_IDC4$i $i {x_configbrd.h} x  "0" 196 {} {} {} {} {} {} {} {} {C4} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDC4 {} 0 bool
addvar RREG_IDC5$i $i {x_configbrd.h} x  "0" 197 {} {} {} {} {} {} {} {} {C5} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDC5 {} 0 bool
addvar RREG_IDC6$i $i {x_configbrd.h} x  "0" 198 {} {} {} {} {} {} {} {} {C6} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDC6 {} 0 bool
addvar RREG_IDC7$i $i {x_configbrd.h} x  "0" 199 {} {} {} {} {} {} {} {} {C7} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDC7 {} 0 bool
addvar RREG_IDC8$i $i {x_configbrd.h} x  "0" 200 {} {} {} {} {} {} {} {} {C8} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDC8 {} 0 bool
addvar RREG_IDC9$i $i {x_configbrd.h} x  "0" 201 {} {} {} {} {} {} {} {} {C9} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDC9 {} 0 bool
addvar RREG_IDCA$i $i {x_configbrd.h} x  "0" 202 {} {} {} {} {} {} {} {} {CA} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDCA {} 0 bool
addvar RREG_IDCB$i $i {x_configbrd.h} x  "0" 203 {} {} {} {} {} {} {} {} {CB} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDCB {} 0 bool
addvar RREG_IDCC$i $i {x_configbrd.h} x  "0" 204 {} {} {} {} {} {} {} {} {CC} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDCC {} 0 bool
addvar RREG_IDCD$i $i {x_configbrd.h} x  "0" 205 {} {} {} {} {} {} {} {} {CD} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDCD {} 0 bool
addvar RREG_IDCE$i $i {x_configbrd.h} x  "0" 206 {} {} {} {} {} {} {} {} {CE} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDCE {} 0 bool
addvar RREG_IDCF$i $i {x_configbrd.h} x  "0" 207 {} {} {} {} {} {} {} {} {CF} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDCF {} 0 bool
addvar RREG_IDD0$i $i {x_configbrd.h} x  "0" 208 {} {} {} {} {} {} {} {} {D0} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDD0 {} 0 bool
addvar RREG_IDD1$i $i {x_configbrd.h} x  "0" 209 {} {} {} {} {} {} {} {} {D1} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDD1 {} 0 bool
addvar RREG_IDD2$i $i {x_configbrd.h} x  "0" 210 {} {} {} {} {} {} {} {} {D2} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDD2 {} 0 bool
addvar RREG_IDD3$i $i {x_configbrd.h} x  "0" 211 {} {} {} {} {} {} {} {} {D3} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDD3 {} 0 bool
addvar RREG_IDD4$i $i {x_configbrd.h} x  "0" 212 {} {} {} {} {} {} {} {} {D4} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDD4 {} 0 bool
addvar RREG_IDD5$i $i {x_configbrd.h} x  "0" 213 {} {} {} {} {} {} {} {} {D5} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDD5 {} 0 bool
addvar RREG_IDD6$i $i {x_configbrd.h} x  "0" 214 {} {} {} {} {} {} {} {} {D6} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDD6 {} 0 bool
addvar RREG_IDD7$i $i {x_configbrd.h} x  "0" 215 {} {} {} {} {} {} {} {} {D7} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDD7 {} 0 bool
addvar RREG_IDD8$i $i {x_configbrd.h} x  "0" 216 {} {} {} {} {} {} {} {} {D8} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDD8 {} 0 bool
addvar RREG_IDD9$i $i {x_configbrd.h} x  "0" 217 {} {} {} {} {} {} {} {} {D9} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDD9 {} 0 bool
addvar RREG_IDDA$i $i {x_configbrd.h} x  "0" 218 {} {} {} {} {} {} {} {} {DA} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDDA {} 0 bool
addvar RREG_IDDB$i $i {x_configbrd.h} x  "0" 219 {} {} {} {} {} {} {} {} {DB} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDDB {} 0 bool
addvar RREG_IDDC$i $i {x_configbrd.h} x  "0" 220 {} {} {} {} {} {} {} {} {DC} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDDC {} 0 bool
addvar RREG_IDDD$i $i {x_configbrd.h} x  "0" 221 {} {} {} {} {} {} {} {} {DD} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDDD {} 0 bool
addvar RREG_IDDE$i $i {x_configbrd.h} x  "0" 222 {} {} {} {} {} {} {} {} {DE} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDDE {} 0 bool
addvar RREG_IDDF$i $i {x_configbrd.h} x  "0" 223 {} {} {} {} {} {} {} {} {DF} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDDF {} 0 bool
addvar RREG_IDE0$i $i {x_configbrd.h} x  "0" 224 {} {} {} {} {} {} {} {} {E0} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDE0 {} 0 bool
addvar RREG_IDE1$i $i {x_configbrd.h} x  "0" 225 {} {} {} {} {} {} {} {} {E1} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDE1 {} 0 bool
addvar RREG_IDE2$i $i {x_configbrd.h} x  "0" 226 {} {} {} {} {} {} {} {} {E2} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDE2 {} 0 bool
addvar RREG_IDE3$i $i {x_configbrd.h} x  "0" 227 {} {} {} {} {} {} {} {} {E3} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDE3 {} 0 bool
addvar RREG_IDE4$i $i {x_configbrd.h} x  "0" 228 {} {} {} {} {} {} {} {} {E4} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDE4 {} 0 bool
addvar RREG_IDE5$i $i {x_configbrd.h} x  "0" 229 {} {} {} {} {} {} {} {} {E5} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDE5 {} 0 bool
addvar RREG_IDE6$i $i {x_configbrd.h} x  "0" 230 {} {} {} {} {} {} {} {} {E6} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDE6 {} 0 bool
addvar RREG_IDE7$i $i {x_configbrd.h} x  "0" 231 {} {} {} {} {} {} {} {} {E7} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDE7 {} 0 bool
addvar RREG_IDE8$i $i {x_configbrd.h} x  "0" 232 {} {} {} {} {} {} {} {} {E8} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDE8 {} 0 bool
addvar RREG_IDE9$i $i {x_configbrd.h} x  "0" 233 {} {} {} {} {} {} {} {} {E9} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDE9 {} 0 bool
addvar RREG_IDEA$i $i {x_configbrd.h} x  "0" 234 {} {} {} {} {} {} {} {} {EA} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDEA {} 0 bool
addvar RREG_IDEB$i $i {x_configbrd.h} x  "0" 235 {} {} {} {} {} {} {} {} {EB} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDEB {} 0 bool
addvar RREG_IDEC$i $i {x_configbrd.h} x  "0" 236 {} {} {} {} {} {} {} {} {EC} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDEC {} 0 bool
addvar RREG_IDED$i $i {x_configbrd.h} x  "0" 237 {} {} {} {} {} {} {} {} {ED} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDED {} 0 bool
addvar RREG_IDEE$i $i {x_configbrd.h} x  "0" 238 {} {} {} {} {} {} {} {} {EE} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDEE {} 0 bool
addvar RREG_IDEF$i $i {x_configbrd.h} x  "0" 239 {} {} {} {} {} {} {} {} {EF} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDEF {} 0 bool
addvar RREG_IDF0$i $i {x_configbrd.h} x  "0" 240 {} {} {} {} {} {} {} {} {F0} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDF0 {} 0 bool
addvar RREG_IDF1$i $i {x_configbrd.h} x  "0" 241 {} {} {} {} {} {} {} {} {F1} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDF1 {} 0 bool
addvar RREG_IDF2$i $i {x_configbrd.h} x  "0" 242 {} {} {} {} {} {} {} {} {F2} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDF2 {} 0 bool
addvar RREG_IDF3$i $i {x_configbrd.h} x  "0" 243 {} {} {} {} {} {} {} {} {F3} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDF3 {} 0 bool
addvar RREG_IDF4$i $i {x_configbrd.h} x  "0" 244 {} {} {} {} {} {} {} {} {F4} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDF4 {} 0 bool
addvar RREG_IDF5$i $i {x_configbrd.h} x  "0" 245 {} {} {} {} {} {} {} {} {F5} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDF5 {} 0 bool
addvar RREG_IDF6$i $i {x_configbrd.h} x  "0" 246 {} {} {} {} {} {} {} {} {F6} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDF6 {} 0 bool
addvar RREG_IDF7$i $i {x_configbrd.h} x  "0" 247 {} {} {} {} {} {} {} {} {F7} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDF7 {} 0 bool
addvar RREG_IDF8$i $i {x_configbrd.h} x  "0" 248 {} {} {} {} {} {} {} {} {F8} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDF8 {} 0 bool
addvar RREG_IDF9$i $i {x_configbrd.h} x  "0" 249 {} {} {} {} {} {} {} {} {F9} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDF9 {} 0 bool
addvar RREG_IDFA$i $i {x_configbrd.h} x  "0" 250 {} {} {} {} {} {} {} {} {FA} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDFA {} 0 bool
addvar RREG_IDFB$i $i {x_configbrd.h} x  "0" 251 {} {} {} {} {} {} {} {} {FB} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDFB {} 0 bool
addvar RREG_IDFC$i $i {x_configbrd.h} x  "0" 252 {} {} {} {} {} {} {} {} {FC} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDFC {} 0 bool
addvar RREG_IDFD$i $i {x_configbrd.h} x  "0" 253 {} {} {} {} {} {} {} {} {FD} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDFD {} 0 bool
addvar RREG_IDFE$i $i {x_configbrd.h} x  "0" 254 {} {} {} {} {} {} {} {} {FE} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDFE {} 0 bool
addvar RREG_IDFF$i $i {x_configbrd.h} x  "0" 255 {} {} {} {} {} {} {} {} {FF} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RREG.RREG_IDFF {} 0 bool
addvar RREG_TIMEOUT$i $i {x_configbrd.h} i  "5" 3 {$INCLUDE_RREG==1} {} {} {} {} {} {} {} {Registration timeout} __BOARD_CONFIG._RIO_CONFIG.RREG_TIMEOUT {} 0 int
addvar RREG_THREAD_PRI$i $i {x_configbrd.h} i  "220" 4 {$INCLUDE_RREG==1} {} {} {} {} {} {} {} {Registration thread priority} __BOARD_CONFIG._RIO_CONFIG.RREG_THREAD_PRI {} 0 int
addvar RREG_ID_MAX$i $i {x_configbrd.h} i  "32" 5 {$INCLUDE_RREG==1} {} {} {[range $P 2 256]} {} {} {} {} {Registration ID limit} __BOARD_CONFIG._RIO_CONFIG.RREG_ID_MAX {} 0 int
addvar INCLUDE_RMSG_NET$i $i {x_configbrd.h} u  "0" 6 {$INCLUDE_RREG==1} {} {} {} {} {} {} {} {Ethernet over RIO} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RMSG_NET {} 0 bool
addvar RMSG_NET_RX_SIZE$i $i {x_configbrd.h} i  "256" 0 {} {} {} {} {} {} {} {} {Number of receive network buffers} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RMSG_NET.RMSG_NET_RX_SIZE {} 0 int
addvar RMSG_CHAN_RX_SIZE$i $i {x_configbrd.h} i  "256" 1 {$INCLUDE_CHANNELS==1} {} {} {} {} {} {} {} {Number of receive mpchan buffers} __BOARD_CONFIG._RIO_CONFIG.INCLUDE_RMSG_NET.RMSG_CHAN_RX_SIZE {} 0 int
addfile "x_configbrd.h"
proc external_gentree {ns} {
if !$::x_enabled return
global tree h img1 img2 img3
$h add "__BOARD_CONFIG" -itemtype imagetext -text "[getname __BOARD_CONFIG]" -image $img2 -data ${ns}::dialog___BOARD_CONFIG 
$h add "__BOARD_CONFIG.__AUX" -itemtype imagetext -text "[getname __AUX]" -image $img1 -data ${ns}::dialog___AUX 
$h hide entry "__BOARD_CONFIG.__AUX"
$h add "__BOARD_CONFIG.__AUX.AUX_CONFIG" -itemtype imagetext -text "[getname AUX_CONFIG]" -image $img1 -data ${ns}::dialog_AUX_CONFIG
$h add "__BOARD_CONFIG.__DE_CONFIG" -itemtype imagetext -text "[getname __DE_CONFIG]" -image $img1 -data ${ns}::dialog___DE_CONFIG 
$h hide entry "__BOARD_CONFIG.__DE_CONFIG"
$h add "__BOARD_CONFIG.__RGB_CONFIG" -itemtype imagetext -text "[getname __RGB_CONFIG]" -image $img1 -data ${ns}::dialog___RGB_CONFIG 
$h hide entry "__BOARD_CONFIG.__RGB_CONFIG"
$h add "__BOARD_CONFIG.__IDE_CONFIG" -itemtype imagetext -text "[getname __IDE_CONFIG]" -image $img1 -data ${ns}::dialog___IDE_CONFIG 
$h hide entry "__BOARD_CONFIG.__IDE_CONFIG"
$h add "__BOARD_CONFIG.__IDE_CONFIG.__INCLUDE_BTM405_IDE" -itemtype imagetext -text "[getname __INCLUDE_BTM405_IDE]" -image $img1 -data ${ns}::dialog___INCLUDE_BTM405_IDE 
$h hide entry "__BOARD_CONFIG.__IDE_CONFIG.__INCLUDE_BTM405_IDE"
$h add "__BOARD_CONFIG.__SCSI_CONFIG" -itemtype imagetext -text "[getname __SCSI_CONFIG]" -image $img1 -data ${ns}::dialog___SCSI_CONFIG 
$h hide entry "__BOARD_CONFIG.__SCSI_CONFIG"
$h add "__BOARD_CONFIG.__SCSI_CONFIG.SCSI_AUTO_CONFIG" -itemtype imagetext -text "[getname SCSI_AUTO_CONFIG]" -image $img1 -data ${ns}::dialog_SCSI_AUTO_CONFIG 
$h hide entry "__BOARD_CONFIG.__SCSI_CONFIG.SCSI_AUTO_CONFIG"
$h add "__BOARD_CONFIG.__SCSI_CONFIG.SCSI_MAN_CONFIG" -itemtype imagetext -text "[getname SCSI_MAN_CONFIG]" -image $img1 -data ${ns}::dialog_SCSI_MAN_CONFIG
$h hide entry "__BOARD_CONFIG.__SCSI_CONFIG.SCSI_MAN_CONFIG"
$h add "__BOARD_CONFIG.__BSP_MISC" -itemtype imagetext -text "[getname __BSP_MISC]" -image $img1 -data ${ns}::dialog___BSP_MISC 
$h hide entry "__BOARD_CONFIG.__BSP_MISC"
$h add "__BOARD_CONFIG._RIO_CONFIG" -itemtype imagetext -text "[getname _RIO_CONFIG]" -image $img1 -data ${ns}::dialog__RIO_CONFIG 
$h hide entry "__BOARD_CONFIG._RIO_CONFIG"
$tree autosetmode
$h selection set __BOARD_CONFIG
}
