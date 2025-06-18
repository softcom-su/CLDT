#!/usr/bin/env python3
#
# Выбор участков для перемещения и кодирования в файле .p.json, (С), Als, 06/2021
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
	FN = []			# список функций будет заполняться
else:
	FN = sys.argv[4:]	# список заданных функций для использования метода
	fn = 1		# работаем по списку функций
print(sys.argv[0], " - перемешение и кодирование участков с вероятностью", sys.argv[3] + "% для функций :", " ".join(sys.argv[4:]))
print("\nИсходный файл : " + sys.argv[1])

K, MV, MC = fmp.mselect(F, FN, fn, PC)		# Кандидатов (K), выбраны для перемещения (MV), кодирования с перемещением (MC)

# добавить секцию для выполнения перемещаемых участков ### если её еще нет ###
if "mcnum" in P:
	MCL = P["mcnum"]
else:
	MCL = 10
	P["mcnum"] = MCL
	P["mcbuf"] = [".bss", ".align 3"]
	for i in range(MCL):
		P["mcbuf"].append("__bssmc_" + str(i+1) + ":\t.fill\t10, 8, 0")
# добавление данных с ключами и атрибутами кодирования в участки
for i, lu in enumerate(F):
	if lu["func"] in FN:
		if "free" not in P or lu["func"] not in P["free"] or len(P["free"][lu["func"]]) < 5:
			print("  Ошибка! Нет свободных регистров для перемещения и кодирования. Номер участка", lu["lunum"], "Функция", lu["func"])
			sys.exit(2)
		else:
			if lu["lunum"] in MV: F[i] = fmp.set_mv(lu, MCL)
			if lu["lunum"] in MC: F[i] = fmp.set_mc(lu, MCL)
if not fn:
	print("     всего участков    : " + str(len(F)))
else:
	print("     всего участков    : " + str(fmp.flen(F, FN)))
print("     из них кандидатов : " + str(len(K)))
fmp.pf(F, K)
print("\nВыходной файл : " + sys.argv[2])
print("     выбрано участков для перемещения : " + str(len(MV)))
fmp.pf(F, MV)
print("     для кодирования с перемещением   : " + str(len(MC)))
fmp.pf(F, MC)
# сохранение выходного файла
P["attr"] = fmp.a_section("p", sys.argv[1], sys.argv[2])
F2 = fmp.format_json(P)
f2 = open(sys.argv[2], "w")
for line in F2: f2.write(line + "\n")
f2.close()
