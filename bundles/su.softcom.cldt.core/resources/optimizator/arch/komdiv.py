'''
Created on 7 апр. 2023 г.

@author: Petr Babanov
'''

from core.arch_config import ArchConfiguration
import copy
import random


class Config(ArchConfiguration):
    '''
    classdocs
    '''

    def __init__(self):
        '''
        Constructor
        '''
        self.arch = 'k64'
        self.version = '0.0.1'
        self.global_directives = [".abicalls", ".option"]
        self.skipped_lines = [".file", ".set", ".type", ".size",
                              ".ident", ".addrsig", ".cfi_", ".nan",
                              ".ent", ".frame", ".mask", ".fmask",
                              ".end", ".module", ".loc"]
        self.globals = [".globl", ".global"]
        self.code_sections = [".text", ".motor"]
        self.data_sections = [".data", ".rdata", ".rodata", ".section"]
        self.jmp = ["b", "j", "jr", "jr.hb"]
        self.jcc = ["bc1f", "bc1fl", "bc1t", "bc1tl", "bc2f", "bc2fl", "bc2t",
                    "bc2tl", "beq", "beql", "bgez", "bgezl", "bgtz", "bgtzl",
                    "blez", "blezl", "bltz", "bltzl", "bne", "bnel", "bnez",
                    "beqz", "bge", "bgt", "bgtu", "ble", "blt", "bnez"]
        self.exits = ["exit"]
        self.calls = ["bal", "bgezal", "bgezall", "bltzal",
                      "bltzall", "jal", "jalr", "jalr.hb"]
        self.SC = ['add', 'addi', 'addiu', 'addu', 'and', 'andi', 'cfc1',
                   'cfc2', 'clo', 'clz', 'dadd', 'daddi', 'daddiu', 'daddu',
                   'dclo', 'dclz', 'dmfc0', 'dmfc1', 'dmfc2', 'dneg', 'dnegu',
                   'drotr', 'drotr32', 'drotrv', 'dsbh', 'dshd', 'dsll',
                   'dsll32', 'dsllv', 'dsra', 'dsra32', 'dsrav', 'dsrl',
                   'dsrl32', 'dsrlv', 'dsub', 'dsubu', 'lb', 'lbu', 'ld',
                   'ldl', 'ldr', 'lh', 'lhu', 'lui', 'lw', 'lwl', 'lwr', 'lwu',
                   'mfc0', 'mfc1', 'mfc2', 'mfhc1', 'mfhc2', 'mfhi', 'mflo',
                   'move', 'movf', 'movn', 'movt', 'movz', 'mul', 'neg',
                   'negu', 'nor', 'not', 'or', 'ori', 'rotr', 'rotrv', 'sll',
                   'sllv', 'slt', 'slti', 'sltiu', 'sltu', 'sra', 'srav',
                   'srl', 'srlv', 'sub', 'subu', 'xor', 'xori']
        self.RX = ["$sp", "$fp", "$at", "$k0", "$k1", "$zero",
                   "$gp", "$ra", "$hi", "$lo"]
        self.reg_syn = {"$31": "$ra", "$30": "$fp", "$29": "$sp", "$28": "$gp",
                        "$27": "$k1", "$26": "$k0", "$25": "$t9", "$24": "$t8",
                        "$23": "$s7", "$22": "$s6", "$21": "$s5", "$20": "$s4",
                        "$19": "$s3", "$18": "$s2", "$17": "$s1", "$16": "$s0",
                        "$15": "$t7", "$14": "$t6", "$13": "$t5", "$12": "$t4",
                        "$11": "$t3", "$10": "$t2", "$9": "$t1", "$8": "$t0",
                        "$7": "$a3", "$6": "$a2", "$5": "$a1", "$4": "$a0",
                        "$3": "$v1", "$2": "$v0", "$1": "$at", "$0": "$zero",
                        }
        self.DC = [["daddu", "F", "R", "R"],
                   ["daddu", "F", "R", "R"],
                   ["daddu", "F", "R", "R"],
                   ["daddu", "F", "R", "R"],
                   ["daddu", "F", "R", "R"],
                   ["daddiu", "F", "R", "N"],
                   ["daddiu", "F", "R", "N"],
                   ["daddiu", "F", "R", "N"],
                   ["daddiu", "F", "R", "N"],
                   ["daddiu", "F", "R", "N"],
                   ["dsubu", "F", "R", "R"],
                   ["dsubu", "F", "R", "R"],
                   ["dsubu", "F", "R", "R"],
                   ["dsubu", "F", "R", "R"],
                   ["dsubu", "F", "R", "R"],
                   ["and", "F", "R", "R"],
                   ["andi", "F", "R", "N"],
                   ["or", "F", "R", "R"],
                   ["ori", "F", "R", "N"],
                   ["xor", "F", "R", "R"],
                   ["xori", "F", "R", "N"],
                   # ["sll",    "F", "R", "X"], \
                   # ### Проверить на К64, исправить
                   # Unpredictable и IntegerOverflow Exceptions в эмуляторе
                   ["dsll", "F", "R", "X"],
                   ["dsll32", "F", "R", "X"],
                   # ["srl",    "F", "R", "X"], \
                   # ### Проверить на К64, исправить
                   # Unpredictable и IntegerOverflow Exceptions в эмуляторе
                   ["dsrl", "F", "R", "X"],
                   ["dsrl32", "F", "R", "X"]]
        self.DR = ["$a0", "$a1", "$a2", "$a3", "$v0", "$v1", "$t0", "$t1",
                   "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9",
                   "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7"]
        self.local_directives = ['.previous', '.align']
        self.data = ['.half']

    def split_function(self, function_name, function_text):
        '''
        temporary API, needs to implement function object and use it
        '''
        T = {}
        result = {}
        allign = ""
        p2allign = ""
        text = ""
        n = 1
        prologue = True
        skip_next = False
        for i, line in enumerate(function_text):
            if skip_next:
                skip_next = False
                continue
            if prologue:
                if line.strip() == function_name + ":":
                    prologue = False
                    result[n] = {"align": allign, "p2align": p2allign,
                                 "section": text, "label": function_name,
                                 "txt": []}
                elif line.strip().startswith(".text"):
                    text = line
                elif line.strip().startswith(".align"):
                    allign = line.split()[1]
                elif line.strip().startswith(".p2align"):
                    p2allign = " ".join(line.split()[1:])
            else:
                tmp = line.split()
                if self.is_label(tmp[0]):
                    if len(result[n]["txt"]):
                        T[tmp[0][:-1]] = n + 1
                    else:
                        T[tmp[0][:-1]] = n
                    if i < len(function_text) - 1:
                        result[n]["next"] = n + 1
                    if len(result[n]["txt"]) or \
                            "label" in result[n]:  # так сработает при двух
                        n += 1  # метках подряд (будет
                        # пустой участок с меткой)
                        result[n] = {"txt": []}
                    result[n]["label"] = tmp[0][:-1]
                elif tmp[0] in self.jcc:
                    result[n]["next"] = n + 1
                    result[n]["cmd"] = self.strm(line)
                    result[n]["slot"] = self.strm(function_text[i + 1])
                    n += 1
                    result[n] = {"txt": []}
                elif tmp[0] in self.jmp:
                    result[n]["cmd"] = self.strm(line)
                    result[n]["slot"] = self.strm(function_text[i + 1])
                    n += 1
                    skip_next = True
                    result[n] = {"txt": []}
                else:
                    result[n]["txt"].append(self.strm(line))
        if(prologue):
            return
        if not len(result[n]["txt"]) and \
                "cmd" not in result[n] and \
                "slot" not in result[n] and \
                "label" not in result[n]:
            del result[n]  # удалим последний пустой участок
            if "next" in result[n - 1]:
                del result[n - 1]["next"]  # и ссылку на него

        for lu in result:
            if "cmd" in result[lu]:
                t = result[lu]["cmd"][-1]
                if t in T:
                    result[lu]["target"] = T[t]
            if len(result[lu]["txt"]) > 1 and \
                    "next" in result[lu] and \
                    "target" not in result[lu] and \
                    result[lu]["txt"][-2][-1] in self.exits:
                del result[lu]["next"]
            if "next" in result[lu] and \
                    "cmd" in result[lu] and \
                    result[lu]["cmd"][0] in self.jmp:
                del result[lu]["next"]
        return result

    def split_lu(self, lu, length, new_num):
        if len(lu["txt"]) > 2 * length:
            len1 = len(lu["txt"]) // 2
            len2 = len(lu["txt"]) - len1
            if lu["txt"][len1-1][0] in self.calls:
                len1 -= 1
                len2 += 1
            lu1 = {"txt": lu["txt"][:len1]}
            lu2 = {"txt": lu["txt"][len1:]}
            if 'frg' in lu:
                lu1["frg"] = lu["frg"][:len1]
                lu2["frg"] = lu["frg"][len1:]
            if 'allign' in lu:
                lu1["allign"] = lu["allign"]
            if 'cmd' in lu:
                lu2["cmd"] = lu["cmd"]
            if 'func' in lu:
                lu1["func"] = lu["func"]
                lu2["func"] = lu["func"]
            if 'globl' in lu:
                lu1["globl"] = lu["globl"]
            if 'label' in lu:
                lu1["label"] = lu["label"]
            if 'lunum' in lu:
                lu1["lunum"] = lu["lunum"]
                lu2["lunum"] = new_num
            if 'p2allign' in lu:
                lu1["p2allign"] = lu["p2allign"]
            if 'section' in lu:
                lu1["section"] = lu["section"]
                lu2["section"] = lu["section"]
            if 'slot' in lu:
                lu2["slot"] = lu["slot"]
            if 'next' in lu:
                lu2["next"] = lu["next"]
            lu1["next"] = new_num
            if 'target' in lu:
                lu2["target"] = lu["target"]
            return [lu1, lu2]
        else:
            return [lu]

    def is_label(self, line):
        '''
        check is passed line label
        '''
        tmp = line.split()
        if len(tmp) and tmp[0].endswith(":"):
            return True
        return False

    def strm(self, line):
        tmp = line.split()
        for i, f in enumerate(tmp):
            if f[-1] == ",":
                tmp[i] = tmp[i][:-1]
        return tmp

    def get_comment_line(self, line):
        return "// " + line + "\n"

    def get_global_text(self, lines):
        result = ""
        for line in lines:
            result += ".globl\t" + line + "\n"
        return result

    def get_label_text(self, line):
        return str(line) + ":\n"

    def get_command_text(self, line):
        command = line[0]
        if len(line) > 2:
            args = ', '.join(line[1:])
        elif len(line) == 2:
            args = line[1]
        else:
            args = ""
        return "\t" + ' '.join([command, args])

    def get_comment_char(self):
        return "#"

    def get_data_text(self, line):
        return line

    def get_jmp_lu_command(self, lu_number):
        return "\tj _LU_{}\n\tnop".format(lu_number)

    def get_using_registers(self, lu):
        NR = {"$a0": 0, "$a1": 0, "$a2": 0, "$a3": 0, "$v0": 0, "$v1": 0,
              "$t0": 0, "$t1": 0, "$t2": 0, "$t3": 0, "$t4": 0, "$t5": 0,
              "$t6": 0, "$t7": 0, "$t8": 0, "$t9": 0}
        for line in lu["txt"]:
            if len(line) > 1 and line[1] in NR:
                NR[line[1]] += 1
        if "cmd" in lu and len(lu["cmd"]) > 1 and lu["cmd"][1] in NR:
            NR[lu["cmd"][1]] += 1
        if "slot" in lu and len(lu["slot"]) > 1 and lu["slot"][1] in NR:
            NR[lu["slot"][1]] += 1
        return NR

    def get_registers_names(self):
        return ["$a0", "$a1", "$a2", "$a3", "$v0", "$v1", "$t0", "$t1", "$t2",
                "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9"]

    def prepare_command(self, line):
        for old_name, new_name in self.reg_syn.items():
            line.replace(old_name, new_name)
        return line

    def get_frg_section(self, lu):
        l1 = []
        for line in lu['txt']:
            l1.append([])
            if len(line) > 1 and \
               line[0] in self.SC and \
               line[1] not in " ".join(line[2:]) and \
               line[1] not in self.RX:
                l1[-1].append(line[1])
        l2 = copy.deepcopy(l1)
        for i in range(len(l1)-1, -1, -1):
            if len(l1[i]):
                for j in range(i-1, -1, -1):
                    if l1[i][0] not in " ".join(lu['txt'][j]) and \
                       lu['txt'][j][0] not in self.calls:
                        if l1[i][0] not in l2[j]:
                            l2[j].append(l1[i][0])
                    else:
                        break
        l3 = []
        l4 = []
        for i in range(len(l1)):
            if len(l2[i]):
                l2[i].sort()
                if (i and lu['txt'][i-1][0] not in self.calls) or not i:
                    l4.append(l2[i])
                else:
                    l4.append([])
            else:
                l4.append([])
            l3.append(lu['txt'][i])
        return l4

    def replace_label(self, command, labels_dict):
        if command[-1] in labels_dict:
            command[-1] = '_LU_' + str(labels_dict[command[-1]])
        return command

    def insert_dead_code(self, lu):
        if 'frg' in lu:
            new_txt = []
            for i, line in enumerate(lu['txt']):
                if len(lu['frg'][i]):
                    if random.randint(0, 100) < 50:
                        c = self.DC[random.randint(0, len(self.DC) - 1)]
                        num = random.randint(0, len(lu['frg'][i]) - 1)
                        c[1] = lu['frg'][i][num]
                        c[2] = self.DR[random.randint(0, len(self.DR)-1)]
                        if c[3] == "R":
                            c[3] = self.DR[random.randint(0, len(self.DR)-1)]
                        elif c[3] == "N":
                            c[3] = str(random.randint(1, 32767))
                        elif c[3] == "X":
                            c[3] = str(random.randint(1, 31))
                        new_txt.append([c[0], ','.join(c[1:])])
                new_txt.append(line)
            lu['txt'] = new_txt

    def get_data_section(self):
        return [".data", ".align\t4"]

    def get_dead_bss_value(self):
        return ["\t.long\t0"]

    def get_dead_data_value(self):
        return ["\t.long\t" + hex(random.randint(1, 1000000))]

    def get_bss_section(self):
        return [".bss", ".align\t4"]

    def insert_dead_edge(self, lu, T, lpn):
        lu['target'] = T[lpn][0]
        lu['slot'] = ['nop']
        r1 = self.free_registers[lu["func"]][T[lpn][1]]
        lname = str(random.randint(1, self.dead_data_info["data"]))
        lu["txt"].append(["lui", r1, "%hi" + lname])
        lu["txt"].append(["daddiu", r1, r1, "%lo" + lname])
        if 'frg' in lu:
            for i in range(2):
                lu["frg"].append([])
        lu["cmd"] = ["DeadCFG", "beqz", r1, "_LU_" + str(T[lpn][0])]
        self.code[lpn] = lu

    def get_skipped_lines(self):
        return self.skipped_lines

    def get_globals(self):
        return self.globals

    def get_code_section(self):
        return self.code_sections

    def get_data_section(self):
        return self.data_sections

    def get_dead_bss_value(self):
        return ["\t.long\t0"]

    def get_dead_data_value(self):
        return ["\t.long\t" + hex(random.randint(1, 1000000))]

    def get_bss_section(self):
        return [".bss", ".align\t4"]

    def insert_dead_edge(self, lu, T, lpn):
        lu['target'] = T[lpn][0]
        lu['slot'] = ['nop']
        r1 = self.free_registers[lu["func"]][T[lpn][1]]
        lname = str(random.randint(1, self.dead_data_info["data"]))
        lu["txt"].append(["lui", r1, "%hi" + lname])
        lu["txt"].append(["daddiu", r1, r1, "%lo" + lname])
        if 'frg' in lu:
            for i in range(2):
                lu["frg"].append([])
        lu["cmd"] = ["DeadCFG", "beqz", r1, "_LU_" + str(T[lpn][0])]
        self.code[lpn] = lu

    def prepare_line(self, line):
        self.check_str(line)
        tmp = line.split('#')[0]
        if not tmp in self.skipped_lines:
            return tmp
        return ""

    def get_type(self, lines):
        if not isinstance(lines, list):
            raise ValueError('expected instance of list')
        for line in lines:
            self.check_str(line)
        if(len(lines[0].strip())>0):
            if lines[0] in self.globals:
                return 'global'
            if lines[0] in self.code_sections:
                return 'code_section'
            if lines[0] in self.data_sections:
                return 'data_section'
            if lines[0] in self.global_directives:
                return 'global_directive'
            if lines[0] in self.local_directives:
                return 'local_directive'
            if lines[0] in self.data:
                return 'data'
            elif lines[0][-1:] == ':':
                return 'label'
        return 'code'
    
    def get_global(self, lines):
        if not isinstance(lines, list):
            raise ValueError('expected instance of list')
        for line in lines:
            self.check_str(line)
        if len(lines) != 2:
            raise ValueError('expected string at format ".global <label>"')
        return lines[1]
    
    def prepare_line(self, line):
        content = line.split(self.get_comment_char())[0].strip().split()
        if len(content) == 0 or \
            content[0] in self.get_skipped_lines():
            return []
        if len(content) == 1:
            return content
        else:
            return [content[0]] + content[1].split(',')
        
class Config2:

    def __init__(self, file, verbose):
        self.skipped_lines = [".file", ".set", ".type", ".size",
                              ".ident", ".addrsig", ".cfi_", ".nan",
                              ".ent", ".frame", ".mask", ".fmask",
                              ".end", ".module", ".loc"]
        
        self.code_sections = [".text", ".motor"]
        self.data_sections = [".data", ".section"]
        self.file = file

    def skip_section(self, line:str) -> bool:
        tmp = line.split()
        if len(tmp) ==  2:
            return tmp[0].strip() == '.section' and tmp[1].strip().startswith('.note.')
        return False

    def get_file(self):
        return self.file
    
    def skip_line(self, line):
        tmp = line.replace(',', ' ').split()
        return tmp[0] in self.skipped_lines
    
    def new_section(self, line):
        tmp = line.replace(',', ' ').split()
        return tmp[0] in self.code_sections + self.data_sections
    
    def section_type(self, line):
        tmp = line.replace(',', ' ').split()
        tmp[0] = tmp[0].strip()
        if tmp[0] in self.code_sections:
            return 'code'
        if tmp[0] in self.data_sections:
            return 'data'
    
    def symbol(self, line):
        tmp = line.replace(',', ' ').split()
        tmp[0] = tmp[0].strip()
        if tmp[0] in ['.globl', '.global']:
            return 'global'
        if tmp[0] == '.local':
            return 'local'
        return False
    
    def obj_type(self, line):
        tmp = line.replace(',', ' ').split()
        tmp[0] = tmp[0].strip()
        if tmp[0] == '.type':
            if tmp[2] == '@object':
                return 'data', tmp[1].strip()
            if tmp[2] == '@function':
                return 'function', tmp[1].strip()
            
    def get_label_name(self, line):
        return line[:-1]
    
    def get_global(self, line):
        return '.global ' + line
    
    def global_dir(self, line):
        return None
    
    def get_symbol_name(self, line):
        return line.split()[1].strip()
    
    def is_label(self, line):
        if len(line)>0:
            return line.strip()[-1] == ':'
        return False

    def skip(self, line):
        return line in self.skipped_lines
    
    def start_code(self):
        return '.text'
    
    def start_data(self):
        return '.text'
    
    def end_code(self):
        return ''
    
    def end_data(self):
        return ''
    
    def get_code(self, line):
        if len(line) > 1:
            return '\t' + line[0] + ' ' + ', '.join(line[1:])
        else:
            return '\t' + line[0]
        
    def get_data(self, line):
        return line
    
    def get_label(self, line):
        return line + ':'