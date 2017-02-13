@echo off
CD ..
set project_dir=%cd%
CD x64\Release
:all_loop
coopgephi.exe
echo.
echo.
echo.
echo.
echo.
echo.
echo.
echo Press Enter to run again
set /p null=""
goto all_loop
