set PATH=%PATH%;C:/Qt/5.15.2/msvc2019_64/bin;
rem set PATH=%PATH%;C:/Qt/Tools/Cmake_64/bin; 
rem set PATH=%PATH%;C:/Qt/Tools/mingw810_64/bin;

cmake -G "Visual Studio 17 2022" -A x64 -B "build"

