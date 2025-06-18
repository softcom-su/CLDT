#!/usr/bin/env python3
#
# Слияние нескольких (до 26) файлов .l.json в один для MIPS, (С), Als, 04/2021
# больше чем 26 файлов в командную строку вряд ли поместится
# ### при большом количестве исходных файлов преобразование можно выполнять несколько раз ###
#
import sys, json, time, os
sys.path.append(os.path.dirname(os.path.realpath(__file__)))

import fmipsproc as fmp

def serror(x, f):
	print("Ошибка : файл", f, "секция", x)
	sys.exit(2)
def gerror(x, f):
	print("Ошибка : файл", f, "дублирование глобального символа", x)
	sys.exit(2)
def ferror(x, f):
	print("Ошибка : файл", f, "дублирование имени функции", x)
	sys.exit(2)

if len(sys.argv) < 3:
	print("\t" + sys.argv[0] + " out_file.l.json  in_file1.l.json  [in_file2.l.json [...]]")
	sys.exit(2)
if len(sys.argv) > 28:
	print("\t Слишком много входных файлов в командной строке, поддерживается не больше 26")
	sys.exit(2)

A = fmp.a_section("l", "", sys.argv[1])

P  = {}; P["attr"] = A
F  = {}; P["code"] = F
D  = []; P["data"] = D

S = ["." + chr(i+65) for i in range(26)]

for i, I in enumerate(sys.argv[2:]):
	P1  = json.loads(open(I, "r").read())
	A1  = P1["attr"]
	G1  = P1["globals"]
	L1  = P1["locals"]
	F1  = P1["code"]
	D1  = P1["data"]
	GS1 = P1["gset"]
	GD1 = P1["gdir"]
	FS1 = P1["fset"]
	DS1 = P1["dset"]
	if A1["version"] != "mips.l.0.1":
		print("Неподдерживаемая версия файла " + I); sys.exit(2)
	if len(A["source"]): A["source"] += " "
	A["source"] += I
	if not i:
#		G = G1; GS = GS1; FS = FS1; DS = DS1
		P["globals"] = G1;  G  = P["globals"]
		P["gset"]    = GS1; GS = P["gset"]
		P["fset"]    = FS1; FS = P["fset"]
		P["dset"]    = DS1; DS = P["dset"]
		P["locals"]  = [];  L  = P["locals"]
		P["gdir"]    = GD1; GD = P["gdir"]
	else:
		if GS != GS1: serror("gset", I)
		if FS != FS1: serror("fset", I)
		if DS != DS1: serror("dset", I)
		for g in G1:
			if g in G:
				gerror(g, I)
			else:
				G.append(g)
		for g in GD1:
			if g not in GD: GD.append(g)
	L += [S[i] + lm for lm in L1]
	LM = [lm for lm in L1]	# локальные метки
	
	for fun in F1:
		for lu in F1[fun]:
			if "label" in F1[fun][lu] and F1[fun][lu]["label"] not in G:
				if F1[fun][lu]["label"] not in LM: LM.append(F1[fun][lu]["label"])
	for d in D1:
		for line in d:
			p = line.split()
			if len(p) == 1 and p[0][-1] == ":" and p[0][:-1] not in G1:
				if p[0][:-1] not in LM: LM.append(p[0][:-1])
	# нашли все локальные метки
	# переименование меток в секции данных
	for j, d in enumerate(D1):
		for k, l in enumerate(d):
			s = fmp.macsplit(D1[j][k])
			cor = 0
			for n, f in enumerate(s[1]):
				for lm in LM:
					if lm == f: 
						s[1][n] = S[i] + s[1][n]
						cor += 1
			if cor: D1[j][k] = fmp.macjoin(s)
	D += D1
	# переименование меток в секции кода
	for fun in F1:
		for lu in F1[fun]:
			if "cmd" in F1[fun][lu]:
				if F1[fun][lu]["cmd"][-1] in LM: F1[fun][lu]["cmd"][-1] = S[i] + F1[fun][lu]["cmd"][-1]
			if "label" in F1[fun][lu]:
				if F1[fun][lu]["label"] in LM: F1[fun][lu]["label"] = S[i] + F1[fun][lu]["label"]
			for j, line in enumerate(F1[fun][lu]["txt"]):
				for k, l in enumerate(F1[fun][lu]["txt"][j]):
					s = fmp.macsplit(F1[fun][lu]["txt"][j][k])
					cor = 0
					for n, f in enumerate(s[1]):
						for lm in LM:
							if lm == f:
								s[1][n] = S[i] + s[1][n]
								cor += 1
					if cor: F1[fun][lu]["txt"][j][k] = fmp.macjoin(s)
	# добавляем секцию кода к выходному файлу
	for fun in F1:
		if fun not in F:
			F[fun] = F1[fun]
		else:
			ferror(fun, I)

F2 = fmp.format_json(P)	# форматирование выходного файла
			
f2 = open(sys.argv[1], "w")
for line in F2: f2.write(line + "\n")
f2.close()
