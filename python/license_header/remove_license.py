import os

include_folder = '../../vs/Source/'
target_folders = ['Geometry', 'Rendering', 'Patterns', 'PrecompiledHeaders']

if __name__ == '__main__':
    # Read license file
    with open('LICENSE', 'r') as f:
        license = f.read()

    if license is None:
        print('Error: License file not found')
        exit(1)

    license += '\n\n'

    # Recursive search of .h files
    for root, dirs, files in os.walk(include_folder):
        # If included in any of target_folders
        if any([root.endswith(folder) for folder in target_folders]):
            for file in files:
                if file.endswith('.h'):
                    # Read content of file
                    with open(os.path.join(root, file), 'r', encoding="ansi") as f:
                        content = f.read()

                    content = content.replace(license, '')

                    # Write back the file
                    with open(os.path.join(root, file), 'w', encoding="ansi") as f:
                        f.write(content)

