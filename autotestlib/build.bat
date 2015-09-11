cd ..\generator
release\generator.exe --output-directory=.. ..\autotestlib\global.h ..\autotestlib\build.txt || debug\generator.exe --output-directory=.. ..\autotestlib\global.h ..\autotestlib\build.txt

cd ..\autotestlib
