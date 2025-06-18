#!/usr/bin/env python3
#
# Генерация текста на ассемблере MIPS из файла .p.json, (С), Als, 04/2021
#
import sys, json, random, os
sys.path.append(os.path.dirname(os.path.realpath(__file__)))
import fmipsproc as fmp

fmp.set_random()

def cmderr():
	print("\t" + sys.argv[0] + " in_file.p.json  out_file.S [-s]")
	sys.exit(2)

shuf = 0
if len(sys.argv) < 3 or len(sys.argv) > 4: cmderr()
if len(sys.argv) == 4:
	if sys.argv[3] == "-s": shuf = 1
	else: cmderr()

P  = json.loads(open(sys.argv[1], "r").read())
A  = P["attr"]
G  = P["globals"]
L  = P["locals"]
F  = P["code"]
if shuf: random.shuffle(F)
D  = P["data"]
if "ddata" in P:
	DD = P["ddata"]
else:
	DD = []
if "mcbuf" in P:
	MC = P["mcbuf"]
else:
	MC = []
GS = P["gset"]
FS = P["fset"]
DS = P["dset"]
GD = P["gdir"]

if A["version"] != "mips.p.0.1":
	print("Неподдерживаемая версия файла " + sys.argv[1]); sys.exit(2)

f2 = open(sys.argv[2], "w")
fmp.fwc(" Файл сгенерирован автоматически", f2)
fmp.fwc("\t" + "  ".join(sys.argv), f2)
if len(GD):
	fmp.fwc(" === Глобальные директивы", f2)
	fmp.fwt(".text", f2)
	for line in GD:	fmp.fwt(line, f2)
if len(G):
	fmp.fwc(" === Глобальные символы", f2)
	for line in G:	fmp.fwt(".globl\t" + line, f2)
if len(L):
	fmp.fwc(" === Локальные символы", f2)
	for line in L:	fmp.fwt(".local\t" + line, f2)
if len(GS):
	fmp.fwc(" === Глобальные установки", f2)
	for line in GS:	fmp.fwt(".set\t"   + line, f2)
if len(FS):
	fmp.fwc(" === Установки секций кода", f2)
	for line in FS: fmp.fwt(".set\t"   + line, f2)
fmp.fwc(" === Секции кода", f2)
S = ""
for lu in F:
# заголовок - метки и директивы
	if "section" in lu and lu["section"] != S: 
		S = lu["section"]
		fmp.fwt(S, f2)
	if "globl"   in lu: fmp.fwt(".globl\t" + lu["globl"], f2)
	if "align"   in lu: fmp.fwt(".align\t" + lu["align"], f2)
	if "p2align" in lu: fmp.fwt(".p2align\t" + lu["p2align"], f2)
	if "label"   in lu: fmp.fw (lu["label"] + ":", f2)
	fmp.fw("_LU_" + str(lu["lunum"]) + ":", f2)
# перемещение и кодирование
	if "move" in lu: fmp.mv_code(lu, P["free"][lu["func"]], f2)
# тело участка
	for l in lu["txt"]: 
		if len(l): fmp.fc(l, f2)
# команда перехода и слот
	if "cmd"  in lu: fmp.fc(lu["cmd"], f2)
	if "slot" in lu: fmp.fc(lu["slot"], f2)
# команда перехода на следующий участок при перемешивании
	if shuf and "next" in lu:
		fmp.fc(["ShuffleJmp", "j", "_LU_" + str(lu["next"])], f2)
		fmp.fc(["ShuffleJmp", "nop"], f2)
if len(DS):
	fmp.fwc(" === Установки секций данных", f2)
	for line in DS:	fmp.fwt(".set\t"   + line, f2)
if len(D):
	fmp.fwc(" === Секции данных", f2)
	for dat in D:
		for line in dat: fmp.fw(line, f2)
if len(DD):
	fmp.fwc(" === Секции мертвых данных", f2)
	for dat in DD:
		for line in dat: fmp.fw(line, f2)
if len(MC):
	fmp.fwc(" === Память для перемещаемых участков", f2)
	for line in MC: fmp.fw(line, f2)
f2.close()
