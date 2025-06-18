/*
 * Функции внешних библиотек, (С), Als, 03/2021
 */
.macro set_api name s
.globl \name
\name:
	syscall	\s
	jr	$ra
	nop
.endm

.text

set_api	uemu_ret_main	0x2020
set_api	__fread_chk	0x2021
set_api	__printf_chk	0x2023
set_api	calloc		0x2033
set_api	clock		0x2036
set_api	exit		0x203d
set_api	fclose		0x2041
set_api	fflush		0x2045
set_api	fopen		0x2052
set_api	fread		0x2059
set_api	free		0x205a
set_api	gets		0x206a
set_api	malloc		0x2091
set_api	memcpy		0x2098
set_api	memset		0x209a
set_api	printf		0x209f
set_api	putchar		0x20a1
set_api	puts		0x20a2
set_api	syncos		0x20b2
set_api	sqrt		0x20b6
set_api	strlen		0x20c1
# заглушки для теста
set_api	tan		0x2000
set_api	atan		0x2000
set_api	cos		0x2000
set_api	cosf		0x2000
set_api	sin		0x2000
set_api	sinf		0x2000
set_api	acos		0x2000
set_api	atan2		0x2000
set_api	atan2f		0x2000
set_api	asin		0x2000
set_api	sqrtf		0x2000
