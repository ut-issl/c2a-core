# Communication with Components

## 概要
コンポーネント間通信などをまとめる．  
なお，地上局との通信関連や， CCSDS などについて，衛星内コンポーネントのネットワーク関連などについては [Core Layer/Communication](../Core/communication.md) を参照のこと．


## 通信レイヤ
コンポーネント間通信などのための通信レイヤについてまとめる．

### ネットワーク層
コンポーネントや地上局，その他のノードでのデータ配送において，エンド・ツー・エンドの配送に責任を持つ．
CCSDS における APID によって制御されるデータ配送はネットワーク層である．

この層を流れるデータ単位を `Packet` と呼ぶ．

C2A では，地上局との通信や C2A 間通信において， [Core Layer/Communication](../Core/communication.md) の Common Packet が標準的に使用される．  
C2A を搭載した OBC と Common Packet に対応していないその他のコンポーネントでは，後述する EB90 Packet を可能な限り用いる．

### データリンク層
隣接するコンポーネントや地上局，その他のノード間でのデータ転送を行う．
例えば，衛星内のコンポーネント A と B 間での UART での通信に対して責任を持つ．
したがって，この場合は， UART 通信で発生しうるノイズへの耐性を提供することが期待される．

この層を流れるデータ単位を `Frame` と呼ぶ．

C2A では，CCSDS に準拠した地上局との通信では，それに準拠し Transfer Frame が使用される．  
それ以外では，後述する EB90 Frame を可能な限り用いる．


## ネットワーク層 (Packet)
### Common Packet
[Core Layer/Communication](../Core/communication.md) を参照のこと．

### FB90 Packet
Common Packet ほどリッチな情報をやり取りしない通信に用いる．
したがって， User Data Field 以外の Header には，

- Version ID
- Tlm / Cmd Count
- Tlm / Cmd ID

のみ，定義されている．

直接通信する2者を超え，ネットワークを構築するだけの能力はない．  
また，通常データリンク層としては EB90 Frame が用いられる．

現在は Ver.1 のみ策定されており，その定義は以下を参照すること．

https://github.com/ut-issl/c2a-core/blob/182c8d9ca7f67169aebf0756f4928ff51302950b/Drivers/Protocol/eb90_packet_for_driver_super.h#L1-L25

#### 各フィールドの説明
- Version ID
    - `0x00`: バージョン不定
    - `0x01`: Version 1
- Tlm / Cmd Count
    - 送信 Packet 毎にインクリメントされていくカウンタ
- Tlm / Cmd ID
    - Packet 種別を表す ID
- User Data Field
    - バイト単位で格納されたユーザーデータ

なお，すべてのフィールドのバイトオーダはビッグエンディアンとする．


## データリンク層 (Frame)
### FB90 Frame
UART などで接続されたコンポーネント間の通信といった，軽微な通信ノイズの誤り検出のみ必要な場合に用いる．
ネットワーク層として EB90 Packet や Common Packet などが標準的に使用される．

https://github.com/ut-issl/c2a-core/blob/182c8d9ca7f67169aebf0756f4928ff51302950b/Drivers/Protocol/eb90_frame_for_driver_super.h#L1-L33

#### 各フィールドの説明
- STX
    - Frame 先頭識別子
    - `0xEB 0x90` 固定
- Packet Length
    - Packet Field の長さ
- CRC
    - Packet Field 部分の CRC (Header は含めない)
    - 使用する CRC の種類は CRC-16/CCITT-FALSE (CRC-16/AUTOSAR, CRC-16/IBM-3740 とも)
    - `width=16, poly=0x1021, init=0xffff, refin=false, refout=false, xorout=0x0000, check=0x29b1, residue=0x0000`
- ETX
    - Frame 終端識別子
    - `0xC5 0x79` 固定
- Packet Field
    - バイト単位で格納された送信 Packet
    - EB90 Packet や Common Packet などが格納される

なお，すべてのフィールドのバイトオーダはビッグエンディアンとする．


## C2A 間通信
衛星内上の C2A （ないしは互換の Common Packet を用いる FSW）を搭載したコンポーネント（主に OBC）間での通信について記す．
C2A 間通信によって，以下のような機能が提供される．

- OBC 間の簡易な Driver 実装と自動コード生成
    - [c2a-tlm-cmd-code-generator](https://github.com/ut-issl/c2a-tlm-cmd-code-generator) 参照．
    - [`Examples/minimum_user/src/src_user/Drivers/Aocs`](/Examples/minimum_user/src/src_user/Drivers/Aocs/) などの多くのコードが自動生成される．
- OBC と地上局でネットワークを形成．
    - 地上局から MOBC をルーターとして， 2nd OBC へコマンド配送．
    - 2nd OBC のテレメトリを MOBC を経由して地上局まで配送．
    - OBC A から OBC B に対してコマンド発行 / テレメ送信．
    - 他

C2A 間通信の具体的な実装については，本リポジトリに同封されている User Sample である [`Examples/minimum_user`](/Examples/minimum_user) と [`Examples/2nd_obc_user`](/Examples/2nd_obc_user) での通信（前者が MOBC，後者が AOBC を想定）を参考にされたい．
具体的なドライバのコードは以下となる．

- https://github.com/ut-issl/c2a-core/blob/2bf20f854d4201bd7ff45e88515ae72ee0418288/Examples/minimum_user/src/src_user/Drivers/Aocs/aobc.c#L1-L164
- https://github.com/ut-issl/c2a-core/blob/2bf20f854d4201bd7ff45e88515ae72ee0418288/Examples/2nd_obc_user/src/src_user/Drivers/Etc/mobc.c#L1-L150

地上局からのコマンドルーティングについては [Core Layer/Communication#コマンド配送におけるルーティングについて](../Core/communication.md#コマンド配送におけるルーティングについて) などを参照すること．

