
path=%CD%\devkitadv\bin
gcc -o main.elf main.c
if "%errorlevel%"=="1" pause
objcopy -O binary main.elf TektronicDemo.gba
if "%errorlevel%"=="1" pause
del main.o del main.elf
pause
start VisualBoyAdvance.exe  ".\TektronicDemo.gba"