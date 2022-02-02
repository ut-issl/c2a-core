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



FIXME: 2022/01/24現在  
現在， CTCP 大改修中 (https://github.com/ut-issl/c2a-core/issues/205) であり，現時点では，すべてのパケットの実体は TCP である．
近いうちに整備される予定．

## C2A 標準 Space Packet 定義
TBA


## Common Packet の定義方法
次のように，それぞれ定義する．

- https://github.com/ut-issl/c2a-core/blob/f3197d549559d1f571eb3e28497c9a488611f07f/Examples/minimum_user_for_s2e/src/src_user/Settings/TlmCmd/common_tlm_cmd_packet_define.h#L10-L11
- https://github.com/ut-issl/c2a-core/blob/f3197d549559d1f571eb3e28497c9a488611f07f/Examples/minimum_user_for_s2e/src/src_user/Settings/TlmCmd/common_tlm_packet_define.h#L10-L11
- https://github.com/ut-issl/c2a-core/blob/f3197d549559d1f571eb3e28497c9a488611f07f/Examples/minimum_user_for_s2e/src/src_user/Settings/TlmCmd/common_cmd_packet_define.h#L10-L11


## CCSDS 準拠状況
TBA
