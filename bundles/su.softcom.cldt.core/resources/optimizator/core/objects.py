'''
Common objects for obfuscator processing
Created on 28 апр. 2023 г.

@author: Petr Babanov
'''

import os
import random
import json
import logging
import copy
from datetime import datetime


class ObfObject(object):
    '''
    Abstract class for obfuscation objects,
    included archetecture name and obfuscation version.
    '''

    def __init__(self, config):
        self.config = config

    def get_arch(self):
        return self.config.get_arch_name()

    def get_version(self):
        return self.config.get_version()

    def get_config(self):
        return self.config

    def write_asm_file(self, file):
        pass

    def get_globals(self):
        return self.globals

    def get_locals(self):
        return self.locals

    def get_data(self):
        return self.data

    def write_json(self):
        pass


class C:

    def __init__(self, config, verbose:bool = False):
        self.sections = []
        self.locals = []
        self.globals = []
        self.lu = []
        self.data_objects = []
        self.func_objects = []
        self.additional_sections = []
        self.config = config
        self.label_dict = {}
        self.stat_free_reg = {}
        self.verbose = verbose
        with open(config.get_file()) as file:
            self.lines = file.readlines()
            file.close()
        self.read_sections()
        self.remove_extra_sections()
        to_remove = []
        for section in self.sections:
            if section["handle_objects"]:
                self.read_objects(section)
                to_remove.append(section)
        for section in to_remove:
            self.add_objects(section["objects"])
            self.sections.remove(section)
        self.handle_function()
        self.handle_data()

    def add_objects(self, objects: list):
        for object in objects:
            if object["type"] == "data":
                self.data_objects.append(object)
            else:
                self.func_objects.append(object)

    def rewrite_labels(self):
        for lu in self.lu:
            if "label" in lu and not lu['label'] in self.globals+self.locals:
                new_label = f"LU_{lu['number']}"
                self.label_dict[lu['label']] = new_label
                del lu['label']
        for lu in self.lu:
            if "cmd" in lu:
                self.config.change_jump_command(lu['cmd'], self.label_dict)

    
    def split_lu(self, lu_len):
        for lu in self.lu:
            if len(lu["code"]) > lu_len:
                tmp_lu = lu
                for i in range(lu_len, len(lu['code']), lu_len):
                    tmp_lu = self.separate_lu(tmp_lu, lu_len)
                    self.lu.append(tmp_lu)

    def separate_lu(self, lu:dict, lu_len):
        new_lu = {'number': len(self.lu) + 1, 'code': lu['code'][lu_len:], 'function_name': lu['function_name'], 'free_registers' : lu['free_registers'][lu_len:]}
        if "next" in lu:
            new_lu['next'] = lu['next']
        if "cmd" in lu:
            new_lu['cmd'] = lu['cmd']
            del lu['cmd']
        lu['next'] = new_lu["number"]
        lu['code'] = lu['code'][0:lu_len]
        lu['free_registers'] = lu['free_registers'][0:lu_len]
        return new_lu
    
    def set_free_regs(self):
        for function in self.func_objects:
            free_regs = self.config.get_volatile_regs()
            function["free_registers"] = []
            for line in function['lines']:
                free_regs = self.config.get_free_registers(line, copy.deepcopy(free_regs))
                function['free_registers'].append(free_regs)


    def handle_function(self):
        self.set_free_regs()
        self.split_function()
        self.rewrite_labels()
        for lu in self.lu:
            if not lu['function_name'] in self.stat_free_reg:
                self.stat_free_reg[lu['function_name']] = {}
                for register in self.config.get_free_reg_list():
                    self.stat_free_reg[lu['function_name']][register] = 0
            NRF = self.stat_free_reg[lu["function_name"]]
            for line in lu['code']:
                reg_list = self.config.get_affected_regs(line)
                if len(reg_list) > 0:
                    for reg in reg_list:
                        if reg in NRF:
                            NRF[reg] += 1
            if 'cmd' in lu:
                reg_list = self.config.get_affected_regs(lu['cmd'])
                for reg in reg_list:
                    if reg in NRF:
                        NRF[reg] += 1
            lu['frg'] = self.config.get_frg(lu)
        FRG = {}
        for function in self.stat_free_reg:
            RG = []
            for register in self.stat_free_reg[function]:
                if self.stat_free_reg[function][register] == 0:
                    RG.append(register)
            RG.sort()
            FRG[function] = RG
        self.free = FRG

    def read_sections(self):
        section = {'commands': [], 'type': None}
        skip_section = False
        for line in self.lines:
            if len(line.strip()) == 0 or self.config.is_comment(line) or self.config.skip(line):
                continue
            if self.config.global_dir(line):
                pass
            if self.config.new_section(line):
                if not skip_section:
                    self.sections.append(section)
                skip_section = self.config.skip_section(line) 
                section = {'commands': [], 'type': self.config.get_section_type(line), 'header' : self.config.get_section_header(line), "handle_objects": self.config.handle_objects(line)}
            if not skip_section:
                section['commands'].append(line)
        self.sections.append(section)
        logging.info("read object")
        

    def read_objects(self, section):
        section['objects'] = []
        tmp = None
        new_lines = []
        for line in section['commands']:
            if not self.config.skip(line):
                new_lines.append(line)
        section['commands'] = new_lines
        for line in section['commands']:
            if self.config.symbol(line):
                if self.config.symbol(line) == 'local':
                    self.locals.append(self.config.get_symbol_name(line))
                if self.config.symbol(line) == 'global':
                    self.globals.append(self.config.get_symbol_name(line))
                if tmp and len(tmp):
                    section['objects'].append(tmp)
                tmp = {'type': None, 'lines': []}
            if self.config.obj_type(line):
                if not tmp:
                     tmp = {'type': None, 'lines': []}
                tmp['type'], tmp['name'] = self.config.obj_type(line)
            if tmp:
                tmp['lines'].append(line)
        section['objects'].append(tmp)
        if len(section['objects']) == 1 and section['objects'][0] is None:
            section['objects'][0] = ({'type': None, 'lines': section['commands']})
        for obj in section['objects']:
            if obj['type'] == None:
                if section['type'] == "data":
                    obj["type"] = "data"
                else: 
                    obj['type'] = self.config.get_type(obj)
                    if obj['type'] == 'function':
                        obj['name'] = self.config.get_name(obj)

    def split_function(self):
        number = 0
        for function in self.func_objects:
            if function and function['type'] == 'function':
                lu = None
                for i, line in enumerate(function['lines']):
                    tmp = line.strip().split(maxsplit=1)
                    if self.config.is_label(tmp[0]):
                        if lu:
                            self.lu.append(lu)
                            lu['next'] = number + 1
                        number += 1
                        lu = {'number': number, 'code': [], 'label': self.config.get_label_name(
                            tmp[0]), 'function_name': function['name'], 'free_registers' : []}
                    elif self.config.is_jmp(tmp[0]):
                        lu['cmd'] = tmp
                        number += 1
                        self.lu.append(lu)
                        lu = {'number': number, 'code': [],
                            'function_name': function['name'], 'free_registers' : []}
                    elif self.config.is_cond_jmp(tmp[0]):
                        lu['cmd'] = tmp
                        number += 1
                        lu['next'] = number
                        self.lu.append(lu)
                        lu = {'number': number, 'code': [],
                            'function_name': function['name'], 'free_registers' : []}
                    else:
                        if self.config.is_command(tmp[0]):
                            if (len(tmp) > 1):
                                lu['code'].append([tmp[0].strip()])
                                args = tmp[1].split(',')
                                for arg in args:
                                    lu['code'][-1].append(arg.strip())
                            else:
                                lu['code'].append(tmp)
                            lu['free_registers'].append(function['free_registers'][i])
                self.lu.append(lu)
        print(json.dumps(self.lu, indent=1))

    def handle_data(self):
        for data_obj in self.data_objects:
            to_remove = []
            for line in data_obj['lines']:
                tmp = line.strip().split()
                if self.config.is_label(tmp[0]):
                    data_obj['label'] = self.config.get_label_name(tmp[0])
                    to_remove.append(line)
                elif self.config.skip(tmp[0]):
                    to_remove.append(line)
                elif self.config.new_section(line):
                    data_obj['section'] = line
                    to_remove.append(line)
                elif self.config.is_allign(line):
                    data_obj['allign'] = line
                    to_remove.append(line)
                elif self.config.is_global(line):
                    to_remove.append(line)
            for line in to_remove:
                data_obj['lines'].remove(line)
            

    def remove_extra_sections(self) -> None:
        """removes sections without body or with body full of skipped lines
        """
        for i, section in enumerate(self.sections):
            remove_section = True
            for line in section['commands']:
                if not self.config.skip(line.split()[0]):
                    remove_section = False
                    break
            if remove_section:
                del self.sections[i]

    def insert_dead_data(self) -> None:
        """ insert dead data into data sections
        """        
        pass

    def insert_dead_code(self, functions = None, percentage = 50) -> None:
        """ insert dead data into data sections
        """
        for lu in self.lu:
            if not functions or (lu["func"] in functions):
                self.config.insert_dead_code(lu, percentage)

    def write_asm(self, output_file, mix, dead_code):
        with open(output_file, 'w+') as file:
            file.write(self.config.start_code()+'\n')
            if mix:
                random.shuffle(self.lu)
            for lu in self.lu:
                if 'label' in lu:
                    if lu['label'] in self.globals:
                        file.write(self.config.get_global(lu['label']) + '\n')
                    file.write(self.config.get_label(lu['label']) + '\n')
                file.write(self.config.get_label(f"LU_{lu['number']}") + '\n')
                for i, line in enumerate(lu['code']):
                    file.write(self.config.get_code(line) + '\n')
                    if dead_code and (random.randint(1, 100) < 25):
                        file.write(self.config.get_code(self.config.get_random_command(lu["free_registers"][i]))+ "\n")
                if 'cmd' in lu:
                    file.write(self.config.get_code(lu['cmd']) + '\n')
                if 'next' in lu:
                    file.write(self.config.get_jump_function(lu['next']) + '\n')
            file.write(self.config.end_code()+'\n')
            file.write(self.config.start_data()+'\n')
            for object in self.data_objects:
                if "align" in object:
                    file.write(self.config.get_align(object["align"]) + "\n")
                if "label" in object:
                    file.write(self.config.get_label(object["label"]) + "\n")
                for line in object["lines"]:
                    file.write(self.config.get_code(line.split()) + "\n")
                
            file.write(self.config.end_data()+'\n')
            for section in self.sections:
                for command in section["commands"]:
                    file.write(command + "\n")


class FObject(ObfObject):
    '''
    Object contains separated code,
    data, extracted global symbols and directives
    '''

    def __init__(self, config, input_file, verbose):
        '''
        Constructor
        '''
        super().__init__(config)
        self.verbose = verbose
        self.global_directives = []
        self.globals = []
        self.code = {}
        self.data = []
        self.locals = []
        self.config = config
        with open(input_file) as file:
            logging.info("read file {file_name}".format(file_name=input_file))
            lines = file.read().split('\n')
            section = []
            type = None
            name = ''
            for i, line in enumerate(lines):
                if len(line) > 0:
                    tmp = config.prepare_line(line)
                    if len(tmp) == 0:
                        continue
                    else:
                        t = config.get_type(tmp)
                        logging.info(
                            'line {line} classified as {t}'.format(line=line, t=t))
                        if t == 'label':
                            if section:
                                section.append(line)
                        if t == 'global':
                            if len(section) > 1:
                                self.add_section(section, type, name)
                                section = [line]
                            else:
                                section.append(line)
                            name = config.get_global(tmp)
                            self.globals.append(config.get_global(tmp))
                        if t == 'code':
                            section.append(line)
                        if t == 'data':
                            if section:
                                section.append(line)
                        if t == 'code_section':
                            if len(section):
                                self.add_section(section, type, name)
                            section = [line]
                            type = 'code'
                        if t == 'data_section':
                            if len(section):
                                self.add_section(section, type, name)
                            section = [line]
                            type = 'data'
                        if t == 'global_directive':
                            self.global_directives.append(line)
                            if (len(section) > 0):
                                self.add_data_section(section)
                            section = []
                        if t == 'local_directive':
                            if section:
                                section.append(line)
            if section and len(section):
                if type == 'data':
                    self.add_data_section(section)
                else:
                    self.add_code_section(section, name)
            file.close()
            if self.verbose:
                print('Found local labels:{locals}\nFound global labels: {globals}'.format(
                    locals=self.locals, globals=self.globals))

    def add_section(self, section, type, name=None):
        if (len(section) > 0):
            if type == 'code':
                self.code[name] = section
            elif type == 'data':
                self.data.append(section)
            else:
                raise ValueError('Unexpected value in type parameter')

    def get_functions(self):
        return self.code

    def add_global(self, symbol):
        if not symbol in self.globals:
            self.globals.append(symbol)

    def get_global_directives(self):
        return self.global_directives

    def add_global_directive(self, directive):
        self.global_directives.append(directive)

    def add_code_section(self, section, name):
        new_code = []
        for line in section:
            new_code.append(self.config.prepare_command(line))
        self.code[name] = section

    def add_data_section(self, section):
        self.data.append(section)

    def write_json(self, output):
        result = {}
        result['code'] = self.code
        result['data'] = self.data
        result['attr'] = {'arch': self.config.get_arch(), 'version': self.config.get_version(
        ), 'datetime': datetime.now().strftime('%d/%m/%Y %H:%M:%S')}
        result['globals'] = self.globals
        result['locals'] = self.locals
        result['gset'] = []
        result['gdir'] = []
        result['dset'] = []
        json.dump(result, output)


class LObject(ObfObject):
    '''
    classdocs
    '''

    def __init__(self, f_object):
        '''
        Constructor
        '''
        if not isinstance(f_object, FObject):
            raise Exception("argument must be is instance of" +
                            "obfuscation.common.object.FObject")
        super().__init__(f_object.get_config())
        self.code = {}
        for function in f_object.get_functions():
            self.code[function] = \
                self.config.split_function(function,
                                           f_object.get_functions()[function])
        self.code_section = ".text"
        self.data_section = ".data"
        self.globals = f_object.get_globals()
        self.locals = f_object.get_locals()
        self.data = f_object.get_data()

    def get_code(self):
        return self.code

    def get_code_section(self):
        return self.code_section


class PObject(object):
    '''
    classdocs
    '''

    def __init__(self, l_object):
        '''
        Constructor
        '''
        if not isinstance(l_object, LObject):
            raise Exception("argument must be is instance of" +
                            "obfuscation.common.object.LObject")
        self.config = l_object.get_config()
        self.locals = l_object.get_locals()
        self.globals = l_object.get_globals()
        self.dead_data = []
        self.dead_data_info = {'data': 0, 'bss': 0}
        n = 0
        code = l_object.get_code()
        self.code = []
        S = l_object.get_code_section()
        for function in sorted(code):
            k = {}
            for lu in code[function]:
                k[int(lu)] = code[function][lu]
            for lu in sorted(k):
                if "next" in k[lu]:
                    k[lu]["next"] += n
                if "target" in k[lu]:
                    k[lu]["target"] += n
                if "section" in k[lu]:
                    S = k[lu]["section"]
                else:
                    k[lu]["section"] = S
                k[lu]["lunum"] = lu + n
                k[lu]["func"] = function
                self.code.append(k[lu])
            n += lu
        LM = {}
        self.split_p = {}  # Протокол дробления
        for i, lu in enumerate(self.code):
            if "label" in lu and lu["label"] not in l_object.get_globals():
                LM[lu["label"]] = lu["lunum"]
                del self.code[i]["label"]
                self.split_p[lu["lunum"]] = [lu["lunum"]]
        for i, lu in enumerate(self.code):
            if "cmd" in lu:
                lu['cmd'] = self.config.replace_label(lu['cmd'], LM)
        self.data = l_object.get_data()
        self.max_lu_number = len(self.code)
        self.analyze()

    def write_asm_file(self, file, mix=False):
        if (os.path.exists(file)):
            raise AttributeError("file " + file + " already exists")
        with open(file, "w") as output:
            output.write(
                self.config.get_comment_line(
                    "Файл сгенерирован автоматически"))
            if len(self.globals):
                output.write(self.config.get_global_text(self.globals))
            S = ""
            if mix:
                random.shuffle(self.code)
            for lu in self.code:
                if "section" in lu and lu["section"] != S:
                    S = lu["section"]
                    output.write(self.config.get_section_text(S))
                if "globl" in lu:
                    output.write(self.config.get_global_text(lu["globl"]))
                if "align" in lu:
                    output.write(self.config.get_allign_text(lu["align"]))
                if "p2align" in lu:
                    output.write(self.config.get_p2allign_text(lu["p2align"]))
                if "label" in lu:
                    output.write(self.config.get_label_text(lu["label"]))
                output.write(self.config.get_label_text("_LU_" +
                                                        str(lu["lunum"])))
            # перемещение и кодирование
            #    if "move" in lu:
            #        fmp.mv_code(lu, P["free"][lu["func"]], f2)
            # тело участка
                for command in lu["txt"]:
                    if len(command):
                        output.write(self.config.get_command_text(command)
                                     + "\n")
            # команда перехода и слот
                if "cmd" in lu:
                    output.write(self.config.get_command_text(lu["cmd"])
                                 + "\n")
                if "slot" in lu:
                    output.write(self.config.get_command_text(lu["slot"])
                                 + "\n")
                if mix and "next" in lu:
                    output.write(self.config.get_jmp_lu_command(lu["next"])
                                 + "\n")
            if len(self.data):
                for section in self.data:
                    for line in section:
                        output.write(self.config.get_data_text(line) + "\n")

    def split(self, length):
        new_code_section = []
        for lu in self.code:
            new_lu = self.config.split_lu(lu, length, self.max_lu_number + 1)
            new_code_section += new_lu
            if len(new_lu) > 1:
                self.max_lu_number += 1
                cs = [lu["lunum"] for lu in new_lu]
                if new_lu[0]["lunum"] in self.split_p and \
                   len(self.split_p[new_lu[0]["lunum"]]) > 1:
                    self.split_p[new_lu[0]["lunum"]] = \
                        cs + self.split_p[new_lu[0]["lunum"]][1:]
                else:
                    for x in self.split_p:
                        for i, y in enumerate(self.split_p[x]):
                            if y == cs[0]:
                                self.split_p[x] = self.split_p[x][:i] + \
                                    cs + self.split_p[x][i + 1:]
        self.code = new_code_section

    def analyze(self):
        functions = {}
        self.free_registers = {}
        for lu in self.code:
            if "func" in lu:
                if lu['func'] not in functions:
                    functions[lu['func']] = {
                        name: 0 for name in self.config.get_registers_names()}
                tmp = self.config.get_using_registers(lu)
                for name, using in tmp.items():
                    functions[lu['func']][name] = functions[lu['func']][name] + using
        for name, registers in functions.items():
            RG = []
            for name, usage in registers.items():
                if usage == 0:
                    RG.append(name)
            RG.sort()
            self.free_registers[name] = RG
        for lu in self.code:
            lu['frg'] = self.config.get_frg_section(lu)

    def insert_dead_code(self):
        for lu in self.code:
            self.config.insert_dead_code(lu)

    def insert_dead_data_data(self, value):
        self.dead_data.append(self.config.get_data_section())
        for i in range(value):
            self.dead_data_info['data'] += 1
            self.dead_data[-1] += self.config.get_dead_data_value()

    def insert_dead_data_bss(self, value):
        self.dead_data.append(self.config.get_bss_section())
        for i in range(value):
            self.dead_data_info['bss'] += 1
            self.dead_data[-1] += self.config.get_dead_bss_value()
        pass

    def obfuscate_cfg(self):
        functions = {}
        destinations = {}
        lengths = {}
        for lu in self.code:
            if 'func' in lu:
                function_name = lu['func']
                if function_name in functions:
                    functions[function_name] += [lu['lunum']]
                else:
                    functions[function_name] = [lu['lunum']]
                    destinations[function_name] = 0
                if 'target' in lu:
                    destinations[function_name] += 1
                if function_name in lengths:
                    lengths[function_name] += len(lu['txt'])
                else:
                    lengths[function_name] = len(lu['txt'])
                if 'cmd' in lu:
                    lengths[function_name] += 2
            else:
                raise ValueError("lu" + lu['lununm'] +
                                 'doesn\'t contains func attribute')
        T = {}
        if self.free_registers and \
           self.dead_data_info['bss'] > 0 and\
           self.dead_data_info['data'] > 0:
            for i, lu in enumerate(self.code):
                function_name = lu['func']
                if (len(functions[function_name]) > 4) and\
                   (len(self.free_registers) > 1):
                    if 'next' in lu and\
                       'target' not in lu and\
                       'cmd' not in lu and\
                       'slot' not in lu:
                        ln = lu['lunum']
                        while ln == lu["lunum"] or ln == lu["next"]:
                            ln = functions[function_name][random.randint(
                                0, len(functions[function_name])-1)]
                        rn = random.randint(0, len(self.free_registers[function_name])-2)
                        T[i] = [ln, rn]
        for i, lpn in enumerate(T):
            if not i % 2:
                self.config.insert_dead_edge(lu, T, lpn)

    def __str__(self):
        return 'str:'
