#!/usr/bin/env python3
#
# Дробление участков в файле .p.json, (С), Als, 05/2021
#
import sys, json
import os, sys, copy, json, re, time, random


def islabel(l):
	p = l.split()
	if len(p) and p[0][-1] == ":": return 1
	return 0


jmp = ["b", "j", "jr", "jr.hb"]
jcc = ["bc1f", "bc1fl", "bc1t", "bc1tl", "bc2f", "bc2fl", "bc2t", "bc2tl", "beq", "beql", "bgez", "bgezl", "bgtz",
	   "bgtzl", \
	   "blez", "blezl", "bltz", "bltzl", "bne", "bnel", "bnez", "beqz", "bge", "bgt", "bgtu", "ble", "blt", "bnez"]
calls = ["bal", "bgezal", "bgezall", "bltzal", "bltzall", "jal", "jalr", "jalr.hb"]
exits = ["exit"]
SC = ['add', 'addi', 'addiu', 'addu', 'and', 'andi', 'cfc1', 'cfc2', 'clo', 'clz', 'dadd', 'daddi', 'daddiu', 'daddu',
	  'dclo', \
	  'dclz', 'dmfc0', 'dmfc1', 'dmfc2', 'dneg', 'dnegu', 'drotr', 'drotr32', 'drotrv', 'dsbh', 'dshd', 'dsll',
	  'dsll32', 'dsllv', \
	  'dsra', 'dsra32', 'dsrav', 'dsrl', 'dsrl32', 'dsrlv', 'dsub', 'dsubu', 'lb', 'lbu', 'ld', 'ldl', 'ldr', 'lh',
	  'lhu', 'lui', \
	  'lw', 'lwl', 'lwr', 'lwu', 'mfc0', 'mfc1', 'mfc2', 'mfhc1', 'mfhc2', 'mfhi', 'mflo', 'move', 'movf', 'movn',
	  'movt', 'movz', \
	  'mul', 'neg', 'negu', 'nor', 'not', 'or', 'ori', 'rotr', 'rotrv', 'sll', 'sllv', 'slt', 'slti', 'sltiu', 'sltu',
	  'sra', \
	  'srav', 'srl', 'srlv', 'sub', 'subu', 'xor', 'xori']
wcmd = jmp + jcc + calls + exits

RX = ["$sp", "$fp", "$at", "$k0", "$k1", "$zero", "$gp", "$ra", "$hi", "$lo"]
# RX = [ "$sp", "$fp",        "$k0", "$k1", "$zero", "$gp", "$ra", "$hi", "$lo"]	# регистры, которые не будем использовать в качестве свободных

OPTI = {"DeadCode": "Мертвый код", "DeadCFG": "Запутывание CFG", "ShuffleJmp": "Переход при перемешивании ЛУ"}


#
# Синтаксический анализ v.1.0
#	- работает только с форматом P
#	- генерирует и возвращает новый текст участка
#	- в каждой возможной точке внедрения записывается "машинная команда" ["free-Х", "$rx", ...], где Х - количество свободных регистров в точке
#	- эти команды не выводятся в ассемблерный текст при обработке с помощью p2S.py
#   TODO
#	- ### не поднимает свободный регистр из команды в слоте через вызов подпрограммы
#	+ ### поиск регистров, свободных во всем тексте (реализовано в p2ps)
#	- ### искать регистры, не используемые в конкретных функциях (### пока возможно только в l.json ###), и использовать их для мертвого кода
#	  ###   - м.б. поиск свободных регистров сделать в l.json и отмечать их не командами free а какими-то структурами данных?
#	  ###   например в заголовке участка сделать ключ free для регистров, свободных во всем участке, а для конкретных точек внедрения оставить как есть?
#	  ###   - или принимать 2 входных файла - l.json и p.json?
#	  ###   - или при выполнении L2P ввести в заголовок участка ключ с именем функции?
#	  ###   тогда можно выполнять синтаксический анализ для формата P и в заголовке участка записать их с ключом free ### так и сделать ###
#
def syn(lu):
	l1 = [];
	l3 = []
	for line in lu:
		l1.append([])
		if len(line) > 1 and line[0] in SC and line[1] not in " ".join(line[2:]) and line[1] not in RX: l1[-1].append(
			line[1])  # не используем регистры из RX
	l2 = copy.deepcopy(l1)
	for i in range(len(l1) - 1, -1, -1):
		if len(l1[i]):
			for j in range(i - 1, -1, -1):
				#				if l1[i][0] not in " ".join(lu[j]):
				if l1[i][0] not in " ".join(lu[j]) and lu[j][
					0] not in calls:  # [calls] сбрасывает список, ### (!) надо поднять свободный регистр из слота (!) ###
					if l1[i][0] not in l2[j]: l2[j].append(l1[i][0])
				else:
					break
	for i in range(len(l1)):
		if len(l2[i]):
			l2[i].sort()
			if (i and lu[i - 1][0] not in calls) or not i: l3.append(
				["free-" + str(len(l2[i]))] + l2[i])  # команда в слоте - без точки внедрения !!!
		l3.append(lu[i])
	return l3


def syn2(lu):
	l1 = [];
	l3 = [];
	l4 = []
	for line in lu:
		l1.append([])
		if len(line) > 1 and line[0] in SC and line[1] not in " ".join(line[2:]) and line[1] not in RX: l1[-1].append(
			line[1])  # не используем регистры из RX
	l2 = copy.deepcopy(l1)
	for i in range(len(l1) - 1, -1, -1):
		if len(l1[i]):
			for j in range(i - 1, -1, -1):
				#				if l1[i][0] not in " ".join(lu[j]):
				if l1[i][0] not in " ".join(lu[j]) and lu[j][
					0] not in calls:  # [calls] сбрасывает список, ### (!) надо поднять свободный регистр из слота (!) ###
					if l1[i][0] not in l2[j]: l2[j].append(l1[i][0])
				else:
					break
	for i in range(len(l1)):
		if len(l2[i]):
			l2[i].sort()
			if (i and lu[i - 1][0] not in calls) or not i:
				#				l4.append(["free-" + str(len(l2[i]))] + l2[i])	# команда в слоте - без точки внедрения !!!
				l4.append(l2[i])  # команда в слоте - без точки внедрения !!!
			else:
				l4.append([])
		else:
			l4.append([])
		l3.append(lu[i])
	return l3, l4


def strm(l):
	p = l.split()
	for i, f in enumerate(p):
		if f[-1] == ",": p[i] = p[i][:-1]
	return p


#
# Описание функции в выходном файле .l.json
#	"имя":{"1":{участок1}, "2":{участок2}, ... , "N":{участокN}}
# Описание участка
#	"номер":{
#		"align":   "X",						// если надо
#		"cmd":     "команда перехода со всеми аргументами",	// если есть
#		"func":    "имя_функции"				// только в формате P
#		"globl":   "аргумент директивы в заголовке ЛУ",		// использовать только для функций которых нет в globals
#		"label":   "метка",					// если есть
#		"lunum":   номер участка				// только в формате P
#		"p2align": "4, 0x90",					// если надо
#		"section": ".text",					// секция, в формате L указывается только у первого участка функции, в формате P - у всех участков
#		"slot":    "команда в слоте со всеми аргументами",	// если есть команда перехода и у неё должен быть слот
#		"next":    номер следующего по тексту участка		// int, если возможен переход на следующий по тексту участок
#		"target":  номер участка - таргета команды перехода	// int, если есть
#		"txt": [
#			["команда1"],
#			...
#			["командаN"]
#			]
#		}
# ### Номера участков при экспорте получаются символьные, при загрузке из формата L надо их обратно сделать числами ###
# ### При замене имен меток на метки начала участков заменять надо в txt, cmd и slot ### надеюсь что не потребуется, пока ставим 2 метки рядом ###
# ### Для инлайна и l2l потребуется заменять имена меток на .<имя_функции>.<имя_или_номер_метки> ###
#
# разобрать еще 2 случая
#	1. .align или .p2align перед меткой (компиляторы так делают для ускорения работы циклов)
#	2. директивы в тексте участка (может возникнуть при самомодификации или генерации несуществующих опкодов)
#
def create_lu(f, F):
	C = {}
	i = 0
	s0 = ""  # .text
	p0 = ""  # .p2align
	a0 = ""  # .align
	l0 = f
	T = {}  # метки участков
	while F[i] != f + ":":
		p = F[i].split()
		if len(p) and p[0] == ".text": s0 = p[0]
		if len(p) > 1 and p[0] == ".align": a0 = p[1]
		if len(p) > 1 and p[0] == ".p2align": p0 = " ".join(p[1:])
		i += 1
	n = 1  # начальный номер участка
	C[n] = {}
	if len(a0): C[n]["align"] = a0
	if len(p0): C[n]["p2align"] = p0
	if len(s0): C[n]["section"] = s0
	if len(l0): C[n]["label"] = l0
	C[n]["txt"] = []
	i += 1
	while i < len(F):
		p = F[i].split()
		if len(p):
			if islabel(p[0]):
				if len(C[n]["txt"]):
					T[p[0][:-1]] = n + 1
				else:
					T[p[0][:-1]] = n
				if i < len(F) - 1: C[n]["next"] = n + 1
				if len(C[n]["txt"]) or "label" in C[
					n]:  # так сработает при двух метках подряд (будет пустой участок с меткой)
					n += 1
					C[n] = {"txt": []}
				C[n]["label"] = p[0][:-1]
			elif p[0] in jcc:
				#				if i < len(F)-2: C[n]["next"] = n + 1
				C[n]["next"] = n + 1
				C[n]["cmd"] = strm(F[i])
				C[n]["slot"] = strm(F[i + 1])
				i += 1
				n += 1
				C[n] = {"txt": []}
			elif p[0] in jmp:
				C[n]["cmd"] = strm(F[i])
				C[n]["slot"] = strm(F[i + 1])
				i += 1
				n += 1
				C[n] = {"txt": []}
			else:
				C[n]["txt"].append(strm(F[i]))
		i += 1
	if not len(C[n]["txt"]) and "cmd" not in C[n] and "slot" not in C[n] and "label" not in C[n]:
		del C[n]  # удалим последний пустой участок
		if "next" in C[n - 1]: del C[n - 1]["next"]  # и ссылку на него
	# расставляем/удаляем таргеты переходов
	for lu in C:
		if "cmd" in C[lu]:
			t = C[lu]["cmd"][-1]
			if t in T:
				C[lu]["target"] = T[t]
		if len(C[lu]["txt"]) > 1 and "next" in C[lu] and "target" not in C[lu] and C[lu]["txt"][-2][-1] in exits:
			del C[lu]["next"]
		if "next" in C[lu] and "cmd" in C[lu] and C[lu]["cmd"][0] in jmp:
			del C[lu]["next"]
	return C


#
# форматирование текста .json для улучшения читабельности
#
def format_json(j):
	j["globals"].sort()
	j["locals"].sort()
	if "split" in j:
		S = int_key(j["split"])  # если ключи уже инты - не страшно
		j["split"] = S
	js = json.dumps(j, indent=2, sort_keys=True).split("\n")
	# несколько проходов м.б. и медленнее но меньше логики и проще править
	# форматирование секций с кодом участков
	F0 = []
	intxt = 0
	for line in js:
		p = line.split()
		if not intxt:
			if len(p) == 2 and p[0] == '"txt":' and p[1] == "[": intxt = 1
			F0.append(line)
		elif p[0] == "[":
			ln = copy.deepcopy(line)
		elif p[0] == "]":
			F0.append(ln + "]")
			intxt = 0
		elif p[0] == "],":
			F0.append(ln + "],")
		else:
			ln += p[0]
	# секции со свободными регистрами в участках
	F1 = []
	infr = 0
	for line in F0:
		p = line.split()
		if not infr:
			if len(p) == 2 and p[0] == '"frg":' and p[1] == "[": infr = 1
			F1.append(line)
		elif infr and "[" not in line and "]" not in line:
			F1[-1] += p[0]
		elif line == "        ]," or line == "        ]":
			F1[-1] += p[0]
		elif line == "      ],":
			F1.append(line)
			infr = 0
		else:
			F1.append(line)
	# команды переходов и слоты
	F2 = []
	incmd = 0
	for line in F1:
		if not incmd:
			if '"cmd"' in line or '"slot"' in line:
				incmd = 1
				ln = line
			else:
				F2.append(line)
		else:
			p = line.split()
			ln += p[0]
			if "]" in line:
				F2.append(ln)
				incmd = 0
	# свободные регистры по функциям
	#	F3 = []
	#	infree = 0
	#	for line in F2:
	#		if not infree:
	#			if '"free": {' in line:
	#				infree = 1
	#				infunc = 0
	#			F3.append(line)
	#		else:
	#			p = line.split()
	#			if not infunc:
	#				if ":" in line:
	#					infunc = 1
	#					F3.append("    " + p[0])
	#					ln = "      ["
	#				else:
	#					if "}" in line:
	#						infree = 0
	#						F3.append(line)
	#					else:
	#						print("Ошибка форматирования"); sys.exit(2)
	#			else:
	#				if "]" in line:
	#					ln += p[0]
	#					F3.append(ln)
	#					infunc = 0
	#				else:
	#					ln += p[0]
	# протокол дробления
	F4 = []
	F3 = F2  # ### разобраться что там выше не сработало
	insplit = 0
	for line in F3:
		if not insplit:
			if '"split": {' in line: insplit = 1
			F4.append(line)
		else:
			if "[" in line:
				ln = line
			elif "]" in line:
				ln += line.split()[0]
				F4.append(ln)
				if "," not in line: insplit = 0
			else:
				ln += line.split()[0]

	return F4


def macsplit0(l, splitchar):
	splitprefix = []
	splitbody = []
	splittext = []
	inprefix = 1
	prefix = ''
	body = ''
	for ch in l:
		if inprefix:
			if ch in splitchar:
				prefix += ch
			else:
				body = ch
				splitprefix.append(prefix)
				prefix = ''
				inprefix = 0
		else:
			if ch in splitchar:
				prefix = ch
				inprefix = 1
				splitbody.append(body)
				body = ''
			else:
				body += ch
	if inprefix:
		splitprefix.append(prefix)
	else:
		splitbody.append(body)
	splittext.append(splitprefix)
	splittext.append(splitbody)
	return splittext


def macsplit(l):
	#	splitchar   = [' ', '	', '(', ')', '[', ']', ',', '-', '\n', '$', ':', '=']
	splitchar = [' ', '	', '(', ')', '[', ']', ',', '-', '\n', ':', '=']
	return macsplit0(l, splitchar)


def macsplit2(l):
	splitchar = [' ', '	', '\n']
	return macsplit0(l, splitchar)


def macjoin(s):
	l = ''
	i = -1
	for body in s[1]:
		i += 1
		l += s[0][i]
		l += body
	if len(s[0]) > len(s[1]): l += s[0][i + 1]
	return l


def a_section(typ, inf, outf):
	return {"codesec": ".text", "datasec": ".data", \
			"datetime": time.strftime('%d.%m.%Y %H:%M:%S', time.localtime()), \
			"file": outf, "version": "mips." + typ + ".0.1", "source": inf}


def fwc(l, f):
	f.write("//" + l + "\n")


def fwt(l, f):
	f.write("\t" + l + "\n")


def fw(l, f):
	f.write(l + "\n")


def fc(x, f):
	if x[0] not in OPTI:
		f.write("\t" + x[0])
		if len(x) > 1:
			f.write("\t" + ", ".join(x[1:]))
		f.write("\n")
	else:
		f.write("\t" + x[1])
		if len(x) > 2:
			f.write("\t" + ", ".join(x[2:]))
		f.write("\t\t\t// # ### " + OPTI[x[0]] + "\n")


def fcc(x, f):
	f.write("\t" + x[0])
	if len(x) > 1:
		f.write("\t" + ", ".join(x[1:]))
	f.write("\t\t# DeadCode\n")


# Дробление одного участка, если есть ключ - использовать в ЛУ
# дробит (пока!) максимум на 2 части если длина тела участка больше чем 2*min_len
#		"align":   "X",						// 1й
#		"cmd":     "команда перехода со всеми аргументами",	// последний
#		"func":    "имя_функции"				// все
#		"globl":   "аргумент директивы в заголовке ЛУ",		// 1й
#		"label":   "метка",					// 1й
#		"lunum":   номер участка				// все, в 1м - оригинальный, в остальных - в соответствии с номерами новых участков
#		"p2align": "4, 0x90",					// 1й
#		"section": ".text",					// все
#		"slot":    "команда в слоте со всеми аргументами",	// последний
#		"next":    номер следующего по тексту участка		// все, в последнем - оригинальный, в остальных - в соответствии с номерами новых участков
#		"target":  номер участка - таргета команды перехода	// последний
#		"txt": [...] и "frg": [...]				// в соответствии с результатами дробления
#
def split_lu(lu0, l,
			 n2):  # lu0 - участок, l - рекомендованная длина при дроблении, n2 - номер нового участка при дроблении
	C = []
	c0 = 0;
	c1 = 0;
	c2 = 0
	T = lu0["txt"]
	c0 = len(T)
	if "cmd" in lu0: c1 += 1
	if "slot" in lu0: c2 += 1
	if c0 < 2 * l:
		C.append(lu0)
	else:
		l1 = c0 // 2
		l2 = c0 - l1
		if T[l1 - 1][0] in calls:  # если команда со слотом - переносим её в следующий участок
			l1 -= 1
			l2 += 1
		lu1 = {"txt": [] + T[:l1]}
		lu2 = {"txt": [] + T[l1:]}
		if "frg" in lu0:
			lu1["frg"] = lu0["frg"][:l1]
			lu2["frg"] = lu0["frg"][l1:]
		if "align" in lu0: lu1["align"] = lu0["align"]
		if "cmd" in lu0: lu2["cmd"] = lu0["cmd"]
		if "func" in lu0:
			lu1["func"] = lu0["func"]
			lu2["func"] = lu0["func"]
		if "globl" in lu0: lu1["globl"] = lu0["globl"]
		if "label" in lu0: lu1["label"] = lu0["label"]
		if "lunum" in lu0:
			lu1["lunum"] = lu0["lunum"]
			lu2["lunum"] = n2
		if "p2align" in lu0: lu1["p2align"] = lu0["p2align"]
		if "section" in lu0:
			lu1["section"] = lu0["section"]
			lu2["section"] = lu0["section"]
		if "slot" in lu0: lu2["slot"] = lu0["slot"]
		if "next" in lu0: lu2["next"] = lu0["next"]
		lu1["next"] = n2
		if "target" in lu0: lu2["target"] = lu0["target"]
		n2 += 1
		C.append(lu1)
		C.append(lu2)
	return n2, c0, c1, c2, C


# поиск директив, описывающих секции с данными
def get_dsection(l):
	if ".data" in l: return 1, ".data"
	if ".bss" in l: return 1, ".bss"
	if ".rodata" in l: return 1, ".rodata"
	if ".comm" in l: return 1, ".bss"
	if ".lcomm" in l: return 1, ".bss"
	if ".section" in l:
		p = l.split()
		p1 = p[1].split(".")
		if p[1][0] == ".":
			return 1, "." + p1[1]
		else:
			return 1, p1[0]
	return 0, ""


def stat01(F):
	L = {}
	D = {}
	C = {}
	for lu in F:
		if "func" in lu:
			f = lu["func"]
			if f in L:
				L[f] += [lu["lunum"]]
			else:
				L[f] = [lu["lunum"]]
				D[f] = 0
			if "target" in lu: D[f] += 1
			if f in C:
				C[f] += len(lu["txt"])
			else:
				C[f] = len(lu["txt"])
			if "cmd" in lu: C[f] += 2  # команда + слот
		else:
			n = ""
			if "lunum" in lu: n = str(lu["lunum"])
			print("Ошибка: для участка", n, "не описано имя функции!")
			sys.exit(2)
	return L, D, C


def stat02(F1, F2):
	L = {}
	D = {}
	C = {}
	for lu in F1:
		if "func" in lu:
			f = lu["func"]
			if f in L:
				L[f][0] += 1
			else:
				L[f] = [1, 0]
				D[f] = [0, 0]
			if "target" in lu: D[f][0] += 1
			if f in C:
				C[f][0] += len(lu["txt"])
			else:
				C[f] = [len(lu["txt"]), 0]
			if "cmd" in lu: C[f][0] += 2  # команда + слот
		else:
			n = ""
			if "lunum" in lu: n = str(lu["lunum"])
			print("Ошибка: для участка", n, "(файл", sys.argv[1] + ")", "не описано имя функции!")
			sys.exit(2)
	for lu in F2:
		if "func" in lu:
			f = lu["func"]
			if f in L:
				L[f][1] += 1
			else:
				print("Ошибка: функция", f, "отсутствует в файле", sys.argv[2])
			if "target" in lu: D[f][1] += 1
			if f in C:
				C[f][1] += len(lu["txt"])
			if "cmd" in lu: C[f][1] += 2  # команда + слот
		else:
			n = ""
			if "lunum" in lu: n = str(lu["lunum"])
			print("Ошибка: для участка", n, "(файл", sys.argv[1] + ")", "не описано имя функции!")
			sys.exit(2)
	return L, D, C


# корявый модуль json все целые ключи преобразует в строки, это вредно для сортировки и визуализации
# возвращаем в ключи инты вместо строк
def int_key(x):
	y = {}
	for key in x: y[int(key)] = x[key]
	return y


# внедрение мертвого кода
#
# Шаблоны мертвого кода из одной команды
#	F - свободный регистр
#	R - регистр общего назначения из списка DR[]
#	N - целое число в интервале 1..32767
#	X - целое число в интервале 1..31
#

DR = ["$a0","$a1","$a2","$a3","$v0","$v1","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$t8","$t9","$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7"]

DC = [	["daddu",  "F", "R", "R"], \
	["daddu",  "F", "R", "R"], \
	["daddu",  "F", "R", "R"], \
	["daddu",  "F", "R", "R"], \
	["daddu",  "F", "R", "R"], \
	["daddiu", "F", "R", "N"], \
	["daddiu", "F", "R", "N"], \
	["daddiu", "F", "R", "N"], \
	["daddiu", "F", "R", "N"], \
	["daddiu", "F", "R", "N"], \
	["dsubu",  "F", "R", "R"], \
	["dsubu",  "F", "R", "R"], \
	["dsubu",  "F", "R", "R"], \
	["dsubu",  "F", "R", "R"], \
	["dsubu",  "F", "R", "R"], \
	["and",    "F", "R", "R"], \
	["andi",   "F", "R", "N"], \
	["or",     "F", "R", "R"], \
	["ori",    "F", "R", "N"], \
	["xor",    "F", "R", "R"], \
	["xori",   "F", "R", "N"], \
#	["sll",    "F", "R", "X"], \	# ### Проверить на К64, исправить Unpredictable и IntegerOverflov Exceptions в эмуляторе
	["dsll",   "F", "R", "X"], \
	["dsll32", "F", "R", "X"], \
#	["srl",    "F", "R", "X"], \	# ### Проверить на К64, исправить Unpredictable и IntegerOverflov Exceptions в эмуляторе
	["dsrl",   "F", "R", "X"], \
	["dsrl32", "F", "R", "X"]  ]



def one_deadcode(r):
	c = DC[random.randint(0, len(DC) - 1)]
	c[1] = r[random.randint(0, len(r) - 1)]
	c[2] = DR[random.randint(0, len(DR) - 1)]
	if c[3] == "R":
		c[3] = DR[random.randint(0, len(DR) - 1)]
	elif c[3] == "N":
		c[3] = str(random.randint(1, 32767))
	elif c[3] == "X":
		c[3] = str(random.randint(1, 31))
	return c


def deadcode(lu, p):
	c3 = 0
	c4 = 0
	lu2 = copy.deepcopy(lu)
	if "frg" in lu:
		frg = lu["frg"]
		txt = []
		for i, line in enumerate(lu["txt"]):
			if len(frg[i]):
				c4 += 1
				if random.randint(1, 100) <= p:
					txt.append(["DeadCode"] + one_deadcode(frg[i]))
					c3 += 1
			txt.append(line)
		lu2["txt"] = txt
	return lu2, c3, c4


#
# перемещение и кодирование участков
#
def targets(F, n):
	t = 0
	for lu in F:
		if "target" in lu and lu["target"] == n:
			if lu["cmd"][0] not in OPTI: t += 1
	return t


def wrong_cmd(txt):
	for l in txt:
		if len(l):
			c = l[0]
			if c in OPTI: c = l[1]
			if c in wcmd: return 1
	return 0

def wrong_arg(txt):
	for l in txt:
		if len(l):
			c = l[-1]
			if "%" in c: return 1
	return 0

# выбор кандидатов для перемещения и кодирования
def mselect(F, FN, fn, PC):  # Секция кода, список функций, все / не все функции, % вероятности
	K = []  # кандидаты
	MV = []  # выбраны для перемещения
	MC = []  # выбраны для кодирования с перемещением
	for i, lu in enumerate(F):
		k = 0  # пока не кандидат
		if fn and lu["func"] in FN: k = 1
		if not fn: k = 1  # может стать кандидатом
		if k:
			if i and i < len(F) - 1:  # не работаем с первым и последним участком
				if len(lu["txt"]) > 1 and len(lu["txt"]) < 11:  # содержит от 2 до 10 машинных команд
					if "next" in F[i - 1] and F[i - 1]["next"] == lu[
						"lunum"]:  # является следующим по тексту для предыдущего участка
						if targets(F, lu[
							"lunum"]) == 0:  # не является таргетом других участков, включая самого себя (кроме ложных переходов)
							if wrong_cmd(lu["txt"]) == 0:  # не содержит "вредных" команд
								K.append(lu["lunum"])  # добавляем в кандидаты
	random.shuffle(K)
	n = len(K) * PC // 200
	MV = copy.deepcopy(K[0:n])
	if PC == 100:
		MC = copy.deepcopy(K[n:])
	else:
		MC = copy.deepcopy(K[n:2 * n])
	K.sort()
	MV.sort()
	MC.sort()
	return K, MV, MC


# печать статистики по выбранным участкам с функциями
def pf(F, L):
	X = {}
	for lu in F:
		if lu["lunum"] in L:
			if lu["func"] in X:
				X[lu["func"]].append(lu["lunum"])
			else:
				X[lu["func"]] = [lu["lunum"]]
	for f in sorted(X):
		t = "\t"
		if len(f) < 5: t += "\t"
		print("  " + f, t + ":", str(len(X[f])) + "\t", sorted(X[f]))


# подсчет количества участков по списку функций
def flen(F, FN):
	x = 0
	for lu in F:
		if lu["func"] in FN: x += 1
	return x


# запись данных о перемещении без кодирования в участок
#	"move" = 0 - перемещение без кодирования
#	"mbuf" = x - номер буфера для перезаписи
#	"mkey" = 0 - ключ для кодирования
#	"mnum" = y - количество команд для перемещения (от начала участка)
#
def set_mv(lu, mcl):
	l = len(lu["txt"])
	if l > 4: l = 4
	if l >= 2:
		lu["move"] = 0
		lu["mkey"] = "00"
		lu["mbuf"] = random.randint(1, mcl)
		lu["mnum"] = random.randint(1, l)
	return lu


# запись данных о перемещении с кодированием в участок
#	"move" = 1 - перемещение с кодированием
#	"mbuf" = x - номер буфера для перезаписи
#	"mkey" = k - ключ для кодирования (2 цифры в hex без 0x)
#	"mnum" = y - количество команд для перемещения с кодированием (от начала участка)
#
def set_mc(lu, mcl):
	l = len(lu["txt"])
	if l > 4: l = 4
	if l >= 2:
		lu["move"] = 1
		lu["mkey"] = hex(random.randint(16, 127))[2:]
		lu["mbuf"] = random.randint(1, mcl)
		lu["mnum"] = random.randint(1, l)
	return lu


#
# запись команд для перемещения и кодирования в ассемблерный текст
#
def mv_code(lu, fr, f):  # участок, свободные регистры, файл для записи
	random.shuffle(fr)
	if "move" in lu:  # все варианты перемещения
		#	if not lu["move"]:	# перемещение без кодирования
		bssmc = "__bssmc_" + str(lu["mbuf"])
		lucmd = "_LU_" + str(lu["lunum"]) + "_cmd"
		fc(["lui", fr[0], "%hi(" + lucmd + ")"], f)
		fc(["addiu", fr[0], fr[0], "%lo(" + lucmd + ")"], f)
		fc(["lui", fr[1], "%hi(" + bssmc + ")"], f)
		fc(["addiu", fr[1], fr[1], "%lo(" + bssmc + ")"], f)
		for i in range(0, lu["mnum"] * 4, 4):
			fc(["lw", fr[2], str(i) + "(" + fr[0] + ")"], f)
			fc(["sw", fr[2], str(i) + "(" + fr[1] + ")"], f)
		# команда перехода по адресу _LU_x_cmd + mnum*4
		fc(["lw", fr[2], "-12(" + fr[0] + ")"], f)
		fc(["sw", fr[2], str(i + 4) + "(" + fr[1] + ")"], f)
		fc(["lw", fr[2], "-8(" + fr[0] + ")"], f)
		fc(["sw", fr[2], str(i + 8) + "(" + fr[1] + ")"], f)
		fc(["lw", fr[2], "-4(" + fr[0] + ")"], f)
		fc(["j", bssmc], f)
		fc(["sw", fr[2], str(i + 12) + "(" + fr[1] + ")"], f)
		fc(["addiu", fr[0], fr[0], str(i + 4)], f)
		fc(["jr", fr[0]], f)
		fc(["nop"], f)
		fw(lucmd + ":", f)
		
def set_random():
	if "_RAND_" in os.environ and os.environ["_RAND_"].isdigit():
		random.seed(a=int(os.environ["_RAND_"]))


###################################################################################

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
	S = int_key(P["split"])
else:
	S = {}
	for lu in F: S[lu["lunum"]] = [lu["lunum"]]	# первичное заполнение секции split

for lu in F:
	N2, c0, c1, c2, c = split_lu(lu, ML, N2)	# дробление
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
print(sys.argv[0], " - дробление ЛУ, минимальный размер участка".encode("utf-8"), sys.argv[3], "машинных команд".encode("utf-8"))
print(sys.argv[1], "==>", sys.argv[2])
print("Всего в исходном файле".encode("utf-8"))
print("       линейных участков :".encode("utf-8"), L0)
print("       машинных команд   :".encode("utf-8"), C0)
print("       команд переходов  :".encode("utf-8"), C1)
print("       команд в слотах   :".encode("utf-8"), C2)
print("    минимальный номер ЛУ :".encode("utf-8"), N0)
print("   максимальный номер ЛУ :".encode("utf-8"), N1)
print("Добавлено новых участков :".encode("utf-8"), L1)

# сохранение выходного файла
P["attr"] = a_section("p", sys.argv[1], sys.argv[2])
F2 = format_json(P)
f2 = open(sys.argv[2], "w")
for line in F2: f2.write(line + "\n")
f2.close()
