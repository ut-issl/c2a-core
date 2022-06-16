@ECHO OFF

REM @brief git revision –„‚ßž‚Ý

REM current directory‚ÉˆÚ“®
cd /d %~dp0

for /f "usebackq" %%a in (`"git log --pretty=format:%%H -1"`) do set git_rev_user=%%a
for /f "usebackq" %%a in (`"git log --pretty=format:%%h -1"`) do set git_rev_user_short=%%a

cd ..\..\src_core

for /f "usebackq" %%a in (`"git log --pretty=format:%%H -1"`) do set git_rev_core=%%a
for /f "usebackq" %%a in (`"git log --pretty=format:%%h -1"`) do set git_rev_core_short=%%a

cd ..\src_user\Script

if exist ..\Library\git_revision.c del ..\Library\git_revision.c

setlocal EnableDelayedExpansion
for /f "delims=" %%a in (git_revision.tmp) do (
  set line=%%a
  set line=!line:git_rev_core_hash=%git_rev_core%!
  set line=!line:git_rev_core_short_hash=%git_rev_core_short%!
  set line=!line:git_rev_user_hash=%git_rev_user%!
  set line=!line:git_rev_user_short_hash=%git_rev_user_short%!
  echo !line! >> ..\Library\git_revision.c
)
endlocal
