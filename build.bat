@echo off
if EXIST "main.exe" (
    del main.exe
)

gcc main3.c BaseLib\src\Memory.c Image\Preprocessed.c Image\CompareMinutiae.c Image\newMath.c Image\newPreprocessed.c -o main
.\main
python -u "showPicture.py"
