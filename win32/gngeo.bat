@echo off

if /I "%~x1"==".gno" (
    set "file=%~nx1"
) else (
    set "file=%~n1"
)

if defined file (
    "%~dp0gngeo.exe" -d "%~dp0gngeo_data.zip" -i "%~dp1/" -B "%~dp1/" %file%
) else (
    "%~dp0gngeo.exe" -d "%~dp0gngeo_data.zip"
)