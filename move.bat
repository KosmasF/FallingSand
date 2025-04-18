setlocal

FOR /F "tokens=*" %%A IN ('objdump -f main.exe ^| findstr /i "architecture"') DO (
    set "ARCH_LINE=%%A"
)

REM Now compare the captured line
IF "%ARCH_LINE%" == "architecture: i386:x86-64, flags 0x0000013b:" (
	copy "SDL2-2.32.4\x86_64-w64-mingw32\bin\SDL2.dll" SDL2.dll
	copy "SDL2_ttf-2.24.0\x86_64-w64-mingw32\bin\SDL2_ttf.dll" SDL2_ttf.dll 
) ELSE (
	copy "SDL2-2.32.4\i686-w64-mingw32\bin\SDL2.dll" SDL2.dll
	copy "SDL2_ttf-2.24.0\i686-w64-mingw32\bin\SDL2_ttf.dll" SDL2_ttf.dll 
)

endlocal