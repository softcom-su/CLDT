#!/usr/bin/env python3
#
# Синтаксический анализ v.02 файла .p.json, (С), Als, 05/2021
# использует секцию frg при формировании списка свободных регистров
#
import sys, json, copy, os
sys.path.append(os.path.dirname(os.path.realpath(__file__)))
import fmipsproc as fmp

if len(sys.argv) != 3:
	print("\t" + sys.argv[0] + " in_file.p.json  out_file.p.json")
	sys.exit(2)
P  = json.loads(open(sys.argv[1], "r").read())
F  = P["code"]
if P["attr"]["version"] != "mips.p.0.1":
	print("Неподдерживаемая версия входного файла " + sys.argv[1]); sys.exit(2)

# поиск неиспользуемых регистров по функциям
FR = {}
#NR = {"$a0":0, "$a1":0, "$a2":0, "$a3":0, "$v0":0, "$v1":0, "$t0":0, "$t1":0, "$t2":0, "$t3":0, "$t4":0, "$t5":0, "$t6":0, "$t7":0, "$t8":0, "$t9":0}
NR = {"$t0":0, "$t1":0, "$t2":0, "$t3":0, "$t4":0, "$t5":0, "$t6":0, "$t7":0, "$t8":0, "$t9":0}
for lu in F:
	if "func" in lu:
		if lu["func"] not in FR: FR[lu["func"]] = copy.deepcopy(NR)
		NRF = FR[lu["func"]]
		for line in lu["txt"]:
			if len(line) > 1 and line[1] in NRF: NRF[line[1]] += 1
		if "cmd"  in lu and len(lu["cmd"])  > 1 and lu["cmd"][1]  in NRF: NRF[lu["cmd"][1]]  += 1
		if "slot" in lu and len(lu["slot"]) > 1 and lu["slot"][1] in NRF: NRF[lu["slot"][1]] += 1
FRG = {}
for func in FR:
	RG = []
	for r in FR[func]:
		if FR[func][r] == 0: RG.append(r)
	RG.sort()
	FRG[func] = RG
P["free"] = FRG

# синтаксический анализ v.2.0
for lu in F: 
	lu["txt"], lu["frg"] = fmp.syn2(lu["txt"])

# статистика
C0= 0	# всего машинных команд (включая free)
C1= 0	# команд переходов
C2= 0	# команд в слотах
L = 0	# всего участков
N = 0	# из них с точками внедрения
X = 0	# всего точек внедрения
X2= 0	# всего свободных регистров в точках внедрения
Y = {}	# перечень свободных регистров
Z = [0 for i in range(70)]	# количество (с запасом на $Fx, HI, LO и т.п.)
for lu in F:
	L += 1
	n = 0
	C0 += len(lu["txt"])
	if "cmd" in lu : C1 += 1
	if "slot" in lu: C2 += 1
#	for line in lu["txt"]:
#		if "free" in line[0]: 
#			n += 1
#			Z[len(line)-1] += 1
#			for r in line[1:]:
#				if r in Y:
#					Y[r] += 1
#				else:
#					Y[r] = 1
	for line in lu["frg"]:
		X2 += len(line)
		if len(line): n += 1
	X += n
	if n: N += 1
print(sys.argv[0], " - синтаксический анализ свободных регистров")
print(sys.argv[1], "==>", sys.argv[2])
print("Всего линейных участков      :", L)
print("Из них с точками внедрения   :", N)
print("Машинных команд (в участках) :", C0)
print("Команд переходов             :", C1)
print("Команд в слотах переходов    :", C2)
print("Всего точек внедрения        :", X)
print("Свободных регистров в точках внедрения :", X2)

#print("Количество свободных регистров / Количество точек внедрения")
#for i, x in enumerate(Z):
#	if x: print("\t", i, "/", x)
#print("Свободные регистры / Количество")
#for r in sorted(Y):
#	print("\t", r, "/", Y[r])

#print("Не используемые в тексте регистры :")
#if len(RG): print("   ", RG)
#else: print("    отсутствуют")
# сохранение выходного файла
P["attr"] = fmp.a_section("p", sys.argv[1], sys.argv[2])
F2 = fmp.format_json(P)
f2 = open(sys.argv[2], "w")
for line in F2: f2.write(line + "\n")
f2.close()
