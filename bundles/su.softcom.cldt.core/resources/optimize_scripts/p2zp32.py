#!/usr/bin/env python3
#
# Запутывание графа потока управления (отдельно для каждой функции) .p.json, (С), Als, 06/2021
# Версия для 32-битного адресного пространства
#
import sys, json, random, copy, os
sys.path.append(os.path.dirname(os.path.realpath(__file__)))
import fmipsproc as fmp

fmp.set_random()

def cmderr():
	print("\t" + sys.argv[0] + " in_file.p.json out_file.p.json")
	sys.exit(2)

if len(sys.argv) != 3: cmderr()

P  = json.loads(open(sys.argv[1], "r").read())
A  = P["attr"]
F  = P["code"]

if A["version"] != "mips.p.0.1":
	print("Неподдерживаемая версия файла " + sys.argv[1]); sys.exit(2)

L, D, x = fmp.stat01(F)

#	Для формирования адресов используются свободеые регистры из секции free
#	Регистры из секции frg остаются для внедрения мертвого кода в участки
# Последовательность обработки
#   Только для файлов, у которых
#	- есть ddinfo со значениями bss и data > 0
#   Только для функций, у которых
#	- больше 4 участков
#	- в секции free не меньше двух свободных регистров
#   Если у участка есть next и нет target, cmd и slot, тогда
#	- выбрать участок Х не равный lunum и next
#	- добавить участку cmd, slot и target

# Формирование данных для генерации дополнительных дуг
T = {}
if "free" in P and "ddinfo" in P and "bss" in P["ddinfo"] and P["ddinfo"]["bss"] > 0 and "data" in P["ddinfo"] and P["ddinfo"]["data"] > 0 :
	for i, lu in enumerate(F):
		f = lu["func"]
		if len(L[f]) > 4 and len(P["free"][f]) > 1:
			r = P["free"][f]
			if "next" in lu and "target" not in lu and "cmd" not in lu and "slot" not in lu:
				ln = lu["lunum"]
				while ln == lu["lunum"] or ln == lu["next"]:
					ln = L[f][random.randint(0, len(L[f])-1)]
#				rn = r[random.randint(0, len(r)-1)]
				rn = random.randint(0, len(r)-2)	# выбираем 2 последовательно расположенных регистра
				T[i] = [ln, rn]
# Генерация дополнительных дуг с ложными переходами
dd = 0
for lpn in T:
	dd += 1
	lu2 = copy.deepcopy(F[lpn])
	lu2["target"] = T[lpn][0]
#	lu2["slot"] = ["nop"]
	r1 = P["free"][lu2["func"]][T[lpn][1]]
	r2 = P["free"][lu2["func"]][T[lpn][1]+1]
	lname = "(.__datadd_" + str(random.randint(1, P["ddinfo"]["data"])) + ")"
# ### p2zp32 ### загрузка младших 32 бит адреса, остальное в нашем случае - расширение знакового бита
	lu2["txt"].append(["DeadCFG", "lui",r1,"%hi" + lname])
	lu2["slot"] = ["DeadCFG", "addiu",r1,r1,"%lo" + lname]	# ### не уверен что так корректно ### но для демо-версии пойдёт ###
							# ### правильно будет последнюю нормальную команду участка положить в слот и соответственно поправить frg ###
#	if "frg" in lu2: lu2["frg"].append(lu2["frg"][-1])	# ### не надо так ###
	if "frg" in lu2: lu2["frg"].append([])
# ### правильная работа со слотами ### надо отладить, где-то путается в слотах ###
# ### понял - нельзя класть в слот команды перехода участка команду из слота вызова подпрограммы (например printf, которые в main есть в конце участков) ###
# ### получается что наше lui попадает в слот команды jal, там регистр портится и дальше не понятно как работает ложный переход ###
# ### при этом правильная команда из слота jal попадает в слот ложного перехода и не выполняется до вызова jal ###
# ### нужно подумать как это победить ... ###
#	if len(lu2["txt"]):
#		lu2["slot"] = copy.deepcopy(lu2["txt"][-1])
#		lu2["txt"][-1] = ["lui",r1,"%hi" + lname]
#	else:
#		lu2["slot"] = ["nop"]
#		lu2["txt"].append(["lui",r1,"%hi" + lname])
#		if "frg" in lu2: lu2["frg"].append(lu2["frg"][-1])
#	if "frg" in lu2: lu2["frg"].append(lu2["frg"][-1])
#	lu2["txt"].append(["addiu",r1,r1,"%lo" + lname])
# ### === ###
# пока без анализа данных в ddata, только один вариант команды
	lu2["cmd"] = ["DeadCFG", "beqz", r1, "_LU_" + str(T[lpn][0])]
	F[lpn] = lu2
print(sys.argv[0], " - запутывание CFG (генерация ложных дуг)")
print(sys.argv[1], "==>", sys.argv[2])
print("Сгенерировано дополнительных дуг - ", dd)
P["attr"] = fmp.a_section("p", sys.argv[1], sys.argv[2])
F2 = fmp.format_json(P)
f2 = open(sys.argv[2], "w")
for line in F2: f2.write(line + "\n")
f2.close()
