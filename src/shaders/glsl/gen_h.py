#!/usr/bin/env python3
import os
import re
import datetime
import textwrap
from collections import defaultdict


class CppHeaderFromGlsl:
    """ Convert GLSL files to C++ headers. 2014 Daeyun Shin
    """

    template = """
    #pragma once
    #include <string>
    #include "shader_source.h"

    // Generated from {vertex_filename} and {fragment_filename} on {today}
    static const ShaderSource {cpp_var} = {{
    {vertex_source},
    {fragment_source}
    }};
    """

    def __init__(self, template=template):
        self.template = textwrap.dedent(self.template).strip()

    def const_from_name(self, name, ending):
        words = re.split(r'[-_ ]+', name) + [ending]
        return 'k' + ''.join(word.capitalize() for word in words)

    def find_source_groups(self, regex, path):
        files = [os.path.join(path, f) for f in os.listdir(path)
                 if re.search(regex, f)]

        groups = defaultdict(list)
        for filename in files:
            shader_name = os.path.basename(filename).split('.')[0]
            groups[shader_name].append(filename)

        return groups.items()

    def transform_string(self, string, substitutions):
        for regex, replacement in substitutions:
            string = re.sub(regex, replacement, string)
        return string

    def generate_cpp_header(self, paths):
        filenames = [os.path.basename(path) for path in paths]
        bases = [name.split('.')[0] for name in filenames]
        assert (all([item == bases[0] for item in bases]))
        cpp_var_name = self.const_from_name(bases[0], 'Shader')

        source = {}
        for path in paths:
            with open(path, 'r') as f:
                content = f.read()

            substitutions = [
                (r"/\*[\s\S\r\n]+?\*/[\r\n]*", ""),
                (r"\"", "\\\""),
                (r"(#.*)[\n\r]+", r"\1\\n\n"),
                (r"(^[\n\r]*|[\n\r]*$)", "\""),
                (r"//[^\n\r]*", ""),
                (r"[ \t]*[\n\r]+", "\n"),
                (r"[\n]+", "\"\n\"")
            ]
            source[path[-7:]] = self.transform_string(content, substitutions)

        return self.template.format(cpp_var=cpp_var_name,
                                    vertex_source=source['.v.glsl'],
                                    fragment_source=source['.f.glsl'],
                                    today=str(datetime.date.today()),
                                    vertex_filename=bases[0]+'.v.glsl',
                                    fragment_filename=bases[0]+'.f.glsl',
                                    )

    def generate_headers(self, source_dir, target_dir):
        groups = self.find_source_groups(r'\.[fv]\.glsl$', source_dir)

        for base, paths in groups:
            source = self.generate_cpp_header(paths)

            with open(os.path.join(target_dir, base + '_shader.h'), "w") as f:
                f.write(source)


def main():
    target_dir = '../'

    source_dir = os.path.dirname(os.path.realpath(__file__))
    target_dir = os.path.join(source_dir, target_dir)

    generator = CppHeaderFromGlsl()
    generator.generate_headers(source_dir, target_dir)


if __name__ == '__main__':
    main()
