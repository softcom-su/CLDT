#!/usr/bin/env python3
import sys, json
if len(sys.argv) <3:
	print("\t" + sys.argv[0] + " out_file.json  infile1.json [infile2.json [...]]")
	sys.exit(2)
J = []
#print("\tГенерация файла", sys.argv[1], "для отображения преобразований CFG")
for fle in sys.argv[2:]:
#	print(fle)
	P  = json.loads(open(fle, "r").read()[7:])
	P[0]["name"] = fle.split("\\")[-1].split(".")[0]
	J.append(P[0])
#print("==>", sys.argv[1])
f2 = open(sys.argv[1], "w")
js = json.dumps(J, indent = 2, sort_keys=True)
f2.write("var O = \n" + js)
f2.close()
