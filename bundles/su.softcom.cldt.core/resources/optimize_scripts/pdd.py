#!/usr/bin/env python3
#
# Внедрение мертвых данных в файле .p.json, (С), Als, 05/2021
#
# Сохраняет все ранее внедренные мертвые данные и добавляет новые в указанном количестве фрагментов
# Каждый фрагмент - одно слово (4 байта)
# Имена меток и количество полей описаны в секции ddinfo в подсекциях bss и data
# Данные описаны в секции ddata
# 
import sys, json, random, os
sys.path.append(os.path.dirname(os.path.realpath(__file__)))
import fmipsproc as fmp

fmp.set_random()

if len(sys.argv) != 5:
	print("\t" + sys.argv[0] + " in_file.p.json  out_file.p.json data_num bss_num")
	sys.exit(2)
DX = int(sys.argv[3])	# сколько добавить переменных в .data
BX = int(sys.argv[4])	# сколько добавить переменных в .bss

P  = json.loads(open(sys.argv[1], "r").read())
D  = P["data"]
if P["attr"]["version"] != "mips.p.0.1":
	print("Неподдерживаемая версия входного файла " + sys.argv[1]); sys.exit(2)
# начальная статистика
D0 = 0		# количество секций с данными
D1 = 0		# строк в секциях данных
D2 = 0		# секций в обнаруженных мертвых данных
D3 = 0		# строк в обнаруженных мертвых данных
D4 = 0		# добавлено переменных в .bss
D5 = 0		# добавлено строк в .bss
D6 = 0		# добавлено переменных в .data
D7 = 0		# добавлено строк в .data
DS = []		# список обнаруженных секций с данными
DDS= []		# список обнаруженных секций с мертвыми данными
DD = []		# "ddata", если уже есть - переопределится ниже
for d in D:
	for line in d:
		D1 += 1
		x, s = fmp.get_dsection(line)
		if x and s not in DS:
			D0 += 1
			DS.append(s)
if "ddata" in P:
	DD = P["ddata"]
	for d in DD:
		for line in d:
			D3 += 1
			x, s = fmp.get_dsection(line)
			if x and s not in DDS:
				D2 += 1
				DDS.append(s)
else:
	P["ddata"] = DD
	P["ddinfo"] = {"bss":0, "data":0}
NBX = P["ddinfo"]["bss"]
DD.append([".bss", ".align\t4"])
for i in range(BX):
	NBX += 1
	DD[-1] += [(".__bssdd_"+str(NBX)) + ":", "\t.long\t0"]
P["ddinfo"]["bss"] = NBX
NDX = P["ddinfo"]["data"]
DD.append([".data", ".align\t4"])
for i in range(DX): 
	NDX += 1
	DD[-1] += [(".__datadd_"+str(NDX)) + ":", "\t.long\t" + hex(random.randint(1, 1000000))]
P["ddinfo"]["data"] = NDX
D4 += BX
D5 += 2 + BX*2
D6 += DX
D7 += 2 + DX*2

print(sys.argv[0], " - внедрение мертвых данных, файл", sys.argv[2])
print(sys.argv[1], "==>", sys.argv[2])
print("Всего в исходном файле")
print("       секций с данными :", D0, DS)
print("       строк с данными  :", D1)
print("       секций с мертвыми данными :", D2, DDS)
print("       строк с мертвыми данными  :", D3)
print("Добавлено при внедрении")
print("    переменных в .bss  :", D4)
print("         строк в .bss  :", D5)
print("    переменных в .data :", D6)
print("         строк в .data :", D7)

# сохранение выходного файла
P["attr"] = fmp.a_section("p", sys.argv[1], sys.argv[2])
F2 = fmp.format_json(P)
f2 = open(sys.argv[2], "w")
for line in F2: f2.write(line + "\n")
f2.close()
