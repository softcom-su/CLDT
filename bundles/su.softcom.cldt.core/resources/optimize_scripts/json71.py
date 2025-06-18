#!/usr/bin/env python3
#
# json7.py 
#	- генерация .json из файла .cfg
#	- числовая нумерация дуг
#	- уровни для рисования для всех выявленных случаев взаимного расположения стрелок
#	- уровни для циклов из одного участка
#
import sys, json
def leveltest(a, n):
	l1 = [i for i in range(a[0], a[1])]
	for arc in arcs:
		l2 = [i for i in range(arc[0], arc[1]+1)]
		for l in l1:
			if l in l2 and arc[2] == n: return 1
	return 0
def min_free_level(a, m):
	for i in range(1, m):
		if not leveltest(a, i): return i
	return a[2]
def leveltest2(a, n):
	l1 = [i for i in range(a[1], a[0])]
	for arc in arcs:
		l2 = [i for i in range(arc[1], arc[0])]
		for l in l1:
			if l in l2 and arc[2] == n: 
#				print("lt2", n, 1)
				return 1
#	print("lt2", n, 0)
	return 0
def max_free_level(a, m):
	for i in range(1, -m):
		if not leveltest2(a, -i): return -i
	return a[2]
	
if len(sys.argv) != 4:
	print("\t" + sys.argv[0] + " in_file.cfg  out_file.json x")
	sys.exit(2)
# x=0 - не в массиве, x!=0 - в массиве
testprint1 = True	# True - печать отладочной информации
testprint1 = False
fl  = open(sys.argv[1], "r").read().split("\n")
fle = [l.split() for l in fl]
if not len(fle[-1]): fle.pop(-1)
end  = ["R", "E"]
pin  = {}
pon  = {}
arcs = []
prop = {0:1}
lu   = []
for i, l in enumerate(fle): 
	pin[i]    = l[0]
	pon[l[0]] = i
	lu.append(l[0])
for l in fle: 
	if l[1] in end:
		prop[pon[l[0]]] = 3
	elif len(l) > 1:
		for a in l[1:]:
			arcs.append([pon[l[0]], pon[a], 0, 0])
arcs.sort()
for i, l in enumerate(arcs): arcs[i][3] = i+1		# нумерация дуг
#
# начальные значения уровней
#
for i, arc in enumerate(arcs):
	if arc[0]+1 == arc[1]: arcs[i][2] = 0
	if arc[1]+1 == arc[0]: arcs[i][2] = 0
#	if arc[0]+1 <  arc[1]: arcs[i][2] = 1
	if arc[0]+1 <  arc[1]: arcs[i][2] = 2		# ### 71.py
	if arc[0]   == arc[1]: arcs[i][2] = 1		# ### 71.py
	if arc[1]   <  arc[0]: arcs[i][2] = -1
#
# дуги вперед и циклы из одного участка
#
for k in range(len(arcs)):
	for i, arc in enumerate(arcs):
		if arc[1] > arc[0]+1:
			maxlevel = 0
			for arc2 in arcs:
# если начало внутри дуги - увеличиваем уровень
				if arc[0] <= arc2[0] and arc[1] > arc2[0]:
					if arc2[2] > maxlevel: maxlevel = arc2[2]
			if maxlevel: arcs[i][2] = maxlevel + 1
# ### если ниже полученного на всем протяжении дуги есть свободный уровень ### уменьшаем до него ###

maxlevel = 0
if len(arcs): maxlevel = max([arc[2] for arc in arcs])

arcs.sort(reverse=True)		# ### сильно помогает
print("maxlevel1", maxlevel)
for i, arc in enumerate(arcs):
	if arc[1] > arc[0]+1:
		x = min_free_level(arc, maxlevel)
		if arc[2] > x: arcs[i][2] = x
arcs.sort()

maxlevel = 0
if len(arcs): maxlevel = max([arc[2] for arc in arcs])

print("maxlevel2", maxlevel)
#
# дуги назад
#
for k in range(len(arcs)):
	for i, arc in enumerate(arcs):
		if arc[1] < arc[0]:
			maxlevel = 0
			for arc2 in arcs:
# если начало внутри дуги - уменьшаем уровень
				if arc[0] >= arc2[0] and arc[1] < arc2[0]:
					if arc2[2] < maxlevel: maxlevel = arc2[2]
			if maxlevel: arcs[i][2] = maxlevel - 1
# ### для дуг назад (если будет надо) можно повторить аналог min_free_level ### пока не сделал ###

minlevel = 0
if len(arcs): minlevel = min([arc[2] for arc in arcs])

print("minlevel1", minlevel)
for i, arc in enumerate(arcs):
	if arc[1] < arc[0]:
		x = max_free_level(arc, minlevel)
#		print(x, arcs[i][2])
		if arc[2] < x: arcs[i][2] = x

minlevel = 0
if len(arcs): minlevel = min([arc[2] for arc in arcs])

print("minlevel2", minlevel)
if int(sys.argv[3]):
	jj = [{"lu":lu, "arc":arcs, "prop":prop}]
else:
	jj = {"lu":lu, "arc":arcs, "prop":prop}
f2 = open(sys.argv[2], "w")
f2.write("var O = \n" + json.dumps(jj, indent = 2, sort_keys=True))
f2.close()