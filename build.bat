@echo off
if EXIST "main.exe" (
    del main.exe
)

gcc main2.c BaseLib\src\Memory.c Image\Preprocessed.c Image\CompareMinutiae.c -o main
.\main
@REM python -u "showPicture.py"
