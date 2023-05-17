@ECHO OFF
REM print C2A core Git revision as C header

REM current directory(src_core/Script/Git)‚ÉˆÚ“®
cd /d %~dp0

for /f "usebackq" %%a in (`"git log --pretty=format:%%H -1"`) do set git_rev_core=%%a
for /f "usebackq" %%a in (`"git log --pretty=format:%%h -1"`) do set git_rev_core_short=%%a

echo #define GIT_REVISION_C2A_CORE "%git_rev_core%"
echo #define GIT_REVISION_C2A_CORE_SHORT "%git_rev_core_short%"
