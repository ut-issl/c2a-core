@echo off
rem FIXME:
rem 現状， 2nd obc user と minimum user で全く同じファイルが大量にあり，管理上めんどくさいため作った
rem そのうちなんとかする
echo sync_with_minimum_user.bat
echo sync with minimum user...
echo.

call :sync_dir ".\src\s2e_mockup\" "..\minimum_user\src\s2e_mockup\"
call :sync_dir ".\src\src_user\BootLoader\" "..\minimum_user\src\src_user\BootLoader\"
call :sync_dir ".\src\src_user\Library\" "..\minimum_user\src\src_user\Library\"
call :sync_dir ".\src\src_user\ReProgramming\" "..\minimum_user\src\src_user\ReProgramming\"
call :sync_dir ".\src\src_user\Script\" "..\minimum_user\src\src_user\Script\"
call :sync_dir ".\src\src_user\Settings\TlmCmd\Ccsds\" "..\minimum_user\src\src_user\Settings\TlmCmd\Ccsds\"

call :sync_file ".\src\src_user\Applications\UserDefined\debug_apps.h" "..\minimum_user\src\src_user\Applications\UserDefined\debug_apps.h"
call :sync_file ".\src\src_user\IfWrapper\Sils\uart_sils_sci_if.hpp" "..\minimum_user\src\src_user\IfWrapper\Sils\uart_sils_sci_if.hpp"
call :sync_file ".\src\src_user\IfWrapper\Sils\wdt_sils.cpp" "..\minimum_user\src\src_user\IfWrapper\Sils\wdt_sils.cpp"
call :sync_file ".\src\src_user\IfWrapper\SilsMockup\README.md" "..\minimum_user\src\src_user\IfWrapper\SilsMockup\README.md"
call :sync_file ".\src\src_user\IfWrapper\SilsMockup\uart_sils.c" "..\minimum_user\src\src_user\IfWrapper\SilsMockup\uart_sils.c"
call :sync_file ".\src\src_user\IfWrapper\SilsMockup\wdt_sils.c" "..\minimum_user\src\src_user\IfWrapper\SilsMockup\wdt_sils.c"
call :sync_file ".\src\src_user\Settings\git_revision_config.h" "..\minimum_user\src\src_user\Settings\git_revision_config.h"
call :sync_file ".\src\src_user\Settings\DriverSuper\driver_buffer_define.h" "..\minimum_user\src\src_user\Settings\DriverSuper\driver_buffer_define.h"
call :sync_file ".\src\src_user\Settings\TlmCmd\common_cmd_packet_define.c" "..\minimum_user\src\src_user\Settings\TlmCmd\common_cmd_packet_define.c"
call :sync_file ".\src\src_user\Settings\TlmCmd\common_tlm_cmd_packet_define.h" "..\minimum_user\src\src_user\Settings\TlmCmd\common_tlm_cmd_packet_define.h"
call :sync_file ".\src\src_user\TlmCmd\block_command_user_settings.c" "..\minimum_user\src\src_user\TlmCmd\block_command_user_settings.c"
call :sync_file ".\src\src_user\TlmCmd\block_command_user_settings.h" "..\minimum_user\src\src_user\TlmCmd\block_command_user_settings.h"
call :sync_file ".\src\src_user\TlmCmd\common_cmd_packet.c" "..\minimum_user\src\src_user\TlmCmd\common_cmd_packet.c"
call :sync_file ".\src\src_user\TlmCmd\common_tlm_cmd_packet.c" "..\minimum_user\src\src_user\TlmCmd\common_tlm_cmd_packet.c"

call :sync_file ".\src\src_user\Test\utils\wings_utils.py" "..\minimum_user\src\src_user\Test\utils\wings_utils.py"
call :sync_file ".\src\src_user\Test\pytest.ini" "..\minimum_user\src\src_user\Test\pytest.ini"


echo.
echo The process has been completed.
echo Press any key to exit...
pause >nul
exit


:sync_dir
if exist %1 (
  rmdir /s /q %1
)
xcopy /e %2 %1
exit /b

:sync_file
xcopy /y %2 %1
exit /b
