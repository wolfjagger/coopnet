@echo off
CD ..
set project_dir=%cd%
CD x64\Debug
:all_loop
coopgephi.exe
echo.
echo.
echo.
echo.
echo.
echo.
echo.
echo Press Enter to run all tests again
set /p null=""
goto all_loop
