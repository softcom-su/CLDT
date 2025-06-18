#!/usr/bin/env python3
#
# Обработка текста на ассемблере MIPS, сгенерированного CLANG-11, (С), Als, 04/2021
# Сохраняет результат в формате .f.json
#
import sys, re, json, time
comment_char = "#"
space_chars = [" ", "\t"]
del_text	= [".file", ".set", ".type", ".size", ".ident", ".addrsig", ".cfi_", ".nan", ".ent", ".frame", ".mask", ".fmask", ".end", ".module"]
del_line	= [".GNU-stack", ".Lfunc_end"]
text_sections	= [".text", ".motor"]
data_sections	= [".data", ".rdata", ".rodata", ".section"]
sections	= text_sections + data_sections
globs		= [".globl", ".global"]
gset		= ["nomicrokomdiv", "nokomdiv16"]
fset		= ["noreorder", "nomacro", "noat"]
dset		= ["reorder", "macro", "at"]
gdir		= [".abicalls",".option"]

def no_comment(l):
	if l[0] == comment_char: return ""
	n  = 1
	for i, char in enumerate(l[1:]):
		if char == comment_char:
			return l[:n] + "\n"
		elif char not in space_chars:
			n = i+2
	return l
def uni_code(l):
	l1 = re.sub("\\\\t", "#1#t#1", l)
	l1 = re.sub("\\\\r", "#1#r#1", l1)
	l1 = re.sub("\\\\n", "#1#n#1", l1)
	l2 = eval('b"""{}"""'.format(l1+"\n"), {},{}).decode()[:-1]
	l2 = re.sub("#1#t#1", "\\\\t", l2)
	l2 = re.sub("#1#r#1", "\\\\r", l2)
	l2 = re.sub("#1#n#1", "\\\\n", l2)
	return l2
def get_line(line):
	global GD
	l = no_comment(line)
	p0 = l.split("\n")[0]
	if len(p0):
		p = p0.split()
		if len(p):
			if p[0] in gdir:
				if len(p) == 1:
					GD.append(p[0])
				else:
					GD.append(p[0] + "\t" + p[1])
			elif p[0] not in del_text and ".cfi_" not in p[0]:
				e = 0
				for x in del_line:
					if x in line: e += 1
				if not e:
					if ".asci" not in line:
						return p0
					else:
						return uni_code(p0)
	return ""
def get_section(p, line):
	global cs, csn
	if p[0] in text_sections:
		cs  = 1
		csn = line
	elif ".motor" in line and ".section" in line:
		cs  = 1
		csn = line
	elif p[0] in data_sections:
		cs = 2
		csn = line
def write_section():
	global D, F, G, s
	if cs == 1 and len(s):
		F[G[-1]] = ([csn] + s)
	elif cs == 2 and len(s):
		D.append([csn] + s)
	s = []
	
if len(sys.argv) != 3:
	print("\t" + sys.argv[0] + " in_file.s out_file.f.json")
	sys.exit(2)
G   = []		# глобалы
F   = {}		# функции
D   = []		# данные
L   = []		# локалы
C   = []		# .comm
GD  = []		# глобальные директивы
cs  = 0			# текущая секция, 0 - неизвестна, 1 - код, 2 - данные
csn = ""		# имя текущей секции
glo = ""		# текущий глобал

T0 = []		# текст файла
for line in open(sys.argv[1], "r"):
	l = get_line(line)
	if len(l): T0.append(l)

s = []		# текст текущей секции
for i, line in enumerate(T0):
	p = line.split()
	if len(p):
		if p[0] in globs:
			write_section()
			G.append(p[1])
		elif not cs:
			get_section(p, line)
		elif cs:
			if p[0] in sections:
				write_section()
				get_section(p, line)
			else:
				if ".comm" in line : 
					C.append(line)
				elif ".local" in line:
					L.append(p[1])
				else:
					s.append(line)
write_section()
if len(L): D = [C] + D	# все .comm в начало данных
jj = {"locals": L, "globals":G, "code":F, "data":D, "fset":fset, "dset":dset, "gset":gset, "gdir":GD}
jj["attr"] = {"codesec": ".text", "datasec": ".data", \
    "datetime": time.strftime('%d.%m.%Y %H:%M:%S',time.localtime()), \
    "file": sys.argv[1], "version": "mips.f.0.1" }
f2 = open(sys.argv[2], "w")
jc = json.dumps(jj, indent = 2, sort_keys=True)
jc = re.sub("\$31", "$ra", jc)
jc = re.sub("\$30", "$fp", jc)
jc = re.sub("\$29", "$sp", jc)
jc = re.sub("\$28", "$gp", jc)
jc = re.sub("\$27", "$k1", jc)
jc = re.sub("\$26", "$k0", jc)
jc = re.sub("\$25", "$t9", jc)
jc = re.sub("\$24", "$t8", jc)
jc = re.sub("\$23", "$s7", jc)
jc = re.sub("\$22", "$s6", jc)
jc = re.sub("\$21", "$s5", jc)
jc = re.sub("\$20", "$s4", jc)
jc = re.sub("\$19", "$s3", jc)
jc = re.sub("\$18", "$s2", jc)
jc = re.sub("\$17", "$s1", jc)
jc = re.sub("\$16", "$s0", jc)
jc = re.sub("\$15", "$t7", jc)
jc = re.sub("\$14", "$t6", jc)
jc = re.sub("\$13", "$t5", jc)
jc = re.sub("\$12", "$t4", jc)
jc = re.sub("\$11", "$t3", jc)
jc = re.sub("\$10", "$t2", jc)
jc = re.sub("\$9", "$t1", jc)
jc = re.sub("\$8", "$t0", jc)
jc = re.sub("\$7", "$a3", jc)
jc = re.sub("\$6", "$a2", jc)
jc = re.sub("\$5", "$a1", jc)
jc = re.sub("\$4", "$a0", jc)
jc = re.sub("\$3", "$v1", jc)
jc = re.sub("\$2", "$v0", jc)
jc = re.sub("\$1", "$at", jc)
jc = re.sub("\$0", "$zero", jc)
f2.write(jc)
f2.close()
