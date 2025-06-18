'''
Created on 26 апр. 2023 г.

@author: petrb
'''
from .objects import FObject, LObject, PObject
import re


def uni_code(line):
    l1 = re.sub("\\\\t", "#1#t#1", line)
    l1 = re.sub("\\\\r", "#1#r#1", l1)
    l1 = re.sub("\\\\n", "#1#n#1", l1)
    l2 = eval('b"""{}"""'.format(l1 + "\n"), {}, {}).decode()[:-1]
    l2 = re.sub("#1#t#1", "\\\\t", l2)
    l2 = re.sub("#1#r#1", "\\\\r", l2)
    l2 = re.sub("#1#n#1", "\\\\n", l2)
    return l2


def check_arch_name_version(obj, config):
    if not (object.getName == config.getName()):
        raise Exception("differents arch name: object is " +
                        obj.getName() + " but configuration is " +
                        config.getName())

def generate_f_object(inputFile, config, verbose):
    result = FObject(config, inputFile, verbose)
    return result


def generate_l_object(fObject):
    if not isinstance(fObject, FObject):
        raise Exception("First argument must be instanceof FObject")
    return LObject(fObject)


def generate_p_object(lObject):
    if not isinstance(lObject, LObject):
        raise Exception("First argument must be instanceof LObject")
    return PObject(lObject)
