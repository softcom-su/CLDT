'''
Created on 2 мая 2023 г.

@author: Petr Babanov
'''

from abc import ABC


class ArchConfiguration(ABC):
    '''
    Abstract configuration for archetecture. \n\n
    Subclass constructor must set self name and version to `self.name` and `self.version` as string values\n
    Subclasses must implement follow methods: \n
    `get_global_directives()`\n
    `get_comment_char()`\n
    `get_globals()`\n
    `get_skipped_lines()`\n
    `get_data_sections()`\n
    `get_code_section()`\n
    `split_function()`\n
    `split_lu()`\n
    `replace_label()`\n
    `insert_dead_code()`\n

    '''

    def get_arch_name(self):
        return self.name

    def get_version(self):
        return self.version

    def get_global_directives(self):
        self.raise_error()

    def get_comment_char(self):
        self.raise_error()

    def get_skipped_lines(self):
        self.raise_error()

    def get_globals(self):
        self.raise_error()

    def get_data_section(self):
        self.raise_error()

    def get_code_section(self):
        self.raise_error()

    def split_function(self, function):
        self.raise_error()

    def split_lu(self, lu, length, new_num):
        self.raise_error()
    
    def replace_label(self, command, labels_dict):
        self.raise_error()
    
    def insert_dead_code(self, lu):
        self.raise_error()

    def prepare_line(self, line):
        self.raise_error()

    def prepare_command(self, line):
        self.raise_error()

    def get_type(self, line):
        self.raise_error()

    def check_str(self, line):
        if not isinstance(line, str):
            raise ValueError('expected str object')
        
    def get_global(self, line):
        self.raise_error()

    def raise_error(self):
        raise NotImplementedError('configuration must override this method')
