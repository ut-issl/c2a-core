@echo off
echo setup.bat
echo setting for local environment...
echo.
echo [SET USER HOOKS]
copy /Y .\Script\Git\pre-commit.pl .\.git\hooks\pre-commit
echo done.
echo.
echo [SET UP DEVELOP ENVIRONMENT FOR MAXIMUM_USER_SAMPLE_FOR_S2E_BUILD]
if exist ".\Examples\minimum_user_for_s2e\src\src_core" (
  rmdir /s /q ".\Examples\minimum_user_for_s2e\src\src_core"
)
mklink /j /d ".\Examples\minimum_user_for_s2e\src\src_core" .\
echo done.
echo.
echo The process has been completed.
echo Press any key to exit...
pause >nul
