@ECHO OFF
REM @brief sils_defineのコメントアウトを有効化する

REM スクリプト実行時、sils_define.hから
REM ```
REM #define SILS_FW //ここでdefineする！
REM ```
REM と書かれた行を検出し、コメントアウトする。
REM 以下のサイトを参考にした。
REM   文字列置換: https://qiita.com/wagase/items/2180d8911dcc2a748fd3
REM   空行処理  : https://blogs.yahoo.co.jp/kerupani/15344574.html

cd /d %~dp0
setlocal enabledelayedexpansion

for /f "tokens=1* delims=:" %%A in ('findstr /n "^" ..\Settings\sils_define.h') do (
  set line0=%%B

  REM 空の行はechoで表示できないので分岐しておく
  if "%%B" == "" (
    echo.>>..\Settings\SILS_DEFINE.tmp
  ) else (
    set line0=!line0:#define SILS_FW //ここでdefineする！=//#define SILS_FW //ここでdefineする！!
    set line0=!line0:////=//!
    echo !line0!>>..\Settings\SILS_DEFINE.tmp
  )
)

REM 一時ファイルを削除し、変更を反映
rem move SILS_DEFINE.tmp sils_define.h
move ..\Settings\SILS_DEFINE.tmp ..\Settings\sils_define.h
