# Communication

## 概要
通信関連，ネットワーク関連などをまとめる．

## C2A 内部を流れるパケットについて
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

これらのパケットは，すべてユーザー定義であるが， C2A 標準を，ここ (TBA) で定義する．
基本的にはこれを用いることを想定している．

FIXME: 2022/01/24現在  
現在， CTCP 大改修中 (https://github.com/ut-issl/c2a-core/issues/205) であり，現時点では，すべてのパケットの実体は TCP である．
近いうちに整備される予定．

## C2A 標準 Space Packet 定義
TBA

## CCSDS 準拠状況
TBA
