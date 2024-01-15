cd D:/
D:
if not exist "Software" mkdir Software
cd Software
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
vcpkg.exe integrate install