# Tips in Core Layer

## 概要
C2A Coreを理解する上で，混乱しやすいポイントをまとめる．


## OBC Timeの2つの時間単位
- OBC Timeには，cycleとstepの2つの時間単位がある．
- cycleは実質的なOBCの最小時間分解能であり，stepはOBCのCPUリソースをどう分割するかを管理する．
- したがって，Applicaionなどが識別できる時間はcycleである．
- stepはTaskListでの時間配分を規定し得ている．
    - 言い換えると，単一スレッドのC2Aにおける，Applicationの時分割を司っている．
- なお，Block Command Table上では，時刻データは無次元である．
    - これがcycleなのかstepなのかはコンテスト依存（Dispatcher依存）であるので，注意が必要


## コマンドの実行主体
- C2Aにおいて処理の最小単位はコマンドである．
- TaskListでも，Applicationが並んでるようにみえるが，実際はApplicationのexecutorを叩くコマンドが発行されているのが実体．
    - つまり，TDSPが実行してるのはコマンド．
- しかし，TLC，RTC，GSCなどのその他各種コマンドはすべて，ApplicationとしてのDispatcherが実行している．
    - 言い換えると，一般的なコマンドはTDSPから直接実行されていないが，例外的にTaskListだけApplicationではなくTDSPで実行されている．
