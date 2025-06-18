#!/usr/bin/env python3
#
# Генерация файла .l.json из файла .f.json для MIPS 
# c разбиением на участки функций исходной программы, (С), Als, 04/2021
#
import sys, json, os
sys.path.append(os.path.dirname(os.path.realpath(__file__)))
import fmipsproc as fmp

if len(sys.argv) != 3:
	print("\t" + sys.argv[0] + " in_file.f.json  out_file.l.json")
	sys.exit(2)

P  = json.loads(open(sys.argv[1], "r").read())
F  = P["code"]		# Исходные секции кода
C  = {}			# Новые секции кода

if P["attr"]["version"] != "mips.f.0.1":
	print("Неподдерживаемая версия входного файла " + sys.argv[1]); sys.exit(2)

for fun in F: C[fun] = fmp.create_lu(fun, F[fun])	# разбиение на участки

P["attr"] = fmp.a_section("l", sys.argv[1], sys.argv[2])

P["code"] = C

F2 = fmp.format_json(P)	# форматирование выходных данных
			
f2 = open(sys.argv[2], "w")
for line in F2: f2.write(line + "\n")
f2.close()
