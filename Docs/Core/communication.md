# Communication

## 概要
通信関連，ネットワーク関連などをまとめる．  
コンポーネント間通信については [Driver/Communication with Components](../Driver/communication_with_components.md) などを参照のこと．


## C2A 内部を流れるパケットについて (Common Packet)
C2A 内部を流れるパケットは以下の 3 つである．
- `CommonTlmCmdPacket`
    - CTCP
    - テレコマを区別しないパケット
    - CCSDS で規定される Space Packet に相当する
- `CommonTlmPacket`
    - CTP
    - CTCP のうち，テレメトリパケットに限定したもの
- `CommonCmdPacket`
    - CCP
    - CTCP のうち，コマンドパケットに限定したもの

これらのパケットは，抽象化された型であり，その実体はすべてユーザー定義である．  
C2A 標準として， Space Packet が Core 内で定義されており，基本的にはこれを用いることを想定している．


## C2A 標準 Space Packet 定義
C2A 標準 Space Packet は，CCSDS (Consultative Committee for Space Data Systems．宇宙データシステムの標準化を進めている機関) の Space Packet に準拠したものである（参考: [CCSDS SPACE PACKET PROTOCOL](https://public.ccsds.org/Pubs/133x0b2e1.pdf)）．
CCSDS Space Packet において， Secondary Header はユーザー定義とされており， C2A 用に Secondary Header がカスタムされたものが C2A 標準 Space Packet である．

現在，C2A 標準 Space Packet として，テレメパケット，コマンドパケットともに， Ver.1 のみ策定している．  
ここでは， Ver.1 について記載する．

後方互換性を保つために，バージョン情報は， Secondary Header に埋め込まれている．


### Primary Header
Primary Header はテレメパケット，コマンドパケットともに共通であり，また， CCSDS の規定する Space Packet と同一である．
本ドキュメント更新時の実装は，以下である．ヘッダ構造は以下を参照すること．  
https://github.com/ut-issl/c2a-core/blob/217c3156a07ec503cd60fc7b75978a3234ec2c5d/TlmCmd/Ccsds/space_packet.h#L1-L35

#### 各フィールドの説明
- Packet Version Number
    - `0b000` Space Packet 固定
- Packet Identification
    - Packet Type
        - `0b0`: Telemetry
        - `0b1`: Command
    - Secondary Hreader Flag
        - `0b0`: Secondary Header Absent
        - `0b1`: Secondary Header Present
    - Application Process Identifier (APID)
        - ユーザー定義
        - 以下は CCSDS で規定
            - `0b11111111000` - `0b11111111110`: CCSDS Reserved
            - `0b11111111111`: Idle Packet
- Packet Sequence Control Field
    - Sequence Flag
        - `0b00`: Continuation component of higher data structure
        - `0b01`: First component of higher data structure
        - `0b10`: Last component of higher data structure
        - `0b11`: Standalone Packet
    - Sequence Count
        - APID ごとにパケットの伝送順番を示すカウンタ
- Packet Data Length
    - パケット全長から Primary Header 長を引き，さらに 1 を引いたもの
    - つまり，これが 0 の時， Secondary Header + User Data Field 長は 1 byte である


### Secondary Header (Telemetry)
Secondary Header は CCSDS Space Packet 定義において，ユーザー定義部分であるため，ここで定義する．
本ドキュメント更新時の実装は，以下である．ヘッダ構造は以下を参照すること．  
https://github.com/ut-issl/c2a-core/blob/b84c3d051a1e15ab62c8f1a9744957daa4a62a3f/TlmCmd/Ccsds/tlm_space_packet.h#L1-L51

#### 各フィールドの説明
- Secondary Header Version
    - `0x00`: バージョン不定
    - `0x01`: Version 1
- Board Time
    - テレメトリが生成されたボード (OBC など) の時刻 (TI など)
- Telemetry ID
    - テレメトリID
    - APID 内でユニークであればいい
- Global Time
    - テレメトリが生成された絶対時刻 (unixtime や GPS Time など)
    - APID ごとに解釈方法（ビットフィールドの使い方）を定義する
    - `0xFFFFFFFF 0xFFFFFFFF` の場合， パケット中継中に MOBC (地上局とつながる OBC) で，上書き設定される
        - Global Time を取得できない機器向け
- On-Board Subnetwork Time （将来拡張）
    - 各ボードで作られたパケットの時刻を統一的に管理するために，オンボードサブネットワークで共通の時刻体系に基づくテレメトリ生成時刻
    - `0xFFFFFFFF` の場合， パケット中継中に MOBC (地上局とつながる OBC) で，上書き設定される
        - On-Board Subnetwork Time を取得できない機器向け
- Destination Flags
    - テレメトリ配送種別
    - 同時に複数配送ができるように， flag で管理
    - 今後拡張予定あり
    - 現時点では以下
        - `0b00000001`: Housekeeping Telemetry
        - `0b00000010`: Mission Telemetry
        - `0b00000100`: Stored Telemetry
        - `0b00001000`: Replay Telemetry
        - `0b00010000`: 将来拡張用の確保領域
        - `0b00100000`: 将来拡張用の確保領域
        - `0b01000000`: 将来拡張用の確保領域
        - `0b10000000`: 将来拡張用の確保領域
- Destination Info
    - 例えば，Stored Telemetry 時には Data Recorder のどのパーティションに配送されるかを規定する
    - 将来拡張の可能性あり


### Secondary Header (Command)
Secondary Header は CCSDS Space Packet 定義において，ユーザー定義部分であるため，ここで定義する．
本ドキュメント更新時の実装は，以下である．ヘッダ構造は以下を参照すること．  
https://github.com/ut-issl/c2a-core/blob/b84c3d051a1e15ab62c8f1a9744957daa4a62a3f/TlmCmd/Ccsds/cmd_space_packet.h#L1-L38

#### 各フィールドの説明
- Secondary Header Version
    - `0x00`: バージョン不定
    - `0x01`: Version 1
- Command Type
    - 将来拡張用
    - 現時点では以下
        - `0x00`: 不定
- Command ID
    - コマンドID
    - APID 内でユニークであればいい
- Destination Type
    - `0x0` 以外はユーザー定義
    - 例えば次のように定義する
        - `0x0`: 自分宛
        - `0x1`: MOBC 宛
        - `0x2`: AOBC 宛
        - `0x3`: 不明
    - ここで言う，宛先はコマンド実行場所ではなく，キューイングされる先のことである（詳細は後述）
- Execution Type
    - 現時点では以下（将来拡張予定あり）
        - `0x0`: Ground Station Command
        - `0x1`: Timeline Command (TL0)
        - `0x2`: Block Command
        - `0x3`: Realtime Command
        - `0x4`: Unixtime Timeline Command (TL0)
        - `0x5`: Timeline Command (TL1)
        - `0x6`: Timeline Command (TL2)
        - `0x7`: 不明
    - ルーティングについては後述
- Time Indicator
    - TLC や BC における実行時刻を示す TI

#### コマンド配送におけるルーティングについて
- コマンドの最終的な配送先，つまり実行されるボードは APID によって規定される
    - https://github.com/ut-issl/c2a-core/blob/5d7a9d9b878cf5ddcad4de919e77dcae13df7407/Examples/minimum_user_for_s2e/src/src_user/Settings/TlmCmd/Ccsds/apid_define.h#L9-L13
- 一方で， BC や TLC などでのキューイングは， Destination Type によって決定される
    - https://github.com/ut-issl/c2a-core/blob/6d71249dcdb3aefa1d67ffe8ce946e8d8d8b2a33/Examples/minimum_user/src/src_user/Settings/TlmCmd/common_cmd_packet_define.h#L20-L27
- 具体例（GS と接続される OBC は MOBC とし，AOBC は MOBC にぶら下がってるものとする）
    - APID: MOBC, Destination Type: TO_ME or MOBC
        - GSC: GS から MOBC に届き， MOBC で GSC としてエンキューされる．デキューした後， MOBC 内で GSC として実行される．
        - TLC: GS から MOBC に届き， MOBC で TLC としてエンキューされる．デキューした後， MOBC 内で RTC として実行される．
        - BC: GS から MOBC に届き， MOBC で BC 登録される．BC 展開した後， TL にエンキューされ，デキューした後， MOBC 内で RTC として実行される．
    - APID: AOBC, Destination Type: TO_ME or MOBC
        - GSC: GS から MOBC に届き， MOBC で GSC としてエンキューされる．デキューした後， APID を元に， AOBC へ配送される．配送時， Destination Type は自分宛 (TO_ME) に上書きされ， AOBC で RTC としてキューイング & 実行される．
        - TLC: GS から MOBC に届き， MOBC で TLC としてエンキューされる．デキューした後， APID を元に， AOBC へ配送される．配送時， Destination Type は自分宛 (TO_ME) に上書きされ， AOBC で RTC としてキューイング & 実行される．
        - BC: GS から MOBC に届き， MOBC で BC 登録される．BC 展開した後， TL にエンキューされ，デキューした後， APID を元に， AOBC へ配送される．配送時， Destination Type は自分宛 (TO_ME) に上書きされ， AOBC で RTC としてキューイング & 実行される．
    - APID: AOBC, Destination Type: AOBC
        - GSC: GS から MOBC に届き， MOBC でエンキューされずに，そのまま AOBC へ配送される．配送時， Destination Type は自分宛 (TO_ME) に上書きされ， AOBC で GSC としてキューイング & 実行される．
        - TLC: GS から MOBC に届き， MOBC でエンキューされずに，そのまま AOBC へ配送される．配送時， Destination Type は自分宛 (TO_ME) に上書きされ， AOBC で TLC としてキューイング & 実行される．
        - BC: GS から MOBC に届き， MOBC で BC 登録されずに，そのまま AOBC へ配送される．配送時， Destination Type は自分宛 (TO_ME) に上書きされ， AOBC で BC として登録 & 実行される．
- 地上局 SW での実装まとめ
    - MOBC 宛
        - APID: APID_MOBC_CMD
        - CCP_DEST_TYPE: CCP_DEST_TYPE_TO_ME
    - AOBC 宛（AOBC 直送）
        - APID: APID_AOBC_CMD
        - CCP_DEST_TYPE: CCP_DEST_TYPE_TO_AOBC
    - AOBC 宛（MOBC でキューに入り，実行時に AOBC に転送）
        - APID: APID_AOBC_CMD
        - CCP_DEST_TYPE: CCP_DEST_TYPE_TO_ME


## Common Packet の定義方法
次のように，それぞれ定義する．

- https://github.com/ut-issl/c2a-core/blob/f3197d549559d1f571eb3e28497c9a488611f07f/Examples/minimum_user_for_s2e/src/src_user/Settings/TlmCmd/common_tlm_cmd_packet_define.h#L10-L11
- https://github.com/ut-issl/c2a-core/blob/f3197d549559d1f571eb3e28497c9a488611f07f/Examples/minimum_user_for_s2e/src/src_user/Settings/TlmCmd/common_tlm_packet_define.h#L10-L11
- https://github.com/ut-issl/c2a-core/blob/f3197d549559d1f571eb3e28497c9a488611f07f/Examples/minimum_user_for_s2e/src/src_user/Settings/TlmCmd/common_cmd_packet_define.h#L10-L11


## C2A 間通信について
上記 [#コマンド配送におけるルーティングについて](#コマンド配送におけるルーティングについて) や [Driver/Communication with Components#c2a-間通信](../Driver/communication_with_components.md#c2a-間通信) などを参照すること．


## CCSDS 準拠状況
TBA
