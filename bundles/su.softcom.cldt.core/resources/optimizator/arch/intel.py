from core.arch_config import ArchConfiguration
import random
import copy
import re


class Config(ArchConfiguration):

    def __init__(self):
        self.version = '0.0.1'
        self.arch = 'x64'
        self.skipped_lines = ['.file', '.ident']
        self.globals = ['.globl']
        self.global_directives = []
        self.local_directives = ['.align']
        self.locals = []
        self.data = ['.value', '.long', '.string']
        self.code_section = ['.text']
        self.data_section = ['.data']
        self.comment_char = '#'

    def prepare_line(self, line):
        content = line.split(self.comment_char)[0].strip().split(maxsplit=1)
        if len(content) == 0 or \
                content[0] in self.skipped_lines:
            return []
        if len(content) == 1:
            return content
        else:
            return [content[0]] + content[1].split('[,\s]')

    def get_type(self, lines):
        if not isinstance(lines, list):
            raise ValueError('expected instance of list')
        for line in lines:
            self.check_str(line)
        if (len(lines[0].strip()) > 0):
            if lines[0] in self.globals:
                return 'global'
            if lines[0] in self.global_directives:
                return 'global_directive'
            if lines[0] in self.local_directives:
                return 'local_directive'
            if lines[0] in self.locals:
                return 'local'
            if lines[0] in self.code_section:
                return 'code_section'
            if lines[0] in self.data_section:
                return 'data_section'
            if lines[0] == '.section':
                tmp = lines[1].split(',')
                if tmp[0] == '.rodata' or \
                   tmp[0] == '.note.gnu.property':
                    return 'data_section'
                if tmp[0] == '.note.GNU-stack' and \
                   tmp[2] == '@progbits':
                    return 'data_section'
            if lines[0] in self.data:
                return 'data'
            if lines[0][-1:] == ':':
                return 'label'
            return 'code'
        return 'none'

    def get_globals(self):
        return self.globals

    def get_global(self, lines):
        if not isinstance(lines, list):
            raise ValueError('expected instance of list')
        for line in lines:
            self.check_str(line)
        if len(lines) != 2 or not lines[0] in self.globals:
            raise ValueError('expected string at format ".globl <label>"')
        return lines[1]

    def remove_comment(self, line):
        self.check_str(line)
        if len(line.strip()) > 0:
            return line.split('#')[0]
        return ""

    def skip_line(self, line):
        self.check_str(line)
        tmp = line.strip().split()
        if tmp[0] in self.skipped_lines:
            return True
        if '.cfi_' in line:
            return True
        return False

    def get_comment_char(self):
        return self.comment_char

    def get_skipped_lines(self):
        return self.skipped_lines

    def get_global_directives(self):
        return self.global_directives

    def get_code_section(self):
        return self.code_section

    def get_data_section(self):
        return self.data_section

    def prepare_command(self, line):
        return line


class Config2():

    def __init__(self, file : str, verbose: bool):
        self.skipped_lines = [".file", ".set", ".type", ".size",
                              ".ident", ".cfi_", ".nan",
                              ".ent", ".frame", ".mask", ".fmask",
                              ".end", ".module", ".loc", ".def", ".scl", ".endef", ".seh_proc", ".seh_endproc", ".seh_endprologue", ".seh_pushreg", ".seh_stackalloc"]
        self.volatile_regs = ["RAX", "RCX", "RDX", "R8", "R9", "R10", "R11"]
        self.non_volatile_regs = ["RBX", "RBP", "RDI", "RSI", "RSP", "R12", "R13", "R14", "R15"]
        self.regs = self.volatile_regs + self.non_volatile_regs
        self.code_sections = [".text", ".motor"]
        self.data_sections = [".data", ".bss"]
        self.file = file
        self.jmp = ['jo', 'jno', 'js', 'jns', 'je', 'jz', 'jne', 'jnz', 'jb', 'jnae', 'jc', 'jnb', 'jae', 'jnc', 'jbe', 'jna',
                    'ja', 'jnbe', 'jl', 'jnge', 'jge', 'jnl', 'jle', 'jng', 'jg', 'jnle', 'jp', 'jpe', 'jnp', 'jpo', 'jcxz', 'jecxz']

        self.alligns = [".p2align"]

        self.free_reg_list = ['ax', 'bx']

        self.calls = ['call', 'lcall']

        self.SC = ['add', 'sub', 'mul', 'mov', 'and', 'not']

        self.data_directives = [".long", ".int", ".short", ".word", ".hword", ".byte", ".asciz", ".ascii"]

        self.dead_code_dict = [
            ["add", "R", "R"],\
            ["add", "N", "R"], \
            ["sub", "R", "R"],\
            ["sub", "N", "R"]
        ]

        self.sizes = [['', 'w', 16], ['e', 'l', 32], ['r', 'q', 64]]

    def is_comment(self, line):
        return line.strip()[0] == "#"
    
    def get_free_registers(self, line, free_regs):
        tmp = line.strip().split()
        if len(tmp) > 2 and self.is_command(tmp[0]):
            reg = self.get_reg_name(tmp[2])
            if reg in free_regs:
                free_regs.remove(reg)
        elif len(tmp) == 2 and re.match(r"pop\D", tmp[0]):
            reg = self.get_reg_name(tmp[1])
            if reg in free_regs:
                free_regs.remove(reg)
#        elif len(tmp) == 2 and re.match(r"call\D", tmp[0]):
#            return self.get_volatile_regs()
        return free_regs

    def get_reg_name(self, reg_name:str):
        tmp = reg_name.strip()[1:].upper()
        if re.match(r"R\d+", reg_name):
            return reg_name
        if not tmp in self.regs:
            tmp = "R" + tmp[1:2] + "X"
        return tmp
    
    def get_random_command(self, regs):
        reg = random.choice(regs)
        return ["movq", f"${random.randint(-65536, 65535)}", f"%{reg.lower()}"]
    
    def get_volatile_regs(self):
        return copy.deepcopy(self.volatile_regs)

    def get_file(self) -> str:
        return self.file
    
    def handle_objects(self, line: str) -> bool:
        return line.split()[0].strip() in self.code_sections + self.data_sections
    
    def is_global(self, line):
        return line.strip().split()[0] in [".global", ".globl"]
    
    def insert_dead_code(self, lu:dict, percentage:int) -> dict:
        if lu['frg']:
            frg = lu['frg']
            txt = []
            for i, line in enumerate(lu['code']):
                if len(frg[i]):
                    if random.randint(1, 100) <= percentage:
                        txt.append(self.get_dead_code_line(frg[i]))
                txt.append(line)
            lu['code'] = txt

    def get_section_header(self, line:str) -> str:
        return line.strip()

    def get_dead_code_line(self, frg:list) -> list:
        command = copy.deepcopy(self.dead_code_dict[random.randint(0, len(self.dead_code_dict) -1)])
        size = self.sizes[random.randint(0, len(self.sizes) - 1)]
        command[0] += size[1]
        for i in range(1, len(command)):
            if command[i] == 'N':
                command[i] = "${val}".format(val = random.randint(-(2**(size[2]/2-1)) + 1, 2**(size[2]/2-1)))
            if(command[i] == 'R'):
                command[i] = "%{reg}".format(reg = size[0] + frg[random.randint(0, len(frg) - 1)])
        return command

    def new_section(self, line: str) -> bool:
        tmp = line.replace(',', ' ').split()
        return tmp[0] in self.code_sections + self.data_sections + [".section"]
        
    
    def is_allign(self, line:str) -> str:
        tmp = line.strip().split()
        if len(tmp) != 2:
            return False
        return tmp[0] in self.alligns

    def symbol(self, line: str):
        tmp = line.replace(',', ' ').split()
        tmp[0] = tmp[0].strip()
        if tmp[0] in ['.globl', '.global']:
            return 'global'
        if tmp[0] == '.local':
            return 'local'
        return False
    
    def skip_section(self, line:str) -> bool:
        tmp = line.split()
        if len(tmp) ==  2:
            return tmp[0].strip() == '.section' and (tmp[1].strip().startswith('.note.') or tmp[1].strip().startswith('.debug'))
        return False
    
    def get_section_type(self, line:str)->str:
        tmp = line.strip().split()
        if len(tmp) == 2 and tmp[0] == ".section":
            typeStr = tmp[1].split(",")[1]
            if typeStr[1] == "d" or typeStr[1] == "b":
                return "data"
            elif typeStr[1] == "e" or typeStr[1] == "x":
                return "code"
            else:
                return "data"
        elif tmp[0] in self.code_sections:
            return 'code'
        elif tmp[0] in self.data_sections:
            return 'data'
        return None
    
    def get_type(self, data:dict) -> str:
        for line in data['lines']:
            tmp = line.strip().split()[0]
            if tmp in self.data_directives:
                return "data"
            elif not self.is_directive(line) and not self.is_label(line):
                return "function"
        return None
    
    def get_name(self, data:dict) -> str:
        for line in data['lines']:
            if line.strip().split()[0] in [".globl"]:
                return line.strip().split()[1]

    def obj_type(self, line: str):
        if self.skip(line):
            return None
        tmp = line.replace(',', ' ').split()
        tmp[0] = tmp[0].strip()
        if tmp[0] == '.type':
            if tmp[2] == '@object':
                return 'data', tmp[1].strip()
            if tmp[2] == '@function':
                return 'function', tmp[1].strip()
        if tmp[0] == '.comm':
            return 'data', tmp[1].strip()

    def get_symbol_name(self, line: str) -> str:
        return line.split()[1].strip()

    def get_label_name(self, line: str) -> str:
        return line[:-1]

    def global_dir(self, line: str):
        return None

    def is_label(self, line: str) -> str:
        return line.strip()[-1] == ':'

    def is_jmp(self, line: str) -> bool:
        return line.strip() == 'jmp' or line.strip() == 'ret'

    def is_cond_jmp(self, line: str) -> bool:
        return line.strip() in self.jmp

    def get_code(self, line: list) -> str:
        if len(line) > 1:
            return '\t' + line[0] + '\t' + ', '.join(line[1:])
        return '\t' + line[0]
    
    def change_jump_command(self, command, dict):
        if len(command) > 1:
            if command[1] in dict:
                command[1] = dict[command[1]]

    def get_jump_function(self, lu_num):
        return f"\tjmp\tLU_{lu_num}"

    def get_data(self, line: str) -> str:
        return line

    def skip(self, line: str) -> bool:
        if(line in self.skipped_lines):
            return True
        else:
            return line.split()[0] in self.skipped_lines

    def get_global(self, line: str) -> str:
        return '\n\t.globl\t' + line

    def get_label(self, line: str) -> str:
        return line + ':'

    def is_command(self, line: str) -> str:
        return not (line in self.skipped_lines or self.is_label(line) or self.is_directive(line))

    def is_directive(self, line: str) -> bool:
        tmp = line.strip()
        if self.is_label(tmp):
            return False
        elif tmp[:1] == ".":
            return True
        return False

    def start_code(self) -> str:
        return '.text'

    def start_data(self) -> str:
        return '.data'

    def end_code(self):
        return ''

    def end_data(self):
        return ''

    def get_free_reg_list(self):
        return self.free_reg_list

    def get_affected_regs(self, command):
        return []
    
    def handle_command(self, command:str) -> bool:
        '''
        check, is this commands in SC list
        '''
        without_suffix = command[:-1]
        return without_suffix in self.SC
    
    def is_register(self, line:str) -> bool:
        '''
        test, if passed line is register name
        '''
        return line.strip()[0] == "%"
    
    def get_register_name(self, line:str) -> str:
        line = line.strip()
        if self.is_register(line):
            if len(line.strip()) == 4:
                return line[2:]
            if line.strip()[:-1] == 'l':
                return line[1] + 'x'
            return line[1:]

    def get_frg(self, lu):
        l1 = [] # для каждой команды - целевой регистр, если до этого в него ничего не писали - можно использовать в мертвом коде
        l4 = []
        for line in lu['code']: # для каждой команды - наличие свободного регистра
            l1.append([])
            if len(line) > 2:
                if self.handle_command(line[0]):
                    if self.is_register(line[2]) and line[2] not in " ".join(line[:1]):
                        if self.get_register_name(line[2]) in self.free_reg_list:
                            l1[-1].append(self.get_register_name(line[2]))
        l2 = copy.deepcopy(l1)
        for i in range(len(l1)-1, -1, -1): # Идем в обратную сторону
            if len(l1[i]): #Если есть нужный регистр тут
                reg_name = l1[i][0]
                for j in range(i-1, -1, -1): # проходим по всем командам вверх
                    command = lu['code'][j]
                    if reg_name not in " ".join(command) and command[0] not in self.calls: # если это не вызов 
                        if reg_name not in l2[j]:  # И если не повторяется
                            l2[j].append(reg_name) # То добавляем в l2
                    else: break
        for i in range(len(l1)):
            if len(l2[i]):
                l2[i].sort
                if (i and lu['code'][i-1][0] not in self.calls) or not i: 
                    l4.append(l2[i])
                else:
                    l4.append([])
            else:
                l4.append([])
        return l4
