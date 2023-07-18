@echo off
if EXIST "main2.exe" (
    del main2.exe
)

gcc main2.c BaseLib\src\Memory.c Image\Preprocessed.c Image\CompareMinutiae.c Image\newMath.c -o main2
.\main2
python -u "showPicture.py"