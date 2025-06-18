#!/usr/bin/env python3
#
# Генерация .p.json из .l.json для MIPS, (С), Als, 04/2021
#
import sys, json, os
sys.path.append(os.path.dirname(os.path.realpath(__file__)))
import fmipsproc as fmp

if len(sys.argv) != 3:
	print("\t" + sys.argv[0] + " in_file.l.json  out_file.p.json")
	sys.exit(2)
P  = json.loads(open(sys.argv[1], "r").read())
F  = P["code"]
C  = []
if P["attr"]["version"] != "mips.l.0.1":
	print("Неподдерживаемая версия входного файла " + sys.argv[1]); sys.exit(2)
N = 0	# номер последнего участка предыдущей функции
S = P["attr"]["codesec"]
for fun in sorted(F):
	K = {}
	for lu in F[fun]: K[int(lu)] = F[fun][lu]
	for lu in sorted(K):
		if "next" in K[lu]: K[lu]["next"] += N
		if "target" in K[lu]: K[lu]["target"] += N
		if "section" in K[lu]: 
			S = K[lu]["section"]
		else:
			K[lu]["section"] = S
		K[lu]["lunum"] = lu+N
		K[lu]["func"]  = fun
		C.append(K[lu])
	N += lu
# замена локальных меток в командах перехода на метки участков
LM = {}		# соответствие локальных меток номерам участков
for i, lu in enumerate(C):
	if "label" in lu and lu["label"] not in P["globals"]:
		LM[lu["label"]] = lu["lunum"]
		del C[i]["label"]
for i, lu in enumerate(C):
	if "cmd" in lu and lu["cmd"][-1] in LM:
		C[i]["cmd"][-1] = "_LU_" + str(LM[lu["cmd"][-1]])

# ### для поддержки переходов по таблицам и ссылок на локальные метки в коде и данных	###
# ### добавить здесь замену во всех секциях TXT, слотах и данных имен локальных меток	###
# ### если поддержка вдруг потребуется - текст не будет транслироваться, так и узнаем	###

P["attr"] = fmp.a_section("p", sys.argv[1], sys.argv[2])
P["code"] = C
F2 = fmp.format_json(P)

f2 = open(sys.argv[2], "w")
for line in F2: f2.write(line + "\n")
f2.close()
