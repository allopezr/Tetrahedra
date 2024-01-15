import os

vs_folder = '../../vs/Source/'
target_folders = ['Geometry', 'Rendering', 'Patterns', 'PrecompiledHeaders', 'Libraries', 'Fonts']
assets_folder = 'Assets/'

clion_folder = '../../clion_linux/'
debug_folder = 'cmake-build-debug-visual-studio-x64/Debug/'
release_folder = 'cmake-build-release-visual-studio-x64/Release/'

# Copy folders from vs to clion
for folder in target_folders:
    # Get absolute path
    vs_path = os.path.abspath(os.path.join(vs_folder, folder))
    clion_path = os.path.abspath(os.path.join(clion_folder, folder))

    os.system('xcopy /s /i /y ' + vs_path + ' ' + clion_path)

# Copy main.cpp
os.system('xcopy /i /y ' + os.path.abspath(os.path.join(vs_folder, 'main.cpp')) + ' ' +
          os.path.abspath(os.path.join(clion_folder)))

# Copy assets folder to clion debug and release
# vs_path = os.path.abspath(os.path.join(vs_folder, assets_folder))
# os.system('xcopy /s /i /y ' + vs_path + ' ' + os.path.abspath(os.path.join(clion_folder, debug_folder)))
# os.system('xcopy /s /i /y ' + vs_path + ' ' + os.path.abspath(os.path.join(clion_folder, release_folder)))
