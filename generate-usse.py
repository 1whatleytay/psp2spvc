from yaml import load, Loader
from enum import Enum


class Protection(Enum):
    NONE = 0
    SAFE = 1
    DEBUG = 2


protection = Protection.SAFE

bit_types = """
    typedef uint64_t Instruction;
    typedef uint64_t Param;
"""

debug = '#include <stdexcept>\n\n' if protection == Protection.DEBUG else ''

with open('external/usse-decoder-gen/grammar.yaml', 'r') as stream:
    instructions = load(stream, Loader=Loader)

    header = '#pragma once\n\n#include <cstdint>\n\nnamespace usse {' + bit_types + '\n'
    source = '#include <gxp/instructions.h>\n\n' + debug + 'namespace usse {\n'

    for instruction_name, instruction in instructions.items():
        members = instruction['members']
        first = True
        declaration = 'Instruction make' + instruction_name + '('
        parameters = ''
        function = '\t\tInstruction inst = 0;\n'
        index = 64
        for member in members:
            member_name = list(member)[0]
            member_info = member[member_name]
            if type(member_info) is str:
                index -= len(member_info)
                function += '\t\tinst |= 0b' + member_info + 'ull << ' + str(index) + 'u;\n'
            elif type(member_info) is int:
                index -= member_info
                if not first:
                    parameters += ',\n\t\t\t'
                parameters += 'Param/*' + str(member_info) + '*/ ' + member_name
                first = False
                if protection == Protection.DEBUG:
                    function += '\t\tif ((' + member_name + ' & ~0b' + ('1' * member_info) + 'ull) != 0)\n'\
                        + '\t\t\tthrow std::runtime_error("Instruction field ' + member_name\
                        + ' for ' + instruction_name + ' out of bounds.' + '");\n'
                if protection == Protection.NONE:
                    function += '\t\tinst |= ' + member_name + ' << ' + str(index) + 'u;\n'
                else:
                    function += '\t\tinst |= (' + member_name + ' & 0b'\
                        + ('1' * member_info) + 'ull) << ' + str(index) + 'u;\n'
            else:
                if 'offset' in member_info:
                    index = member_info['offset']
                else:
                    index -= member_info['size']
                if 'match' in member_info:
                    function += '\t\tinst |= 0b' + member_info['match'] + 'ull << ' + str(index) + 'u;\n'
                else:
                    if not first:
                        parameters += ',\n\t\t\t'
                    parameters += 'Param/*' + str(member_info['size']) + '*/ ' + member_name
                    first = False
                    if protection == Protection.DEBUG:
                        function += '\t\tif ((' + member_name + ' & ~0b' + ('1' * member_info['size']) + 'ull) != 0)\n'\
                            + '\t\t\tthrow std::runtime_error("Instruction field ' + member_name \
                            + ' for ' + instruction_name + ' out of bounds.' + '");\n'
                    if protection == Protection.NONE:
                        function += '\t\tinst |= ' + member_name + ' << ' + str(index) + 'u;\n'
                    else:
                        function += '\t\tinst |= (' + member_name + ' & 0b'\
                            + ('1' * member_info['size']) + 'ull) << ' + str(index) + 'u;\n'

        if parameters:
            declaration += '\n\t\t\t' + parameters
        declaration += ')'
        header += '\t' + declaration + ';\n\n'
        source += '\t' + declaration + ' {\n' + function + '\t\treturn inst;\n\t}\n\n'

    header += '}'
    source += '}'
    with open('instructions.h', 'w+') as header_out:
        header_out.write(header)
    with open('instructions.cpp', 'w+') as source_out:
        source_out.write(source)
