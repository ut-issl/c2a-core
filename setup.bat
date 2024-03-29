@echo off
echo setup.bat
echo setting for local environment...
echo.

echo [SET USER HOOKS]
git config --local core.hooksPath Script/GitHooks
echo done.
echo.

echo [SET UP DEVELOP ENVIRONMENT FOR C2A MINIMUM USER SAMPLE]
call :make_link ".\Examples\minimum_user\src\src_core" ".\"
echo done.
echo.

echo [SET UP DEVELOP ENVIRONMENT FOR 2ND OBC USER SAMPLE]
call :make_link ".\Examples\2nd_obc_user\src\src_core" ".\"
echo done.
echo.

echo The process has been completed.
echo Press any key to exit...
pause >nul
exit


:make_link
if exist %1 (
  rmdir /s /q %1
)
mklink /j /d %1 %2

exit /b
