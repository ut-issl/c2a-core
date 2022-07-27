# C2A PYTEST (C2A 間通信用)
- C2A におけるテスト一般については， [Examples/minimum_user/src/src_user/Test](../../../../minimum_user/src/src_user/Test) を参照すること．
- ここでは， `minimum_user` （MOBC に相当）と `2nd_obc_user` （非 MOBC に相当．ここでは AOBC とする）を用いた C2A 間通信のテストについて記載する．

## テスト用 SILS 構成
- WINGS に， MOBC と AOBC の両方の TlmCmd DB を登録する．
    - WINGS の使い方は WINGS Document を参照すること．
- SILS 環境 [S2E User for C2A Core](https://github.com/ut-issl/s2e-user-for-c2a-core) を 2 セット準備し， MOBC，AOBC それぞれを立ち上げる．
    - S2E の使い方は S2E Document を参照すること．
    - この時， MOBC の CCSDS ポートは WINGS の仮想ポートに接続（ループバック）し， MOBC の UART ポートは AOBC の UART ポートに接続（ループバック）させる．
    - デフォルトでは，以下のようになっている．
        - MOBC CCSDS: COM11
        - MOBC UART: COM13
        - AOBC UART: COM14
- テストを実行する．

