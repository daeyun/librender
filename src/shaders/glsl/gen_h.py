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

    // Generated from {filename} on {today}
    namespace librender {{
    namespace shader {{
    static const std::string {cpp_var} =
    "{source}";
    }}
    }}
    """

    def __init__(self, template=template):
        self.template = textwrap.dedent(self.template).strip()

    def const_from_name(self, name, ending):
        words = re.split(r'[-_ ]+', name) + [ending]
        return 'k' + ''.join(word.capitalize() for word in words)

    def find_source_files(self, regex, path):
        files = [os.path.join(path, f) for f in os.listdir(path)
                 if re.search(regex, f)]

        groups = defaultdict(list)
        for filename in files:
            shader_name = os.path.basename(filename).split('.')[0]
            groups[shader_name] = filename

        return groups.items()

    def transform_string(self, string, substitutions):
        for regex, replacement in substitutions:
            string = re.sub(regex, replacement, string)
        return string

    def generate_cpp_header(self, path):
        filename = os.path.basename(path)
        base = filename.split('.')[0]
        cpp_var_name = self.const_from_name(base, 'Shader')

        with open(path, 'r') as f:
            content = f.read()

        substitutions = [
            (r"/\*[\s\S\r\n]+?\*/", ""),
            (r"//[^\n\r]*", ""),
            (r"\"", "\\\""),
            (r"[\n\r]+", "\n"),
            (r"\n[\n\t ]*\n", "\n"),
            (r"^\n|\n$", ""),
            (r"^[\t ]*(#[a-zA-Z_-][^\n]*)", "\\1\\\\n"),
            (r"(?<=\n)[\t ]*(#[a-zA-Z_-][^\n]*)", "\\\\n\\1\\\\n"),
            (r"\n[\t ]*", ""),
            (r"(\\n)+", "\\\\n"),
            (r"\\n$", ""),
            (r"[\t ]+", " "),
        ]
        source = self.transform_string(content, substitutions)

        return self.template.format(cpp_var=cpp_var_name,
                                    source=source,
                                    today=str(datetime.date.today()),
                                    filename=filename,
                                    )

    def generate_headers(self, source_dir, target_dir):
        files = self.find_source_files(r'\.glsl$', source_dir)

        for base, path in files:
            source = self.generate_cpp_header(path)

            out_filename = os.path.join(target_dir, base + '_shader.h')

            if os.path.exists(out_filename):
                with open(out_filename, 'r') as f:
                    content = f.read()
                if content == source:
                    continue

            print("Writing", out_filename)

            with open(out_filename, "w") as f:
                f.write(source)


def main():
    target_dir = '../'

    source_dir = os.path.dirname(os.path.realpath(__file__))
    target_dir = os.path.join(source_dir, target_dir)

    generator = CppHeaderFromGlsl()
    generator.generate_headers(source_dir, target_dir)


if __name__ == '__main__':
    main()
