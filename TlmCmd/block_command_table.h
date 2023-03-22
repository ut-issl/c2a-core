/**
 * @file
 * @brief BCTの定義, BCT周りの関数
 */
#ifndef BLOCK_COMMAND_TABLE_H_
#define BLOCK_COMMAND_TABLE_H_

#include <stdint.h>

#define BCT_CMD_MAX_LENGTH (64)  /*!< BCTに保存するパケットのパケットサイズの上限
                                    TCPの最大長はかなり大きいので，ここではそれより小さくしてメモリを節約するのが通例
                                    この値より大きなコマンドはBCTに保存できなくなることに注意すること*/
#define BCT_MAX_CMD_NUM    (32)  //!< １つのBCに最大何個のコマンドを挿入できるか

#define BCT_MAX_BLOCKS    (383) //!< BCのIDの最大数
// ここ変えたら，Cmd_RESET_(RE)STORE_BCT* も変える！
// 2019/07/19 コメント追加
// MM_NOT_DEFINEDにBCT_MAX_BLOCKS が使われているので，-1されてる．
// けど，まあ-1する必要もあまりなさそう？


// 今後，BCTのサイズ変更がしやすいように定義する
// コマンドの引数長チェックもこれを用いる
#define SIZE_OF_BCT_ID_T    (2)    //!< bct_id_t の型サイズ．メモリインパクトがそれなりにあるので，可変に

#include <src_user/Settings/TlmCmd/block_command_table_params.h>

#if SIZE_OF_BCT_ID_T == 1
typedef uint8_t bct_id_t;
#elif SIZE_OF_BCT_ID_T == 2
typedef uint16_t bct_id_t;
#elif SIZE_OF_BCT_ID_T == 4
typedef uint32_t bct_id_t;
#else
#error Illegal value for SIZE_OF_BCT_ID_T
#endif

#include "common_cmd_packet.h" // bct_id_t の定義よりあとにinclude

/*
Block Command Table は
BCT_MAX_BLOCKS x BCT_MAX_CMD_NUM
のテーブルであり，
BCT_Pos.block がブロック番号，
BCT_Pos.cmd   がそのブロック内での位置
を保持している．
コマンドの実データを保存している本体は

[構造体定義]                        [サイズ Byte]
BlockCommandTable                        7668               at 383
  BCT_Pos                               8 : 4 * 2
    block                                   2
    cmd                                     1
  BCT_Func[BCT_MAX_BLOCKS]           6128 : 4 * 4 * 383 at 383
  BCT_Table*[BCT_MAX_BLOCKS]         1532 : 4 * 383     at 383

BCT_Table                          784767               at 383
  length                                    1
  BCT_CmdData[BCT_MAX_CMD_NUM]               2048 : 64 * 32
    data[BCT_CMD_MAX_LENGTH]                        64

Sum                                792435 : 784767 + 7668
なお，構造体のサイズは，そのメンバのサイズの和になるとは限らないことに注意．
この値は目安である．(データの効率化のために，パディングされることがあるため.)
*/

/**
 * @struct BCT_Pos
 * @brief  BCT内での作業中の位置情報
 */
typedef struct
{
  bct_id_t block; //!< ブロックの位置情報
  uint8_t  cmd;   //!< そのブロック内でどのコマンドを指すか
} BCT_Pos;

/**
 * @struct BCT_CmdData
 * @brief  Cmd の保存場所
 * @note   TCP_MAX_LENGTH (444) と比べてかなり短い BCT_CMD_MAX_LENGTH (64)
 *         CTCPの定義がユーザー依存なので現状 uint8_t の配列になっている (FIXME: 直す)
 */
typedef struct
{
  uint8_t data[BCT_CMD_MAX_LENGTH];
} BCT_CmdData;

/**
 * @struct BCT_Table
 * @brief  BCT の本体. 実際に BC の cmd が確保されている
 */
typedef struct
{
  uint8_t     length;                   //!< cmds に保存されているコマンド数
  BCT_CmdData cmds[BCT_MAX_CMD_NUM];     //!< 各 cmd が保存される配列
} BCT_Table;

/**
 * @struct BCT_Func
 * @brief  各setter, getter が入る
 * @note   公開されているが private 扱い. getter はラップされているのでそちらを使うこと
 *         存在する理由は中身が MRAM など三重冗長化されている場所にあることもあるため
 */
typedef struct
{
  BCT_CmdData* (*get_bc_cmd_data_)(const BCT_Pos* pos);                           //!< cmd_data の getter
  uint8_t      (*get_bc_length_)(const bct_id_t block);                           //!< length の getter
  void         (*set_bc_cmd_data_)(const BCT_Pos* pos, const BCT_CmdData* value); //!< cmd_data の setter
  void         (*set_bc_length_)(const bct_id_t block, uint8_t length);           //!< length の setter
} BCT_Func;

/**
 * @struct BlockCommandTable
 * @brief  BCT 全体の構造体
 */
typedef struct
{
  BCT_Pos pos;                            //!< 登録などの作業中の位置情報
  BCT_Pos pos_stored;                     /*!< OBC内部の処理でポインタが変わらないようにするための一時退避場所．
                                               詳細はC2Aドキュメント 「ブロックコマンド追加方法.md」  */
  BCT_Table* blocks[BCT_MAX_BLOCKS];      //!< BCTの各BC．好きな場所で確保できる様にポインタにしている
  BCT_Func   func[BCT_MAX_BLOCKS];        //!< BCT関係の関数
} BlockCommandTable;

/**
 * @enum  BCT_ACK
 * @brief BCT 関係の関数のエラーステータス
 * @note  uint8_t を想定
 */
typedef enum
{
  BCT_SUCCESS,
  BCT_INVALID_BLOCK_NO,
  BCT_INVALID_CMD_NO,
  BCT_ISORATED_CMD,
  BCT_DEFECTIVE_BLOCK,
  BCT_CMD_TOO_LONG,
  BCT_BC_FULL,
  BCT_ZERO_PERIOD,
  BCT_UNKNOWN
} BCT_ACK;

extern const BlockCommandTable* const block_command_table;

/**
 * @brief BCT の初期化関数
 * @note  BCT と書いているが内部では BCE_init も呼ばれるため BC が関係する有象無象が初期化される.
 */
void BCT_initialize(void);

/**
 * @brief BCT_CmdData の const pointer getter
 * @note アサーションはされない
 * @param[in] pos: const BCT_Pos*
 * @return const BCT_Pos*
 */
const BCT_CmdData* BCT_get_bc_cmd_data(const BCT_Pos* pos);

/**
 * @brief BCT の length の getter
 * @note アサーションはされない．有効でない block の場合， 0 を返す
 * @param[in] block: BC の idx
 * @return length
 */
uint8_t BCT_get_bc_length(const bct_id_t block);

/**
 * @brief 指定された block の BCT を初期化する
 * @param[in] block: BC の idx
 * @return BCT_ACK
 */
BCT_ACK BCT_clear_block(const bct_id_t block);

/**
 * @brief pos を pos_stored に保存
 * @note 地上局とOBC内部のBCのポインタが混ざっておかしくならないようにするための改修. 詳細はC2Aドキュメント 「ブロックコマンド追加方法.md」
 * @param  void
 * @return void
 */
void BCT_store_pos(void);

/**
 * @brief pos を pos_stored から復旧
 * @param  void
 * @return void
 */
void BCT_restore_pos(void);

/**
 * @brief BCT_Pos* の validate
 * @param  pos: BCTの位置
 * @return BCT_ACK
 */
BCT_ACK BCT_check_position(const BCT_Pos* pos);

/**
 * @brief BCT_Pos* を引数から作る
 * @param[out] pos: そこに作る pointer
 * @param[in] block: BC の idx
 * @param[in] cmd: Cmds_data 内での idx
 * @return BCT_ACK
 */
BCT_ACK BCT_make_pos(BCT_Pos* pos, const bct_id_t block, const uint8_t cmd);

/**
 * @brief 保存データを CCP にコピーする.
 * @param[in]  pos: コピー元のpacketの位置
 * @param[out] packet: コピーされる packet
 * @return BCT_ACK
 * @note  コマンド長は気にせず最大長を読み出すこととする.
 *        実際の長さ情報はコピーされているヘッダに格納されている.
 */
BCT_ACK BCT_load_cmd(const BCT_Pos* pos, CommonCmdPacket* packet);

/**
 * @brief Cmd を block_command_table->pos に登録する
 * @param[in] packet: 登録する Cmd の内容
 * @return BCT_ACK
 */
BCT_ACK BCT_register_cmd(const CommonCmdPacket* packet);

/**
 * @brief 既に登録されている BC の Cmd の中身を外部から上書きする
 * @param[in] pos: 上書きする位置
 * @param[in] packet: 上書きされる packet
 * @return BCT_ACK
 */
BCT_ACK BCT_overwrite_cmd(const BCT_Pos* pos, const CommonCmdPacket* packet);

/**
 * @brief BCT の中身を 他の BCT に COPY する
 * @param[in] dst_block コピー先の bct_id
 * @param[in] src_block コピー元の bct_id
 * @return BCT_ACK
 */
BCT_ACK BCT_copy_bct(const bct_id_t dst_block, const bct_id_t src_block);

// 以下3つは BCT に登録されている Cmd からその内容を取り出すgetter
/**
 * @brief BCT 内の Cmd の CMD_CODE の getter
 * @param[in] block: BC の idx
 * @param[in] cmd: Cmds_data 内での idx
 * @return CMD_CODE
 * @note (block, cmd) が異常値だった場合は Cmd_CODE_MAX を返す
 */
CMD_CODE BCT_get_id(const bct_id_t block, const uint8_t cmd);

/**
 * @brief BCT 内の Cmd の TI の getter
 * @param[in] block: BC の idx
 * @param[in] cmd: Cmds_data 内での idx
 * @return TI
 * @note (block, cmd) が異常値だった場合は 0 を返す
 */
cycle_t BCT_get_ti(const bct_id_t block, const uint8_t cmd);

/**
 * @brief BCT 内の Cmd の param_head の getter
 * @param[in] block: BC の idx
 * @param[in] cmd: Cmds_data 内での idx
 * @return const uint8_t* ポインタ
 * @note (block, cmd) が異常値だった場合は (0, 0) のものを返す
 */
const uint8_t* BCT_get_param_head(const bct_id_t block, const uint8_t cmd);

/**
 * @brief BCT の block の指すポインタ & func を入れ替える
 * @param[in] block_a: 入れ替える BC
 * @param[in] block_b: 入れ替える BC
 * @return BCT_ACK 実行結果
 * @note ID1:HK:SRAM, ID2:EH:MRAM があった時に ID1:EH:MRAM, ID2:HK:SRAM が出来る
 *       `BCT_swap_contents` と組み合わせることで番号 - 中身はそのままで RAM だけを切り替えることが出来る
 *       必ず `BCE_swap_address` と共に使用すること
 */
BCT_ACK BCT_swap_address(const bct_id_t block_a, const bct_id_t block_b);

/**
 * @brief BCT の block の指す中身を入れ替える
 * @param[in] block_a: 入れ替える BC
 * @param[in] block_b: 入れ替える BC
 * @return BCT_ACK 実行結果
 * @note ID1:EH:MRAM, ID2:HK:SRAM があった時に ID1:HK:MRAM, ID2:EH:SRAM が出来る
 *       `BCT_swap_ram` と組み合わせることで番号 - 中身はそのままで RAM だけを切り替えることが出来る
 *       必ず `BCE_swap_contents` と共に使用すること
 */
BCT_ACK BCT_swap_contents(const bct_id_t block_a, const bct_id_t block_b);

/**
 * @brief  返り値用 enum 変換用関数
 * @param  ack: BCT_ACK
 * @return CCP_CmdRet
 */
CCP_CmdRet BCT_convert_bct_ack_to_ccp_cmd_ret(BCT_ACK ack);

CCP_CmdRet Cmd_BCT_CLEAR_BLOCK(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BCT_SET_BLOCK_POSITION(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BCT_COPY_BCT(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BCT_OVERWRITE_CMD(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BCT_FILL_NOP(const CommonCmdPacket* packet);

#endif
