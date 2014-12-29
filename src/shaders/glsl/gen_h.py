#!/usr/bin/env python3
import os
import re

# cannot start with a slash
target_dir = '../'

exec_file_dir = os.path.dirname(os.path.realpath(__file__))
target_dir_abs = os.path.join(exec_file_dir, target_dir)

template = """#pragma once
#include <string>
#include "shader_source.h"

static ShaderSource {shader_name} = {{
{vertex_source},
{fragment_source}
}};
"""


os.chdir(exec_file_dir)
files = [f for f in os.listdir() if re.search(r'\.[fv]\.glsl$', f)]

names_found = set()
pairs_found = set()
for filename in files:
    shader_name = filename[:-7]
    if shader_name in names_found:
        pairs_found.add(shader_name)
    else:
        names_found.add(shader_name)


def camelcase(word):
    return ''.join(x.capitalize() or '_' for x in word.split('_'))

for shader_name in pairs_found:
    source = {}
    for suffix in ['.v.glsl', '.f.glsl']:
        with open(shader_name+suffix, 'r') as f:
            content = f.read()
        content = re.sub(r"\"", "\\\"", content)
        content = re.sub(r"(#.*)[\n\r]+", r"\1\\n\n", content)
        content = re.sub(r"(^[\n\r]*|[\n\r]*$)", "\"", content)
        content = re.sub(r"//[^\n\r]*", "", content)
        content = re.sub(r"[ \t]*[\n\r]+", "\n", content)
        content = re.sub(r"[\n]+", "\"\n\"", content)
        source[suffix] = content

    const_name = 'k' + camelcase(shader_name) + 'Shader'
    gen_source = template.format(shader_name=const_name,
                                 vertex_source=source['.v.glsl'],
                                 fragment_source=source['.f.glsl'])

    with open(os.path.join(target_dir_abs, shader_name+'_shader.h'), "w") as f:
        f.write(gen_source)
