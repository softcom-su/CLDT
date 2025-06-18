/*
 * Функции внешних библиотек v3, (C), als, 07/2023
 */
.macro pa_string	lab, st
	ln = 0
	.irpc ln, \st
		ln = ln+1
	.endr
.align 2
\lab:
	.byte ln
	.asciz "\st"
.endm

.macro set_api name
	.data
	.align	4
	pa_string	__estr_\name	\name
	.previous
// вызов диспетчера выносов
	.align	4
	.quad	__estr_\name
	.globl \name
	\name:
		syscall	12345
		jr	$ra
		nop
.endm

.macro api_set name s
.globl \name
\name:
	syscall	\s
	jr	$ra
	nop
.endm

.text
/*
 * Встроенные выносы, !!! не изменять !!!
 */
api_set		reset		0x2320			// встроенная функция завершения работы программы (exit напишут в выносах)
api_set		malloc		0x2321			// стандартные функции  malloc
api_set		calloc		0x2322			//						calloc
api_set		free		0x2323			//						free

.text
.include "emul.dv.h"
