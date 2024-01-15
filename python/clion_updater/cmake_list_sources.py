import os

include_folder = '../../vs/Source/'
target_folders = ['Geometry', 'Rendering', 'Patterns', 'PrecompiledHeaders', 'Libraries']
substitute_folder = {
    'Geometry': '${GEOMETRY}',
    'Rendering': '${RENDERING}',
    'Patterns': '${PATTERNS}',
    'PrecompiledHeaders': '${PRECOMPILED_HEADERS}',
    'Libraries': '${LIBRARIES}'
}

if __name__ == '__main__':
    # Recursive search of .h, .hpp, .cpp, c files
    file_str = ""

    # Recursive search of .h files in include_folder
    for root, dirs, files in os.walk(include_folder):
        # If included in any of target_folders
        if any([root.endswith(folder) for folder in target_folders]):
            for file in files:
                print(file)
                if file.endswith('.h') or file.endswith('hpp') or file.endswith('.cpp') or file.endswith('c'):
                    file_processed = os.path.join(root, file).split('Source/')[1].replace('\\', '/')
                    for folder in substitute_folder:
                        file_processed = file_processed.replace(folder + '/', substitute_folder[folder] + '/')

                    file_str += '\t' + file_processed + '\n'

    print(file_str)


