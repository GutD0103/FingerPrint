@echo off
if EXIST "main.exe" (
    del main.exe
)

gcc main.c BaseLib\src\Memory.c Image\Preprocessed.c Image\CompareMinutiae.c Image\newMath.c -o main
.\main
@REM python -u "showPicture.py"