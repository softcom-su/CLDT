#
# Шаблоны мертвого кода из одной команды
#	F - свободный регистр
#	R - регистр общего назначения из списка DR[]
#	N - целое число в интервале 1..32767
#	X - целое число в интервале 1..31
#

DR = ["$a0","$a1","$a2","$a3","$v0","$v1","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$t8","$t9","$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7"]

DC = [	["daddu",  "F", "R", "R"], \
	["daddu",  "F", "R", "R"], \
	["daddu",  "F", "R", "R"], \
	["daddu",  "F", "R", "R"], \
	["daddu",  "F", "R", "R"], \
	["daddiu", "F", "R", "N"], \
	["daddiu", "F", "R", "N"], \
	["daddiu", "F", "R", "N"], \
	["daddiu", "F", "R", "N"], \
	["daddiu", "F", "R", "N"], \
	["dsubu",  "F", "R", "R"], \
	["dsubu",  "F", "R", "R"], \
	["dsubu",  "F", "R", "R"], \
	["dsubu",  "F", "R", "R"], \
	["dsubu",  "F", "R", "R"], \
	["and",    "F", "R", "R"], \
	["andi",   "F", "R", "N"], \
	["or",     "F", "R", "R"], \
	["ori",    "F", "R", "N"], \
	["xor",    "F", "R", "R"], \
	["xori",   "F", "R", "N"], \
#	["sll",    "F", "R", "X"], \	# ### Проверить на К64, исправить Unpredictable и IntegerOverflov Exceptions в эмуляторе 
	["dsll",   "F", "R", "X"], \
	["dsll32", "F", "R", "X"], \
#	["srl",    "F", "R", "X"], \	# ### Проверить на К64, исправить Unpredictable и IntegerOverflov Exceptions в эмуляторе 
	["dsrl",   "F", "R", "X"], \
	["dsrl32", "F", "R", "X"]  ]
