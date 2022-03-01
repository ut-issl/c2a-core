# Core of Command Centric Architecture

## C2A
- Command Centric Architecture
- OBC 搭載フライトソフトウェアフレームワーク

### C2A core
- 各 C2A で共通利用される C2A の中核部のコード
- 基本的には，各々の C2A user repository （サンプル： TBW） にて， git submodule で参照される．
- core 開発のための最小限の user 部は `Examples/` にあり，これを用いてビルドし，検証する．


## ドキュメント
- ドキュメント:  https://github.com/ut-issl/c2a-core/tree/develop/Docs
- リファレンス:  https://github.com/ut-issl/c2a-reference


## 開発
### セットアップ
1. clone 後， setup.bat or setup.sh を実行

### Kanban (開発マイルストーンなど)
- https://github.com/orgs/ut-issl/projects/1

### SILS (Software-In-the-Loop-Simulation) 環境
- C2A は， [S2E](https://github.com/ut-issl/s2e-core) などの SILS を用いて，フライトソフトウェアがそのまま実行される SILS を構築できる．
- つまり，ターゲットの OBC 上で動くソフトウェアが， PC などの上で，そのままエミュレートできる．
- [`Examples/minimum_user_for_s2e`](./Examples/minimum_user_for_s2e) にある， 最小限の C2A 実行サンプルは， [S2E User for C2A Core](https://github.com/ut-issl/s2e-user-for-c2a-core) によってエミュレーション可能である．

### テスト
- 特定のuserを仮定しないと，各種パラメタが確定しないため， [`Examples/minimum_user_for_s2e`](./Examples/minimum_user_for_s2e) でテストする．
- 詳細は [Test](./Examples/minimum_user_for_s2e/src/src_user/Test) 参照．

### ブランチ
- `main`: リリース版（[詳細](./Docs/General/release.md)）
- `develop`: 概ね検証された最新版（beta機能含む）
- `feature/*` : 開発ブランチ
- `hotfix/*` : 重大バグ修正用ブランチ


## 採用実績・動作実績
C2A Core の採用実績のある衛星 OBC や動作実績のあるボードの情報をまとめる．

| Name | Satellite | &nbsp;&nbsp;&nbsp;Lead&nbsp;Institution&nbsp;&nbsp;&nbsp; | Launch | CPU | &nbsp;&nbsp;&nbsp;Clock&nbsp;&nbsp;&nbsp; | &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ROM&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; | &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;RAM&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; | &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;NVRAM&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; | &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Storage&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; | &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Interface&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; | Reference |
| -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- |
| MOBC | ONGLAISAT | ISSL at the University of Tokyo | - | Renesas Electronics SH-2A | 200 MHz | 2.5 MiB internal ROM | 128 KiB internal RAM, 8 MiB external SRAM | 2 MiB MRAM | 2 GiB NAND flash memory | UART (RS422, LVTTL), CCSDS (LVTTL), GPIO (LVTTL), ADC | [^1] |
| AOBC | ONGLAISAT | ISSL at the University of Tokyo | - | Microchip Technology PIC32MX7 | 80 MHz | 512 KiB internal ROM | 128 KiB internal RAM | 512 KiB FRAM | None | UART (RS422, RS485, LVTTL), SPI, I2C, GPIO (LVTTL), ADC | [^1] |
| TOBC | ONGLAISAT | ISSL at the University of Tokyo | - | Microchip Technology PIC32MX7 | 30 MHz | 512 KiB internal ROM | 128 KiB internal RAM | None | None | UART (LVTTL), I2C, GPIO (LVTTL), ADC | [^1] |
| OBC (2U) | MAGNARO (親機) | Inamori Lab. at Nagoya University | - | STMicroelectronics STM32F4 | 90 MHz | 2 MiB internal ROM | 384 KiB internal RAM, 500 KiB external SRAM | 524 KiB MRAM, 131 KiB EEPROM | 16 GiB SD card | UART, SPI, I2C, GPIO, ADC, DCMI | [^1] |
| OBC (1U) | MAGNARO (子機) | Inamori Lab. at Nagoya University | - | STMicroelectronics STM32F4 | 45 MHz | 2 MiB internal ROM | 384 KiB internal RAM, 500 KiB external SRAM | 524 KiB MRAM, 131 KiB EEPROM | 16 GiB SD card | UART, SPI, I2C, GPIO, ADC, DCMI | [^1] |


[^1]: Ryo Suzumoto, et al. Improvement of C2A (Command-Centric Architecture) Reusability for Multiple Types of OBCs and Development of Continuous Integration Environment for Reliability of Flight Software. _33rd International Symposium on Space Technology and Science_, 2022-f-58, 2022.


## 関連リポジトリ
- https://github.com/ut-issl/tlm-cmd-db
- https://github.com/ut-issl/c2a-tlm-cmd-code-generator
- https://github.com/ut-issl/python-wings-interface
- https://github.com/ut-issl/c2a-enum-loader
- https://github.com/ut-issl/s2e-user-for-c2a-core


## 質問，問い合わせ，その他なんでも
お気軽に [Discussions](https://github.com/ut-issl/c2a-core/discussions) に投稿してください．
