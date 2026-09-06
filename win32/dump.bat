set "file=%~n1"

"%~dp0gngeo.exe" --dump -d "%~dp0gngeo_data.zip" -i "%~dp1/" -B "%~dp1/" %file%