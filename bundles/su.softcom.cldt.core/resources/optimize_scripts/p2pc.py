#!/usr/bin/env python3
#
# Внедрение мертвого кода в файле .p.json, (С), Als, 06/2021
#
import sys, json, random, copy, os
sys.path.append(os.path.dirname(os.path.realpath(__file__)))
import fmipsproc as fmp

if len(sys.argv) < 5:
	print("\t" + sys.argv[0] + " in_file.p.json  out_file.p.json  percentage  all|func1 [func2 [...] ]")
	sys.exit(2)
P  = json.loads(open(sys.argv[1], "r").read())
F  = P["code"]
if P["attr"]["version"] != "mips.p.0.1":
	print("Неподдерживаемая версия входного файла " + sys.argv[1]); sys.exit(2)
PC = int(sys.argv[3])			# процент вероятности использования каждой точки внедрения
fn = 0			# используем все функции
if sys.argv[4] == "all":
	FN = []
else:
	FN = sys.argv[4:]
	fn = 1		# работаем по списку функций
FC = []			# функции, в которые внедрен мертвый код
LU = []			# участки, в которые внедрен мертвый код
# начальная статистика
C0 = 0			# всего машинных команд
C1 = 0			# команд переходов
C2 = 0			# команд в слотах
C3 = 0			# добавлено команд мертвого кода
C4 = 0			# общее количество точек внедрения
L0 = len(F)		# количество участков
L1 = 0			# количество участков с мертвым кодом

C = []			# новая секция кода

for lu in F:
	c0 = len(lu["txt"])
	c1 = 0; c2 = 0; c3 = 0
	if "cmd"  in lu: c1 += 1
	if "slot" in lu: c2 += 1
	if not fn or lu["func"] in FN:
		c, c3, c4 = fmp.deadcode(lu, PC)			# внедрение мертвого кода
		if c3: 
			if c["func"]  not in FC: FC.append(c["func"])
			if c["lunum"] not in LU: LU.append(c["lunum"])
			L1 += 1
	else:
		c = copy.deepcopy(lu)
	del c["frg"]						# удаляем описание свободных регистров
	C.append(c); C0 += c0; C1 += c1; C2 += c2; C3 += c3; C4 += c4

P["code"]  = C

print(sys.argv[0], " - внедрение мертвого кода с вероятностью", sys.argv[3] + "%", "в каждой точке внедрения")
print(sys.argv[1], "==>", sys.argv[2])
print("Всего в исходном файле")
print("       линейных участков :", L0)
print("       машинных команд   :", C0)
print("       команд переходов  :", C1)
print("       команд в слотах   :", C2)
print("       точек внедрения   :", C4)
print("Добавлено новых команд   :", C3)
print("Количество участков с мертвым кодом :", L1)
FC.sort()
LU.sort()
print("Измененения внесены")
print("\tв функции :", FC)
print("\tв участки :", LU)

# сохранение выходного файла
P["attr"] = fmp.a_section("p", sys.argv[1], sys.argv[2])
F2 = fmp.format_json(P)
f2 = open(sys.argv[2], "w")
for line in F2: f2.write(line + "\n")
f2.close()
