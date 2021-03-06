@echo off
echo setup.bat
echo setting for local environment...
echo.
echo [SET USER HOOKS]
git config --local core.hooksPath Script/GitHooks
echo done.
echo.
echo [SET UP DEVELOP ENVIRONMENT FOR C2A MINIMUM USER SAMPLE]
if exist ".\Examples\minimum_user\src\src_core" (
  rmdir /s /q ".\Examples\minimum_user\src\src_core"
)
mklink /j /d ".\Examples\minimum_user\src\src_core" .\
echo done.
echo.
echo The process has been completed.
echo Press any key to exit...
pause >nul
