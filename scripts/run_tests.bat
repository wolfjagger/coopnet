@echo off
CD ..
set project_dir=%cd%
CD x64\Release
:test_num_choice

set /p test_num="Tests to run? (all, default=pick)  "
set /p options="Options? (e.g. -s successfull tests not suppressed)  "

2>NULL call :case_%test_num%
if errorlevel 1 call :default_case
	
echo "Done."
exit /B

:case_all

:all_loop
coopnet.test.exe %options%
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

:case_pick

:pick_loop
echo Choose tests to run (either by name of TEST_CASE or [tag]),
echo  or leave blank to repeat previous test.
set /p test_choice="Choice:  "

coopnet.test.exe %options% %test_choice%
echo.
echo.
echo.
echo.
echo.
echo.
echo.
goto pick_loop

:default_case

goto pick_loop
