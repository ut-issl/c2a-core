@echo off
echo setup.bat
echo setting for local environment...
echo .
echo [SET UP DEVELOP ENVIRONMENT FOR MAXIMUM_USER_SAMPLE_FOR_S2E_BUILD]
if exist ".\Examples\maximum_user_sample_for_s2e_build\src\src_core" (
	rmdir /s /q ".\Examples\maximum_user_sample_for_s2e_build\src\src_core"
)
mklink /j /d ".\Examples\maximum_user_sample_for_s2e_build\src\src_core" .\
echo done.
echo.
echo The process has been completed.
echo Press any key to exit...
pause >nul
