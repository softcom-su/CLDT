#!/usr/bin/env python3
#
# Генерация файлов .cfg для всех функций файла .p.json, (С), Als, 06/2021
#
import sys, json, random

def cmderr():
	print("\t" + sys.argv[0] + " in_file.p.json  ./cfg_dir X")
	sys.exit(2)

if len(sys.argv) != 4: cmderr()

P  = json.loads(open(sys.argv[1], "r").read())
A  = P["attr"]
F  = P["code"]

if A["version"] != "mips.p.0.1":
	print("Неподдерживаемая версия файла " + sys.argv[1]); sys.exit(2)

#print("\tГенерация файлов .CFG для ", sys.argv[1])
C = {}
for lu in F:
	f = lu["func"]
	if f not in C: C[f] = []
	C[f].append([str(lu["lunum"])])
	if "next" in lu: C[f][-1].append(str(lu["next"]))
	if "target" in lu: C[f][-1].append(str(lu["target"]))
	if len(C[f][-1]) == 1: C[f][-1].append("R")
for f in sorted(C):
	fn = sys.argv[2] + "/" + f + "_" + sys.argv[3] + ".cfg"
	f2 = open(fn, "w")
	for line in C[f]: f2.write("\t".join(line) + "\n")
	f2.close()
	print(fn)

