# Overview of Driver

## Driver
### 概要
Driver とは，各コンポーネントとの通信において， HW 依存部分を吸収するためのレイヤーである．  
[DriverSuper](https://github.com/ut-issl/c2a-core/blob/develop/Drivers/Super/) と呼ばれる共通の IF を各ドライバが継承することで，統一的で安全な実装が可能になる．

また，これを Application へと抽象化したものが， Application Layer にある DI (Driver Instance) である．

実装方法は，基本的には [driver_super.h](https://github.com/ut-issl/c2a-core/blob/develop/Drivers/Super/driver_super.h) をみればわかるようにしてあるが，現時点で

- UART test: https://github.com/ut-issl/c2a-core/blob/develop/Examples/minimum_user_for_s2e/src/src_user/Drivers/Etc/UART_TEST.c
- GS: https://github.com/ut-issl/c2a-core/blob/develop/Examples/minimum_user_for_s2e/src/src_user/Drivers/Com/gs.c
- C2A 間通信 (TBA)

が実装されているので，それを参考のこと．  
`load_init_setting` については下を参照．

### 推奨する Drivre 構造体の定義
TOBC (TBA) を例にあげる．

```
/**
 * @struct  TOBC_Driver
 * @brief
 */
typedef struct
{
  struct
  {
    DriverSuper super;                  //!< DriverSuper class
    RS422Config rs422_config;           //!< RS422 class
  } driver;
  TOBC_Info info;
} TOBC_Driver;
```
のように， `super` という名前で DriverSuper を継承（ C なので正確には継承ではないが，まあ has-a 関係で）する．  
そして， `info` という名前でそれぞれのドライバの持つ構造体メンバを持つ（ Application Info 構造体と並ぶ概念なので， `info` という命名を推奨）．  
そして，使用するIF構造体を持つ．


## DriverSuper
### テレメとしておろしてもいいのではないかなと思う DriverSuper のメンバ
[driver_super.h](https://github.com/ut-issl/c2a-core/blob/develop/Drivers/Super/driver_super.h) のメンバから抜粋する．

`DriverSuper` の最上位には特に重要な情報はないので， `DriverSuper` の使っているstreamの `stream_config` のメンバを下ろす．  
具体的には以下．（ FIXME: 2021/12/04 現在，だいぶ情報が古い）

```
struct DriverSuperStreamConfig
{
  uint8_t  is_enable;                                       //!< 有効か？  → ONOFFすることがあるのであれば

  DriverSuperSendStatus send_status;                        //!< フレーム送信状況  → 詳細な情報が見れるので必須に近いか？
  DriverSuperRecStatus  rec_status;                         //!< フレーム受信状況  → 詳細な情報が見れるので必須に近いか？

  uint32_t general_cmd_tx_count;                            //!< 通常コマンド送信回数  → 重要度低
  uint32_t req_tlm_cmd_tx_count;                            //!< テレメ要求コマンド送信回数  → 重要度低
  uint32_t req_tlm_cmd_tx_count_after_last_tx;              /*!< 最後にテレメを受信してからのテレメ要求コマンド送信回数  → 重要度低
                                                                 これが0でない場合，テレメが最新ではない可能性がある      */  → 重要度低
  uint32_t rx_frame_fix_count;                              //!< フレーム受信確定回数  → 重要度低

  OBCTime  general_cmd_tx_time;                             //!< 通常コマンド最終送信時刻  → 重要度低？ コンポによりそう
  OBCTime  req_tlm_cmd_tx_time;                             //!< テレメ要求コマンド最終送信時刻  → 重要度低？ コンポによりそう
  OBCTime  rx_time;                                         //!< なにかしらのデータの受信時刻  → 重要度低？ コンポによりそう
  OBCTime  rx_frame_fix_time;                               //!< フレーム確定時刻  → 最後に受信した時間がわかるので必須？

  uint8_t  is_monitor_for_tlm_disruption;                   /*!< テレメ途絶判定をするか？/  → 現時点では未使用だが，将来的には？
                                                                 !!!!! [TODO] この機能は将来実装予定で現在未実装です !!!!! */
  uint32_t time_threshold_for_tlm_disruption;               //!< テレメ途絶判定の閾値 [ms]  → 現時点では未使用だが，将来的には？

  uint16_t rx_frame_rec_len_;                               //!< 受信データフレームの受信済みByte．rx_frame_に対する操作ポインタになる  → 基本的には不要だが，高度なデバッグをしたいなら

  uint8_t  is_rx_buffer_carry_over_;                        //!< 繰越する受信データがあるか？  → 基本的には不要だが，高度なデバッグをしたいなら
  uint16_t carry_over_buffer_size_;                         //!< 繰越する受信データのサイズ  → 基本的には不要だが，高度なデバッグをしたいなら
};
typedef struct DriverSuperStreamConfig DriverSuperStreamConfig;
```


### DriverSuper と C2A 間通信
TBW

### その他
2021年の改良としては，大きく以下がある．
- コードの見通しの良さの大幅改良
- 高速化
- 複数のフレーム定義（＝ヘッダフッタやフレーム長）に対応するためのStreamの新設
  - これまでは定期・非定期で２つあったが，これがコードの見通しの良さとバグの温床になっていた
- varidationの強化

各ドライバの実装方法は，上で述べたように既存コードを参照してほしいが，初期化時にユーザー設定をデフォルト設定で上書きするための `load_init_setting` 関数ポインタを渡す必要がある．  
これは，旧 DriverSuper では，各ドライバの初期化関数で DriverSuper を初期化していたが，それでは DriverSuper のデフォルト値は設定できず（各 Driver 固有値を上書いてしまう）ため， DriverSuper の拡張性に乏しいだけでなく，ユーザーが使わない機能の変数まで明示的に初期化する必要があったことの改良によるものである．  
現 DriverSuper では，デフォルト値がセットされるため，使う機能の変数のみ `load_init_setting` で明示的に初期化すれば良い．
