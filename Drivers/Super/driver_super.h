/**
 * @file
 * @brief  各制御センサ・アクチュエータ等とのインターフェースドライバ群のスーパークラス
 *
 *         DriverSuper Class は
 *         各制御センサ・アクチュエータ等とのインターフェースを実現し，
 *         初期化，コマンド発行，テレメトリリクエスト，テレメトリ受信，テレメトリ解析などを行う，ドライバ群のスーパークラスです．
 *         個々の機器のインターフェースドライバに継承させて使用します．
 * @note   DS_StreamRecBuffer で与える受信バッファサイズについて
 *         このバッファに受信データを格納していき，フレームを確定していく．
 *         したがって，最低限のバッファサイズは，
 *           - 非同期通信の場合: 受信フレーム最大長 x 2 以上
 *           - 同期通信の場合:   受信フレーム最大長 以上
 *         が望ましい．
 *         メモリに余力がある場合，さらに
 *           - rx_buffer_size_in_if_rx_ (or DS_IF_RX_BUFFER_SIZE) x 2 以上
 *         があると，さらに受信漏れを防ぐことができる．
 * @note   バッファのサイズ設定について
 *           - DS_StreamRecBuffer のサイズは rx_buffer_size_in_if_rx_ よりも大きい必要がある
 *           - rx_buffer_size_in_if_rx_ のサイズは，実際の OBC のハードウェアバッファサイズと等しくすると最も効率が良い
 *           - DS_IF_RX_BUFFER_SIZE はすべてのドライバの rx_buffer_size_in_if_rx_ 以上の値にする（最大値にすると良い）
 */
#ifndef DRIVER_SUPER_H_
#define DRIVER_SUPER_H_

#include <src_user/IfWrapper/if_list.h>
#include <stdint.h>
#include "../../Library/endian.h"        // パスが不定な自動生成コード類で使えるように
#include "../../System/TimeManager/time_manager.h"
#include "../../TlmCmd/common_cmd_packet.h"

#define DS_STREAM_MAX                 (3)         /*!< DS_StreamConfig の最大数
                                                       uint8_t を想定          */
#define DS_IF_RX_BUFFER_SIZE          (1024)      /*!< IF_RX で受信するときの一次バッファ
                                                       IF_RX から受信できる最大数を規定する
                                                       OBC の物理的な信号ラインのバッファサイズ以上とするともっともパフォーマンスが出る */

#include <src_user/Settings/DriverSuper/driver_super_params.h>

typedef struct DriverSuper DriverSuper;
typedef struct DS_StreamConfig DS_StreamConfig;

/**
 * @enum   DS_DRIVER_ERR_CODE
 * @brief  Driver の汎用エラーコード
 *
 *         Driver 内の各種 setter 等の関数返り値で使われることを想定
 * @note   uint8_t を想定
 * @note   初期化関数呼び出し時については， DS_INIT_ERR_CODE を用いること
 * @note   受信関数呼び出し時については， DS_REC_ERR_CODE を用いること
 * @note   接続先機器へ送るCmd呼び出し時については， DS_CMD_ERR_CODE を用いること
 * @note   DI の Cmd の返り値である CCP_EXEC_STS との整合性を多少意識している
 * @note   CCP_EXEC_STS への変換は DS_conv_driver_err_to_ccp_cmd_ret を用いる
 * @note   より詳細なエラー情報を返したい場合は， Driver ごとに独自 enum を定義して良い
 */
typedef enum
{
  DS_DRIVER_ERR_CODE_OK = 0,                //!< OK
  DS_DRIVER_ERR_CODE_ILLEGAL_CONTEXT,       //!< CCP_EXEC_ILLEGAL_CONTEXT に対応
  DS_DRIVER_ERR_CODE_ILLEGAL_PARAMETER,     //!< CCP_EXEC_ILLEGAL_PARAMETER に対応
  DS_DRIVER_ERR_CODE_ILLEGAL_LENGTH,        //!< CCP_EXEC_ILLEGAL_PARAMETER に対応
  DS_DRIVER_ERR_CODE_UNKNOWN_ERR = 255      //!< UNKNOWN ERR
} DS_DRIVER_ERR_CODE;

/**
 * @enum   DS_INIT_ERR_CODE
 * @brief  Driver の初期化関数のエラーコード
 * @note   uint8_t を想定
 */
typedef enum
{
  DS_INIT_OK = 0,               //!< OK
  DS_INIT_DS_INIT_ERR,          //!< DS_init でのエラー
  DS_INIT_PARAMETER_ERR,        //!< 初期化パラメタエラー
  DS_INIT_OTHER_ERR,            //!< その他のエラー
  DS_INIT_UNKNOWN_ERR = 255     //!< UNKNOWN ERR
} DS_INIT_ERR_CODE;

/**
 * @enum   DS_REC_ERR_CODE
 * @brief  Driver の受信関数のエラーコード
 * @note   uint8_t を想定
 */
typedef enum
{
  DS_REC_OK = 0,                //!< OK
  DS_REC_DS_RECEIVE_ERR,        //!< DS_receive でのエラー
  DS_REC_ANALYZE_ERR,           //!< DS_analyze_rec_data でのエラー
  DS_REC_OTHER_ERR,             //!< その他のエラー
  DS_REC_UNKNOWN_ERR = 255      //!< UNKNOWN ERR
} DS_REC_ERR_CODE;

/**
 * @enum   DS_CMD_ERR_CODE
 * @brief  各DIが Driver にコマンドを送るときに，統一的に使うコード
 * @note   uint8_t を想定
 * @note   DI の Cmd の返り値である CCP_EXEC_STS との整合性を多少意識している
 * @note   CCP_EXEC_STS への変換は DS_conv_cmd_err_to_ccp_cmd_ret を用いる
 */
typedef enum
{
  DS_CMD_OK                 = 0,   //!< OKは0であることを保証する
  DS_CMD_ILLEGAL_CONTEXT    = 1,   //!< CCP_EXEC_ILLEGAL_CONTEXT に対応．DIでみることも多いはず．HW依存部は Driver でみる
  DS_CMD_ILLEGAL_PARAMETER  = 2,   //!< CCP_EXEC_ILLEGAL_PARAMETER に対応．ヒーターの個数など，HW 依存部は Driver でみる
  DS_CMD_ILLEGAL_LENGTH     = 3,   //!< CCP_EXEC_ILLEGAL_LENGTH に対応．これは基本的にはDIで見るはずなので，使われないことを想定
  DS_CMD_DRIVER_SUPER_ERR   = 4,   //!< DriverSuper 側，つまり配送の低レイヤーでエラーが起きた場合
  DS_CMD_UNKNOWN_ERR        = 255
} DS_CMD_ERR_CODE;

/**
 * @enum   DS_ERR_CODE
 * @brief  DriverSuper の汎用エラーコード
 * @note   uint8_t を想定
 * @note   DriverSuper で用い， Drive では用いない
 */
typedef enum
{
  DS_ERR_CODE_OK   = 0,     //!< 返り値は継承先や IF 先で上書きされるため，OK は 0 であることが必須
  DS_ERR_CODE_ERR  = 1
} DS_ERR_CODE;

/**
 * @enum   DS_RX_DISRUPTION_STATUS_CODE
 * @brief  受信途絶判定コード
 * @note   uint8_t を想定
 */
typedef enum
{
  DS_RX_DISRUPTION_STATUS_OK   = 0,  //!< 受信途絶していない or 受信途絶判定をしない状態
  DS_RX_DISRUPTION_STATUS_LOST = 1   //!< 受信途絶検出
} DS_RX_DISRUPTION_STATUS_CODE;

/**
 * @struct DS_RecStatus
 * @brief  IF 受信状況
 */
typedef struct
{
  int                          ret_from_if_rx;       //!< IF_RX の返り値
  DS_RX_DISRUPTION_STATUS_CODE rx_disruption_status; //!< 受信途絶判定
} DS_RecStatus;

/**
 * @enum   DS_STREAM_SEND_STATUS_CODE
 * @brief  DS_StreamSendStatus でつかう．コマンド受信の状態を示す
 * @note   uint8_t を想定
 */
typedef enum
{
  DS_STREAM_SEND_STATUS_OK,
  DS_STREAM_SEND_STATUS_DISABLE,
  DS_STREAM_SEND_STATUS_TX_ERR,                //!< IF_TX でエラー
  DS_STREAM_SEND_STATUS_VALIDATE_ERR,          //!< 送信前に validate_config でエラー
  DS_STREAM_SEND_STATUS_OTHER_ERR
} DS_STREAM_SEND_STATUS_CODE;

/**
 * @struct DS_StreamSendStatus
 * @brief  フレーム送信状況
 */
typedef struct
{
  DS_STREAM_SEND_STATUS_CODE status_code;       //!< status
  int                        ret_from_if_tx;    //!< IF_TX の返り値
  // 今後詳細情報を拡張するなら，ここに入れる
} DS_StreamSendStatus;

/**
 * @enum   DS_STREAM_REC_STATUS_CODE
 * @brief  DS_StreamRecStatus でつかう．テレメ受信の状態遷移を示す
 * @note   uint8_tを想定
 */
typedef enum
{
  DS_STREAM_REC_STATUS_FINDING_HEADER,
  DS_STREAM_REC_STATUS_RECEIVING_HEADER,
  DS_STREAM_REC_STATUS_RECEIVING_FRAMELENGTH, //!< 可変長フレームでの，フレーム長データを受信中
  DS_STREAM_REC_STATUS_RECEIVING_DATA,
  DS_STREAM_REC_STATUS_RECEIVING_FOOTER,
  DS_STREAM_REC_STATUS_FIXED_FRAME,
  DS_STREAM_REC_STATUS_DISABLE,
  DS_STREAM_REC_STATUS_HEADER_MISMATCH,
  DS_STREAM_REC_STATUS_FOOTER_MISMATCH,
  DS_STREAM_REC_STATUS_RX_FRAME_TOO_LONG,
  DS_STREAM_REC_STATUS_RX_FRAME_TOO_SHORT,
  DS_STREAM_REC_STATUS_RX_ERR,                //!< IF_RX でエラー
  DS_STREAM_REC_STATUS_VALIDATE_ERR,          //!< 受信前に validate_config でエラー
  DS_STREAM_REC_STATUS_OTHER_ERR
} DS_STREAM_REC_STATUS_CODE;

/**
 * @enum   DS_STREAM_RX_DISRUPTION_STATUS_CODE
 * @brief  テレメ途絶判定コード
 * @note   uint8_t を想定
 */
typedef enum
{
  DS_STREAM_TLM_DISRUPTION_STATUS_OK   = 0,  //!< テレメ途絶していない or テレメ途絶判定をしない状態
  DS_STREAM_TLM_DISRUPTION_STATUS_LOST = 1   //!< テレメ途絶検出
} DS_STREAM_TLM_DISRUPTION_STATUS_CODE;

/**
 * @struct DS_StreamRecStatus
 * @brief  フレーム受信状況
 */
typedef struct
{
  DS_STREAM_REC_STATUS_CODE            status_code;                   //!< status
  uint16_t                             fixed_frame_len;               //!< フレーム確定したときのフレーム長さ
  DS_STREAM_TLM_DISRUPTION_STATUS_CODE tlm_disruption_status;         //!< テレメ途絶判定
  uint32_t                             count_of_carry_over_failures;  /*!< 受信バッファの繰越に失敗した回数
                                                                           DS_receive の呼び出し頻度がおそすぎることが原因 */
  // 今後詳細情報を拡張するなら，ここに入れる（ref. EQU Driver Super の DRIVE_Super_rec など）
} DS_StreamRecStatus;

/**
 * @struct DS_StreamRecBuffer
 * @brief  フレーム受信バッファ
 * @note   FIXME: DS_StreamRecStatus.status_code と is_frame_fixed の棲み分けを考える
 *         現状， is_frame_fixed は， DS_receive 後に降りるフラグになってる
 */
typedef struct
{
  uint8_t* buffer;                        //!< バッファ本体となる uint8_t 配列．外部から与える
  uint16_t capacity;                      //!< buffer のメモリ確保量
  uint16_t size;                          //!< 現在使われているサイズ
  uint16_t pos_of_frame_head_candidate;   //!< フレーム開始点の候補
  uint16_t confirmed_frame_len;           //!< 受信フレームとして受理された pos_of_frame_head_candidate からの長さ（フレーム確定後，これがフレーム長になる）
  uint8_t  is_frame_fixed;                //!< このバッファ上でフレームが確定してるか？
  uint16_t pos_of_last_rec;               //!< 最後に受信したデータを格納した位置
} DS_StreamRecBuffer;

/**
 * @struct DS_Config
 * @brief  DriverSuper の設定
 *
 *         各IFはこれを継承してつかう．
 *         DS_Config のメンバはすべての Driver から非公開とし， getter / setter でアクセスする
 */
typedef struct
{
  struct
  {
    uint16_t rx_buffer_size_in_if_rx_;                        /*!< IF_RX で受信するときの一次バッファのサイズ
                                                                   DS ではまず IF_RX を全 Driver 共通の一次バッファにコピーした後，
                                                                   DS_StreamRecBuffer に push して解析していく．
                                                                   IF_RX で読み込む量が多すぎると，DS_StreamRecBuffer に収まりきらないことがあるので，
                                                                   DS_StreamRecBuffer のサイズが小さい場合は，IF_RX で読み込むサイズも小さくする必要がある．
                                                                   最大値: DS_IF_RX_BUFFER_SIZE
                                                                   初期値: DS_IF_RX_BUFFER_SIZE */
    uint8_t  should_monitor_for_rx_disruption_;               /*!< 受信途絶判定をするか？
                                                                   初期値: 0 */
    uint32_t time_threshold_for_rx_disruption_;               /*!< 受信途絶判定の閾値 [ms]
                                                                   初期値: 60 * 1000 */
  } settings;       //!< 設定値

  struct
  {
    DS_RecStatus rec_status_;                                 //!< IF受信状況

    uint32_t rx_count_;                                       //!< なにかしらのデータの受信回数
    uint32_t rx_call_count_;                                  //!< DS_receive 呼び出し回数

    ObcTime  rx_time_;                                        //!< なにかしらのデータの受信時刻
  } info;           //!< 取得値（メトリクス）

  struct
  {
    DS_ERR_CODE (*load_init_setting)(DriverSuper* p_super);   /*!< DS_init でロードする，ドライバの初期設定の設定関数
                                                                   DS_reset_config での設定をオーバーロードする
                                                                   返り値は DS_ERR_CODE */
  } internal;       //!< 内部処理用
} DS_Config;

/**
 * @struct DS_StreamConfig
 * @brief  DriverSuperStream の設定
 *
 *         DS_StreamConfig のメンバはすべての Driver から非公開とし， getter / setter でアクセスする
 */
struct DS_StreamConfig
{
  struct
  {
    uint8_t  is_enabled_;                                     /*!< 有効か？
                                                                   初期値: 0 (無効) */

    uint8_t  is_strict_frame_search_;                         /*!< 厳格なフレーム探索が有効か？
                                                                   ノイズ発生時や複数 stream 使用時にフレーム受信漏れを完全になくすモード
                                                                   - OFFの場合（通常はこちら）
                                                                       ヘッダを見つけて，フレーム候補を見つけた後，次のフレームはフレーム候補から探索する
                                                                   - ONの場合
                                                                       ヘッダを見つけて，フレーム候補を見つけた後，次のフレームは，見つけたヘッダ先頭の次バイトから探索する
                                                                       実行時間は長くなる
                                                                   複数 stream やノイズが入ってしまった場合など，本来ヘッダでない部分をヘッダとして認識してしまう場合に有効化すると，
                                                                   論理的なフレーム受信漏れの確率を限りなく 0 に近づけることができる．
                                                                   ヘッダがあるフレームの場合のみ，有効にできる
                                                                   初期値: 0 (OFF) */

    uint8_t* tx_frame_;                                       /*!< コマンドフレーム
                                                                   FIXME: tx_frame_buffer_ に rename する？
                                                                   初期値: NULL */
    uint16_t tx_frame_size_;                                  /*!< コマンドフレームサイズ
                                                                   tx_frame_ のうち実際に送信するバイト数
                                                                   送信データがない場合は 0
                                                                   初期値: 0 */
    int16_t  tx_frame_buffer_size_;                           /*!< 与えた tx_frame_ の最大サイズ
                                                                   Drivers/Protocol などで， Util が tx_frame_ を使うときに使用
                                                                   Protocol を使うときは設定しておくと良い（一部の関数は設定しないと使えない）
                                                                   未指定の場合は負数とする
                                                                   初期値: -1 */

    DS_StreamRecBuffer* rx_buffer_;                           /*!< 受信バッファ
                                                                   stream 初期化時に user がメモリを割り当て，設定する
                                                                   初期値: NULL */
    const uint8_t* rx_header_;                                /*!< 受信データのヘッダ
                                                                   初期値: NULL */
    uint16_t rx_header_size_;                                 /*!< 受信データのヘッダサイズ
                                                                   ヘッダがない場合は0に設定
                                                                   この場合，基本的には固定長（rx_frame_size が正）を使う．
                                                                   ヘッダがなく，可変長の場合は，受信前（例えば DS_send_req_tlm_cmd 呼び出し前） に
                                                                   rx_frame_size_ を設定することで固定長のように扱うことで対応する．
                                                                   また，初期化時の Validation を通すためにも，初期値は適切な正数にしておくこと
                                                                   初期値: 0 */
    const uint8_t* rx_footer_;                                /*!< 受信データのフッタ
                                                                   初期値: NULL */
    uint16_t rx_footer_size_;                                 /*!< 受信データのフッタサイズ
                                                                   ヘッダがない場合は0に設定
                                                                   初期値: 0 */
    int16_t  rx_frame_size_;                                  /*!< 受信データ（テレメトリ）フレームサイズ
                                                                   受信データがない場合は 0 に設定
                                                                   受信データが可変の場合は負数に設定
                                                                   初期値: 0 */
    uint16_t max_rx_frame_size_;                              /*!< 受信データ（テレメトリ）の想定される最大フレームサイズ
                                                                   これよりも長いフレームが来た（来そうな）場合は，そのフレーム（候補）は破棄される
                                                                   これにより，ヘッダ内部のフレーム長が巨大な値に化けていた場合などに永遠に受信してしまうことを防ぐことができる
                                                                   rx_frame_size_ が固定長の場合は無視される
                                                                   初期値: 0xffff */
    int16_t  rx_framelength_pos_;                             /*!< 受信データ内のフレームサイズデータの存在する場所（先頭から数えて何 byte 目に位置するか．0 起算）
                                                                   受信データが可変長の場合のみ使用される．
                                                                   フレームサイズデータがない場合には負に設定する．
                                                                   可変長でかつフレームサイズデータのないフレームは，フッタが設定されている場合は有効である．
                                                                   ただし，フッタの探索が必要なため，実行時間は若干遅くなる．
                                                                   もちろん，ヘッダがあることを推奨する．ヘッダがない場合は，受信したデータの冒頭からフレームとみなす．
                                                                   受信されるフレーム長が受信前に判明している場合は，
                                                                   ヘッダがない場合のときと同様に，受信前（例えば DS_send_req_tlm_cmd 呼び出し前） に
                                                                   rx_frame_size_ を設定することで，固定長のように扱うことで対応することを推奨する．
                                                                   初期値: -1 */
    uint16_t rx_framelength_type_size_;                       /*!< フレームサイズデータの型サイズ [byte]
                                                                   受信データが可変長の場合のみ使用される．
                                                                   例えば uint8 なら 1， uint32 なら 4
                                                                   初期値: 0 */
    uint16_t rx_framelength_offset_;                          /*!< フレームサイズデータのオフセット値
                                                                   受信データが可変長の場合のみ使用される
                                                                   フレームサイズデータによる可変長データの解析は「フレームの全サイズ」により行われるが，
                                                                   機器の中にはヘッダとフッタの分は除いたデータ数としてサイズが表現される場合がある
                                                                   その場合のサイズ調整のために使う
                                                                   フレームサイズデータが「フレームの全サイズ」を示している場合には 0 に設定する
                                                                   初期値: 0 */
    ENDIAN_TYPE rx_framelength_endian_;                       /*!< フレームサイズデータのエンディアン
                                                                   受信データが可変長の場合のみ使用される
                                                                   初期値: ENDIAN_TYPE_BIG */

    uint8_t  should_monitor_for_tlm_disruption_;              /*!< テレメ途絶判定をするか？
                                                                   初期値: 0 */
    uint32_t time_threshold_for_tlm_disruption_;              /*!< テレメ途絶判定の閾値 [ms]
                                                                   初期値: 60 * 1000 */

    DS_ERR_CODE (*data_analyzer_)(DS_StreamConfig* p_stream_config, void* p_driver);
                                                              /*!< 受信データの解析関数
                                                                   p_driver は継承先機器のドライバ構造体など
                                                                   返り値は DS_ERR_CODE
                                                                   DS_data_analyzer_dummy_ */
  } settings;       //!< 設定値

  struct
  {
    DS_StreamSendStatus send_status_;                         //!< フレーム送信状況
    DS_StreamRecStatus  rec_status_;                          //!< フレーム受信状況

    uint32_t general_cmd_tx_count_;                           //!< 通常コマンド送信回数
    uint32_t req_tlm_cmd_tx_count_;                           //!< テレメ要求コマンド送信回数
    uint32_t req_tlm_cmd_tx_count_after_last_tx_;             /*!< 最後にテレメを受信してからのテレメ要求コマンド送信回数
                                                                   これが 0 でない場合，テレメが最新ではない可能性がある */
    uint32_t rx_frame_fix_count_;                             //!< フレーム受信確定回数

    ObcTime  general_cmd_tx_time_;                            //!< 通常コマンド最終送信時刻
    ObcTime  req_tlm_cmd_tx_time_;                            //!< テレメ要求コマンド最終送信時刻
    ObcTime  rx_frame_fix_time_;                              //!< フレーム確定時刻

    DS_ERR_CODE ret_from_data_analyzer_;                      //!< data_analyzer_ の返り値
  } info;           //!< 取得値（メトリクス）

  struct
  {
    uint8_t  is_validation_needed_for_send_;                  //!< 送信前に設定値の Validation が必要か？
    uint8_t  is_validation_needed_for_rec_;                   //!< 受信前に設定値の Validation が必要か？
  } internal;       //!< 内部処理用
};
// TODO: Protocol 用に data_link_layer_ を追加

/**
 * @struct DriverSuper
 * @brief  DriverSuper の設定
 *         各 IF はこれを継承してつかう．
 */
struct DriverSuper
{
  // 【継承先まで公開】
  IF_LIST_ENUM      interface;                              //!< 継承先の機器の使用 IF
  void*             if_config;                              //!< IF 設定

  DS_Config         config;                                 //!< DriverSuper の設定

  DS_StreamConfig   stream_config[DS_STREAM_MAX];           /*!< DriverSuperStream
                                                                 index が低いものほど優先（に今後するかも．実行速度次第）．
                                                                 使い方例：[0] のみをつかって，テレメ内に仕込んだ TLM ID などで data_analyzer_ 内で処理を分岐
                                                                 使い方例：[0] を定期テレメと一般コマンドで使い，[1] 以降を非定期や特殊コマンド・テレメトリで使う
                                                                 が，まあ自由に使ってもらえたら */
};


// ###### DriverSuper 基本関数 ######

/**
 * @brief  継承先の機器より DriverSuper を初期化する（stream 0 のみの使用の場合）
 *
 *         DriverSuper 構造体を継承先 Driver 構造体のメンバとして定義（継承）し，ポインタを渡すことでポートを初期化する．
 *         そして，構造体内の初期化が必要な変数を初期化する．
 *         デフォルト値の上書きは load_init_setting で行う
 * @note   DriverSuper を使用する時は起動時に必ず実施すること
 * @param  p_super:           初期化する DriverSuper 構造体へのポインタ
 * @param  if_config:         初期化する Driverで用いられている IF の config 構造体
 * @param  rx_buffer:         初期化する DriverSuper の stream 0 で用いられるフレーム受信バッファ
 * @param  load_init_setting: DriverSuper の初期設定ロード関数ポインタ
 * @return DS_ERR_CODE
 */
DS_ERR_CODE DS_init(DriverSuper* p_super,
                    void* if_config,
                    DS_StreamRecBuffer* rx_buffer,
                    DS_ERR_CODE (*load_init_setting)(DriverSuper* p_super));

/**
 * @brief  継承先の機器より DriverSuper を初期化する（複数の stream を使用する場合）
 *
 *         DriverSuper 構造体を継承先 Driver 構造体のメンバとして定義（継承）し，ポインタを渡すことでポートを初期化する．
 *         そして，構造体内の初期化が必要な変数を初期化する．
 *         デフォルト値の上書きは load_init_setting で行う
 * @note   DriverSuper を使用する時は起動時に必ず実施すること
 * @param  p_super:           初期化する DriverSuper 構造体へのポインタ
 * @param  if_config:         初期化する Driverで用いられている IF の config 構造体
 * @param  rx_buffers:        初期化する DriverSuper で用いられるフレーム受信バッファ．使用しない stream は NULL を設定しておく
 * @param  load_init_setting: DriverSuper の初期設定ロード関数ポインタ
 * @return DS_ERR_CODE
 */
DS_ERR_CODE DS_init_streams(DriverSuper* p_super,
                            void* if_config,
                            DS_StreamRecBuffer* rx_buffers[DS_STREAM_MAX],
                            DS_ERR_CODE (*load_init_setting)(DriverSuper* p_super));

/**
 * @brief  DriverSuper のリセット
 * @note   DS_init 内で呼ばれている．
 * @param  p_super: DriverSuper 構造体へのポインタ
 * @return DS_ERR_CODE
 */
DS_ERR_CODE DS_reset(DriverSuper* p_super);

/**
 * @brief  DriverSuper の設定に不整合が生じていないかチェックする
 *
 *         Driver の設定を変えた場合は毎回呼び出すことを推奨する
 * @note   DS_init 内で呼ばれている．
 * @note   内部の管理フラグを変更しているので， p_super に厳密な const 性はない
 * @param  p_super: DriverSuper 構造体へのポインタ
 * @return DS_ERR_CODE
 */
DS_ERR_CODE DS_validate_config(DriverSuper* p_super);

/**
 * @brief  受信バッファをクリアする
 *
 *         例えば，ヘッダなしテレメの場合，途中でゴミデータが入ると以後すべてのフレームがずれてしまう．
 *         そのようなとき（CRC エラーがでるとか，受信データが明らかにおかしい場合）に，buffer を一度クリアし，
 *         次に届くデータからフレーム解析を先頭から行うようにするために用いる．
 * @param  p_super: DriverSuper 構造体へのポインタ
 * @return DS_ERR_CODE
 */
DS_ERR_CODE DS_clear_rx_buffer(DriverSuper* p_super);

/**
 * @brief  継承先の機器からテレメトリを受信する
 *
 *         フレームを確定させて，rx_frame_ にいれるまで．解析 (data_analyzer_) はしないのでドライバで DS_analyze_rec_data を呼び出すこと
 *         これは，同じ stream でもテレメ内部の ID などで解析を変えたいときなどが想定されるため
 * @note   継承先の機器のデータ出力周期より早い周期で定期的に実行すること
 * @param  p_super: DriverSuper 構造体へのポインタ
 * @retval DS_ERR_CODE_OK:  IF_RX でのエラーなし
 * @retval DS_ERR_CODE_ERR: IF_RX でのエラーあり
 * @note   受信状況やエラー情報は rec_status_ に格納されている
 */
DS_ERR_CODE DS_receive(DriverSuper* p_super);

/**
 * @brief  data_analyzer_ を呼び出し，受信データを解析する．
 *
 *         DS_receive にてデータを受信した後， DSSC_get_rec_status(p_stream_config)->status_code が DS_STREAM_REC_STATUS_FIXED_FRAME ならば呼び出す．
 * @param  p_super:  DriverSuper 構造体へのポインタ
 * @param  stream:   どの stream_config を使用するか．stream は 0-MAX なので，継承先で ENUM など宣言して使いやすくすればいいと思う．
 * @param  p_driver: 継承先機器のドライバ構造体など．data_analyzer_ の第二引数．
 * @return DS_ERR_CODE: data_analyzer_ の返り値をそのまま返す
 * @note   data_analyzer_ の返り値は， ret_from_data_analyzer_ にも保存される．
 */
DS_ERR_CODE DS_analyze_rec_data(DriverSuper* p_super, uint8_t stream, void* p_driver);

/**
 * @brief  継承先の機器に一般コマンドを発行する
 *
 *         このコマンドを送ったことによってレスポンスが返ってくることを想定していない（その場合は DS_send_req_tlm_cmd を使う）
 * @note   この関数の実行前に，tx_frame, tx_frame_size の設定が必要である
 * @note   これは基底クラスなため，アノマリ発行は行わない．継承先で返り値を見て適切にアノマリ発行すること
 * @param  p_super: DriverSuper 構造体へのポインタ
 * @param  stream:  どのstream_config を使用するか．stream は 0-MAX なので，継承先で ENUM など宣言して使いやすくすればいいと思う．
 * @retval DS_ERR_CODE_OK:  正常終了
 * @retval DS_ERR_CODE_ERR: IF_TX でのエラーあり
 * @note   受信状況やエラー情報は send_status_ に格納されている
 */
DS_ERR_CODE DS_send_general_cmd(DriverSuper* p_super, uint8_t stream);

/**
 * @brief  継承先の機器にテレメ要求コマンドを発行する
 *
 *         テレメについては DS_receive で受け取る．
 * @note   この関数の実行前に，tx_frame, tx_frame_sizeの設定が必要である
 * @param  p_super: DriverSuper 構造体へのポインタ
 * @param  stream:  どのstream_config を使用するか．stream は 0-MAX なので，継承先で ENUM など宣言して使いやすくすればいいと思う．
 * @retval DS_ERR_CODE_OK:  正常終了
 * @retval DS_ERR_CODE_ERR: IF_TX でのエラーあり
 * @note   受信状況やエラー情報は send_status_ に格納されている
 */
DS_ERR_CODE DS_send_req_tlm_cmd(DriverSuper* p_super, uint8_t stream);


// ###### DS_Config Getter/Setter of Settings ######
uint16_t DSC_get_rx_buffer_size_in_if_rx(const DriverSuper* p_super);
DS_ERR_CODE DSC_set_rx_buffer_size_in_if_rx(DriverSuper* p_super,
                                            const uint16_t rx_buffer_size_in_if_rx);
uint8_t DSC_get_should_monitor_for_rx_disruption(const DriverSuper* p_super);
void DSC_enable_monitor_for_rx_disruption(DriverSuper* p_super);
void DSC_disable_monitor_for_rx_disruption(DriverSuper* p_super);
uint32_t DSC_get_time_threshold_for_rx_disruption(const DriverSuper* p_super);
void DSC_set_time_threshold_for_rx_disruption(DriverSuper* p_super,
                                              const uint32_t time_threshold_for_rx_disruption);


// ###### DS_Config Getter of Info ######
const DS_RecStatus* DSC_get_rec_status(const DriverSuper* p_super);
uint32_t DSC_get_rx_count(const DriverSuper* p_super);
uint32_t DSC_get_rx_call_count(const DriverSuper* p_super);
const ObcTime* DSC_get_rx_time(const DriverSuper* p_super);

DS_RX_DISRUPTION_STATUS_CODE DSC_get_rx_disruption_status(const DriverSuper* p_super);


// ###### DS_StreamConfig Getter/Setter of Settings ######
uint8_t DSSC_get_is_enabled(const DS_StreamConfig* p_stream_config);
void DSSC_enable(DS_StreamConfig* p_stream_config);
void DSSC_disable(DS_StreamConfig* p_stream_config);

uint8_t DSSC_get_is_strict_frame_search(const DS_StreamConfig* p_stream_config);
void DSSC_enable_strict_frame_search(DS_StreamConfig* p_stream_config);
void DSSC_disable_strict_frame_search(DS_StreamConfig* p_stream_config);

const uint8_t* DSSC_get_tx_frame(DS_StreamConfig* p_stream_config);
uint8_t* DSSC_get_tx_frame_as_non_const_pointer(DS_StreamConfig* p_stream_config);
void DSSC_set_tx_frame(DS_StreamConfig* p_stream_config,
                       uint8_t* tx_frame);
uint16_t DSSC_get_tx_frame_size(const DS_StreamConfig* p_stream_config);
void DSSC_set_tx_frame_size(DS_StreamConfig* p_stream_config,
                            const uint16_t tx_frame_size);
int16_t DSSC_get_tx_frame_buffer_size(DS_StreamConfig* p_stream_config);
void DSSC_set_tx_frame_buffer_size(DS_StreamConfig* p_stream_config,
                                   const int16_t tx_frame_buffer_size);

void DSSC_set_rx_buffer(DS_StreamConfig* p_stream_config,
                        DS_StreamRecBuffer* rx_buffer);
void DSSC_set_rx_header(DS_StreamConfig* p_stream_config,
                        const uint8_t* rx_header,
                        const uint16_t rx_header_size);
uint16_t DSSC_get_rx_header_size(const DS_StreamConfig* p_stream_config);
void DSSC_set_rx_footer(DS_StreamConfig* p_stream_config,
                        const uint8_t* rx_footer,
                        const uint16_t rx_footer_size);
uint16_t DSSC_get_rx_footer_size(const DS_StreamConfig* p_stream_config);
int16_t DSSC_get_rx_frame_size(const DS_StreamConfig* p_stream_config);
void DSSC_set_rx_frame_size(DS_StreamConfig* p_stream_config,
                            const int16_t rx_frame_size);
uint16_t DSSC_get_max_rx_frame_size(const DS_StreamConfig* p_stream_config);
void DSSC_set_max_rx_frame_size(DS_StreamConfig* p_stream_config,
                            const uint16_t max_rx_frame_size);

void DSSC_set_rx_framelength_pos(DS_StreamConfig* p_stream_config,
                                 const int16_t rx_framelength_pos);
void DSSC_set_rx_framelength_type_size(DS_StreamConfig* p_stream_config,
                                       const uint16_t rx_framelength_type_size);
void DSSC_set_rx_framelength_offset(DS_StreamConfig* p_stream_config,
                                    const uint16_t rx_framelength_offset);
void DSSC_set_rx_framelength_endian(DS_StreamConfig* p_stream_config,
                                    const ENDIAN_TYPE rx_framelength_endian);

uint8_t DSSC_get_should_monitor_for_tlm_disruption(const DS_StreamConfig* p_stream_config);
void DSSC_enable_monitor_for_tlm_disruption(DS_StreamConfig* p_stream_config);
void DSSC_disable_monitor_for_tlm_disruption(DS_StreamConfig* p_stream_config);
uint32_t DSSC_get_time_threshold_for_tlm_disruption(const DS_StreamConfig* p_stream_config);
void DSSC_set_time_threshold_for_tlm_disruption(DS_StreamConfig* p_stream_config,
                                                const uint32_t time_threshold_for_tlm_disruption);

void DSSC_set_data_analyzer(DS_StreamConfig* p_stream_config,
                            DS_ERR_CODE (*data_analyzer)(DS_StreamConfig* p_stream_config, void* p_driver));


// ###### DS_StreamConfig Getter of Info ######
const DS_StreamSendStatus* DSSC_get_send_status(const DS_StreamConfig* p_stream_config);
const DS_StreamRecStatus* DSSC_get_rec_status(const DS_StreamConfig* p_stream_config);

uint32_t DSSC_get_general_cmd_tx_count(const DS_StreamConfig* p_stream_config);
uint32_t DSSC_get_req_tlm_cmd_tx_count(const DS_StreamConfig* p_stream_config);
uint32_t DSSC_get_req_tlm_cmd_tx_count_after_last_tx(const DS_StreamConfig* p_stream_config);
uint32_t DSSC_get_rx_frame_fix_count(const DS_StreamConfig* p_stream_config);

const ObcTime* DSSC_get_general_cmd_tx_time(const DS_StreamConfig* p_stream_config);
const ObcTime* DSSC_get_req_tlm_cmd_tx_time(const DS_StreamConfig* p_stream_config);
const ObcTime* DSSC_get_rx_frame_fix_time(const DS_StreamConfig* p_stream_config);

DS_STREAM_TLM_DISRUPTION_STATUS_CODE DSSC_get_tlm_disruption_status(const DS_StreamConfig* p_stream_config);

DS_ERR_CODE DSSC_get_ret_from_data_analyzer(const DS_StreamConfig* p_stream_config);


// ###### Driver 汎用 Util 関数 ######

/**
 * @brief DS_StreamRecBuffer に確保したメモリを与えて初期化する
 * @param[out] stream_rec_buffer: 初期化する DS_StreamRecBuffer
 * @param[in]  buffer:            与えるメモリ領域
 * @param[in]  buffer_capacity:   与えるメモリサイズ
 * @return DS_ERR_CODE
 */
DS_ERR_CODE DS_init_stream_rec_buffer(DS_StreamRecBuffer* stream_rec_buffer,
                                      uint8_t* buffer,
                                      const uint16_t buffer_capacity);

/**
 * @brief DS_StreamRecBuffer の要素数 DS_STREAM_MAX の配列を NULL で初期化する
 * @note  DS_init_streams の引数を作るのに使う
 * @param[out] rx_buffers: 初期化する DS_StreamRecBuffer の配列
 * @return void
 */
void DS_nullify_stream_rec_buffers(DS_StreamRecBuffer* rx_buffers[DS_STREAM_MAX]);

/**
 * @brief  DS_DRIVER_ERR_CODE から CCP_CmdRet への変換関数
 *
 *         DI から Driver の関数を呼び出したときのエラーコードの変換に用いる
 * @note   汎用 Util 関数
 * @param  DS_DRIVER_ERR_CODE
 * @return CCP_CmdRet
 */
CCP_CmdRet DS_conv_driver_err_to_ccp_cmd_ret(DS_DRIVER_ERR_CODE code);

/**
 * @brief  DS_CMD_ERR_CODE から CCP_CmdRet への変換関数
 *
 *         DI から Driver の関数を呼び出したときのエラーコードの変換に用いる
 * @note   汎用 Util 関数
 * @param  DS_CMD_ERR_CODE
 * @return CCP_CmdRet
 */
CCP_CmdRet DS_conv_cmd_err_to_ccp_cmd_ret(DS_CMD_ERR_CODE code);


// ###### Driver Stream Config 汎用 Util 関数 ######

/**
 * @brief  確定したフレームを取得
 * @param  p_stream_config[in]: DriverSuper 構造体の DS_StreamConfig
 * @retval フレーム確定時:   受信フレーム先頭ポインタ
 * @retval フレーム未確定時: rx_buffer_.pos_of_frame_head_candidate
 * @note   フレームサイズは DSSC_get_fixed_rx_frame_size で取得可能
 * @note   rx_buffer_ (DS_StreamRecBuffer) には，前回確定したフレームが入っているが，次回の DS_receive 呼び出しで失われる．
 *         したがって，次回の DS_receive 呼び出し時までに内容を吸い出しておくこと
 * @note   基本的には， rx_buffer_.pos_of_frame_head_candidate は rx_buffer_.buffer 先頭に頭出しされているはず．
 *         したがって，ここで取得したポインタを rx_buffer_.capacity 以上読み込まない場合は，問題ない．
 */
const uint8_t* DSSC_get_rx_frame(const DS_StreamConfig* p_stream_config);

/**
 * @brief  確定したフレームのサイズを取得
 * @param  p_stream_config[in]: DriverSuper 構造体の DS_StreamConfig
 * @retval フレーム確定時:   確定したフレームサイズ
 * @retval フレーム未確定時: 0
 */
uint16_t DSSC_get_fixed_rx_frame_size(const DS_StreamConfig* p_stream_config);

#endif
