'''
Created on 26 апр. 2023 г.\

entry point for obfuscator app

@author: Petr Babanov
'''

import argparse
from arch.komdiv import Config2 as komdiv_conf
from arch.intel import Config2 as intel_conf
from core import objects
import sys


def main(arg=None):
    argParser = argparse.ArgumentParser()
    supportedArgs = ["k64", "x64"]
    argParser.add_argument('input_file')
    argParser.add_argument('-o', '--output', help='Имя выходного файла',
                           dest="output", default="out.json")
    argParser.add_argument('-s', '--stat', help='Генерировать отчет',
                           dest="stat", action='store_true')
    argParser.add_argument('-j', '--json', help='Генерировать промежуточные ' +
                           'представления в формате json',
                           dest="json", action='store_true')
    argParser.add_argument('-v', '--verbose', help='Подробный вывод',
                           dest="verbose", action='store_true')
    argParser.add_argument('-l', '--length',
                           help='Максимальная длина линейного участка',
                           dest='len', type=int)
    argParser.add_argument('-m', '--mix',
                           help='Перемешать линейные участки',
                           action='store_true', dest='mix')
    argParser.add_argument('-dc', '--dead-code', dest='dead_code',
                           help='Вставка мертвого кода внутрь участков',
                           action='store_true')
    argParser.add_argument('-ddd', '--dead-data-data', dest='dead_data_d',
                           help='Вставка мертвых данных в секцию data',
                           type=int)
    argParser.add_argument('-ddb', '--dead-data-bss', dest='dead_data_b',
                           help='Вставка мертвых данных в секцию bss',
                           type=int)
    argParser.add_argument('-t', '--target', choices=supportedArgs,
                           help='Целевая архитектура',
                           dest="target", required=True)
    argParser.add_argument('-c', '--cfg', dest="cfg",
                           help='Запутать граф потока управления',
                           action='store_true')

    args, namespace = argParser.parse_known_args(arg)
    config = None
    if(args.target == 'k64'):
        config = komdiv_conf(file=args.input_file, verbose=True)
    elif(args.target == 'x64'):
        config = intel_conf(file=args.input_file, verbose=True)
    obf_obj = objects.C(config)
    if args.len:
        obf_obj.split_lu(args.len)
    obf_obj.write_asm(args.output, args.mix, args.dead_code)


if __name__ == '__main__':
    main(sys.argv[1:])