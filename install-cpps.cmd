@echo off

:: set environment variables
setx Path "%Path%";C:\CPPS\bin

:: build and install cpps
make
make install


