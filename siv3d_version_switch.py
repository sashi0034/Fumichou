import re
import sys
import glob

switch_version = '0_6_14'

if __name__ == '__main__':
    if len(sys.argv) == 2:
        switch_version = sys.argv[1]

    project_files = glob.glob('**/*.vcxproj', recursive=True)

    for project_name in project_files:
        with open(project_name, 'r', encoding='utf-8') as file:
            file_contents = file.read()

        updated_contents = re.sub(r'\$\(SIV3D_\d+_\d+_\d+\)', f'$(SIV3D_{switch_version})', file_contents)

        with open(project_name, 'w', encoding='utf-8') as file:
            file.write(updated_contents)
