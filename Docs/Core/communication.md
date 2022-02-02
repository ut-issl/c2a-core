# Communication

## 概要
通信関連，ネットワーク関連などをまとめる．

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
現在，C2A 標準 Space Packet として，テレメパケット，コマンドパケットともに， Ver.1 のみ策定している．  
ここでは， Ver.1 について記載する．

後方互換性を保つために，バージョン情報は， Secondary Header に埋め込まれている．

### Primary Header
- Primary Header はテレメパケット，コマンドパケットともに共通であり，また， CCSDS の規定する Space Packet と同一である．
- 本ドキュメント更新時の実装は，以下である．ヘッダ構造は以下を参照すること．
  - https://github.com/ut-issl/c2a-core/blob/217c3156a07ec503cd60fc7b75978a3234ec2c5d/TlmCmd/Ccsds/space_packet.h#L1-L35
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


## Common Packet の定義方法
次のように，それぞれ定義する．

- https://github.com/ut-issl/c2a-core/blob/f3197d549559d1f571eb3e28497c9a488611f07f/Examples/minimum_user_for_s2e/src/src_user/Settings/TlmCmd/common_tlm_cmd_packet_define.h#L10-L11
- https://github.com/ut-issl/c2a-core/blob/f3197d549559d1f571eb3e28497c9a488611f07f/Examples/minimum_user_for_s2e/src/src_user/Settings/TlmCmd/common_tlm_packet_define.h#L10-L11
- https://github.com/ut-issl/c2a-core/blob/f3197d549559d1f571eb3e28497c9a488611f07f/Examples/minimum_user_for_s2e/src/src_user/Settings/TlmCmd/common_cmd_packet_define.h#L10-L11


## CCSDS 準拠状況
TBA
