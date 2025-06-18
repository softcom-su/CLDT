#!/usr/bin/env python3
#
# Дробление участков в файле .p.json, (С), Als, 05/2021
#
import sys, json, os
sys.path.append(os.path.dirname(os.path.realpath(__file__)))
import fmipsproc as fmp

if len(sys.argv) != 4:
	print("\t" + sys.argv[0] + " in_file.p.json  out_file.p.json min_lu_len")
	sys.exit(2)
P  = json.loads(open(sys.argv[1], "r").read())
F  = P["code"]
if P["attr"]["version"] != "mips.p.0.1":
	print("Неподдерживаемая версия входного файла " + sys.argv[1]); sys.exit(2)
ML = int(sys.argv[3])			# рекомендованная длина участка при дроблении
# начальная статистика
C0 = 0					# всего машинных команд
C1 = 0					# команд переходов
C2 = 0					# команд в слотах
L0 = len(F)				# исходное количество участков
L1 = 0					# количество новых участков
N0 = min(lu["lunum"] for lu in F)	# минимальный номер участка
N1 = max(lu["lunum"] for lu in F)	# максимальный номер участка
N2 = N1 + 1				# начальный номер для новых участка

C = []					# новая секция кода
if "split" in P:
	S = fmp.int_key(P["split"])
else:
	S = {}
	for lu in F: S[lu["lunum"]] = [lu["lunum"]]	# первичное заполнение секции split

for lu in F:
	N2, c0, c1, c2, c = fmp.split_lu(lu, ML, N2)	# дробление
	C += c; C0 += c0; C1 += c1; C2 += c2
	if len(c) > 1:					# дозапись протокола если участок дробился
		cs = [l["lunum"] for l in c] 
		if c[0]["lunum"] in S and len(S[c[0]["lunum"]]) > 1:	# ### похоже(?) не очень корректное условие, подумать как сделать по другому ###
			S[c[0]["lunum"]] = cs + S[c[0]["lunum"]][1:]
		else:
			for x in S:
				for i, y in enumerate(S[x]):
					if y == cs[0]:
						S[x] = S[x][:i] + cs + S[x][i+1:]
P["split"] = S
P["code"]  = C
L1 = len(C) - L0
print(sys.argv[0], " - дробление ЛУ, минимальный размер участка", sys.argv[3], "машинных команд")
print(sys.argv[1], "==>", sys.argv[2])
print("Всего в исходном файле")
print("       линейных участков :", L0)
print("       машинных команд   :", C0)
print("       команд переходов  :", C1)
print("       команд в слотах   :", C2)
print("    минимальный номер ЛУ :", N0)
print("   максимальный номер ЛУ :", N1)
print("Добавлено новых участков :", L1)

# сохранение выходного файла
P["attr"] = fmp.a_section("p", sys.argv[1], sys.argv[2])
F2 = fmp.format_json(P)
f2 = open(sys.argv[2], "w")
for line in F2: f2.write(line + "\n")
f2.close()
