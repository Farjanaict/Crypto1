@echo off
echo Building OSI Communication Simulator...
gcc -Wall -Wextra -std=c11 src\server.c src\layers.c -o server.exe -lws2_32
if errorlevel 1 (
    echo Server build failed. Check that GCC/MinGW is installed and in PATH.
    exit /b 1
)
gcc -Wall -Wextra -std=c11 src\client.c src\layers.c -o client.exe -lws2_32
if errorlevel 1 (
    echo Client build failed. Check the compiler output above.
    exit /b 1
)
echo.
echo Build successful: server.exe and client.exe are ready.
