#!/usr/bin/env python3
#
# Сводная статистика для оценки сложности обфускации, (С), Als, 08/2021
#
# Команда:
#	python pstat03.py <имя_файла.p.json>
#
import sys, json, copy
OPTI = {"DeadCode":"Мертвый код", "DeadCFG":"Запутывание CFG", "ShuffleJmp":"Переход при перемешивании ЛУ"}
def step1():
	global F, NU, UN, CMD, DCMD, MAXLULEN, MN
	for i, lu in enumerate(F):
		l = len(lu["txt"])
		c = copy.deepcopy(lu["txt"])
		if MAXLULEN < len(c): 
			MAXLULEN = len(c)
			MN = lu["lunum"]
		if "cmd"  in lu: 
			l += 1
			c += lu["cmd"]
		if "slot" in lu: 
			l += 1
			c += lu["slot"]
		for line in c:
			n = 0
			if line[0] in OPTI: n += 1
			if line[n] in CMD:
				CMD[line[n]] += 1
			else:
				CMD[line[n]]  = 1
			if n:
				if line[n] in DCMD:
					DCMD[line[n]] += 1
				else:
					DCMD[line[n]]  = 1
		NU[lu["lunum"]] = i+1
		UN.append(len(c)*4)
def step2():
# второй проход - переходы с учетом последовательных номеров ЛУ, вычисленных при первом проходе
	global F, NU
	for lu in F:
		x = 0; y = 0
		if "next"   in lu: x = NU[lu["next"]]
		if "target" in lu: y = NU[lu["target"]]
		J.append([x, y])
def calc_cmd(x):
	n = 0
	for k in x: n += x[k]
	return n
def calc_targets():
	global J, UN
	u = 0; b = 0; t = 0; ma = 0; mi = 10000000000000; md = 0; r = 0; nn = 0; nl = 0; ps = 0
	for i, lu in enumerate(J):
		if not lu[0] and not lu[1]:
			t += 1
		elif lu[0] and lu[1]:
			u += 1
		else:
			b += 1
		for p in lu:
			if p:
				ps += 1
				pl = 0			# длина перехода в байтах
				if p != i+2: 		# номера участков - с единицы!
					nn += 1
					nl += abs(p-i-1)
				if p < i+2:		# циклы из одного участка тоже считаем переходами назад
					r += 1
					pl = sum(UN[j] for j in range(p, i+1, 1))
				elif p > i+2:
					if p != i+2: pl = sum(UN[j] for j in range(i+1, p-1, 1))
				if ma < pl: ma = pl
				if mi > pl: mi = pl
				md += pl
	if ps: md = md // ps
	return u, b, t, ma, mi, md, r, nn, nl
def calc_mkey():
	global F, NU, UN
	x = 0
	for lu in F:
		if "mkey" in lu:
			x += UN[NU[lu["lunum"]]]
	return x

if len(sys.argv) != 2:
	print("\t" + sys.argv[0] + " in_file.p.json")
	sys.exit(2)

P  = json.loads(open(sys.argv[1], "r").read())
A  = P["attr"]
F  = P["code"]

if A["version"] != "mips.p.0.1":
	print("Неподдерживаемая версия файла " + sys.argv[1]); sys.exit(2)

NU  = {}		# последовательные номера участков с единицы {lunum:номер}
UN  = [0]		# длина тела в байтах по последовательному номеру с единицы
J   = []		# таргеты переходов (последовательные номера участков) [таргет 1 или 0, таргет 2 или 0]
CMD = {}		# {команда:количество}
DCMD= {}		# то же для мертвого кода
MAXLULEN = 0
MN = 0			# номер этого участка

step1(); step2()

#print(" 4. Всего линейных участков      :", len(F))
#print(" 5. Команд мертвого кода         :", calc_cmd(DCMD))
#print(" 6.        из них разных         :", len(DCMD))
#print(" 7. Всего машинных команд        :", calc_cmd(CMD))
#print(" 8.        из них разных         :", len(CMD))
#u, b, t, ma, mi, md, r, nn, nl = calc_targets()
#print(" 9. Команд переходов между ЛУ    :", u+b+t)
#print("10.        из них условных       :", u)
#print("11.        из них безусловных    :", b)
#print("12.        из них без таргетов   :", t)
#print("13. Максимальная длина перехода  :", ma)
#print("14. Минимальная длина перехода   :", mi)
#print("15. Средняя длина перехода       :", md)
#print("16. Переходов (таргетов) назад   :", r)
#print("17. Таргетов не на следующий ЛУ  :", nn)
#print("18.        сумма в участках      :", nl)
#print("19. Измененных байт команд       :", calc_mkey())
#print("37. Процент внедрения мертвого кода в каждой возможной точке внедрения (DEADCODEPROC) - предпоследний параметр при вызове скрипта p2pc.py")
#print("38. Максимальная длина ЛУ (MAXLULEN) :", MAXLULEN, "(номер участка", str(MN)+")" )

the_file = open('stat.txt', 'w', encoding="utf-8")
stat_str = "Всего линейных участков      \t" + str(len(F))
stat_str = stat_str + "\nКоманд мертвого кода         \t" + str(calc_cmd(DCMD))
stat_str = stat_str + "\n       из них разных         \t" + str(len(DCMD))
stat_str = stat_str + "\nВсего машинных команд        \t" + str(calc_cmd(CMD))
stat_str = stat_str + "\n       из них разных         \t" + str(len(CMD))
u, b, t, ma, mi, md, r, nn, nl = calc_targets()
stat_str = stat_str + "\nКоманд переходов между ЛУ    \t" + str(u+b+t)
stat_str = stat_str + "\n       из них условных       \t" + str(u)
stat_str = stat_str + "\n       из них безусловных    \t" + str(b)
stat_str = stat_str + "\n       из них без таргетов   \t" + str(t)
stat_str = stat_str + "\nМаксимальная длина перехода  \t" + str(ma)
stat_str = stat_str + "\nМинимальная длина перехода   \t" + str(mi)
stat_str = stat_str + "\nСредняя длина перехода       \t" + str(md)
stat_str = stat_str + "\nПереходов (таргетов) назад   \t" + str(r)
stat_str = stat_str + "\nТаргетов не на следующий ЛУ  \t" + str(nn)
stat_str = stat_str + "\n       сумма в участках      \t" + str(nl)
stat_str = stat_str + "\nИзмененных байт команд       \t" + str(calc_mkey())
stat_str = stat_str + "\nПроцент внедрения мертвого кода в каждой возможной точке внедрения (DEADCODEPROC)\t" + str(50)
stat_str = stat_str + "\nМаксимальная длина ЛУ (MAXLULEN) \t" + str(MAXLULEN)

the_file.write(stat_str)
the_file.close()