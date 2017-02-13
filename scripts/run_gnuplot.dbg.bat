@echo off
CD ..
set project_dir=%cd%
CD x64\Debug
:all_loop
coopplot.exe
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
