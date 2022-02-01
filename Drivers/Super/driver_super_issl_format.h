/**
 * @file
 * @brief  Driver のための ISSL フォーマット通信のための公開定義，関数など
 */
#ifndef DRIVER_SUPER_ISSL_FORMAT_H_
#define DRIVER_SUPER_ISSL_FORMAT_H_

#include "driver_super.h"
#include <src_user/Library/stdint.h>

#define DS_ISSLFMT_STX_SIZE             (2)          //!< ISSL 標準フォーマットの STX のサイズ
#define DS_ISSLFMT_ETX_SIZE             (2)          //!< ISSL 標準フォーマットの ETX のサイズ
#define DS_ISSLFMT_LEN_SIZE             (2)          //!< ISSL 標準フォーマットの LEN のサイズ
#define DS_ISSLFMT_CRC_SIZE             (2)          //!< ISSL 標準フォーマットの CRC のサイズ
#define DS_ISSLFMT_COMMON_HEADER_SIZE   (DS_ISSLFMT_STX_SIZE + DS_ISSLFMT_LEN_SIZE)  //!< ISSL 標準フォーマットのコモンヘッダーのサイズ
#define DS_ISSLFMT_COMMON_FOOTER_SIZE   (DS_ISSLFMT_ETX_SIZE + DS_ISSLFMT_CRC_SIZE)  //!< ISSL 標準フォーマットのコモンフッターのサイズ
#define DS_ISSLFMT_CMD_HEADER_SIZE      (4)          //!< ISSL 標準フォーマットの CMD ヘッダーのサイズ
#define DS_ISSLFMT_TLM_HEADER_SIZE      (4)          //!< ISSL 標準フォーマットの TLM ヘッダーのサイズ
#define DS_ISSLFMT_STX_1ST_BYTE         (0xEB)       /*!< ISSL 標準フォーマットの STX 1st byte の取得
                                                          これは C2A 形式/非 C2A 形式共通の TLM HEADER 仕様である */
#define DS_ISSLFMT_STX_2ND_BYTE         (0x90)       /*!< ISSL 標準フォーマットの STX 2nd byte の取得
                                                          これは C2A 形式/非 C2A 形式共通の TLM HEADER 仕様である */
#define DS_ISSLFMT_ETX_1ST_BYTE         (0xC5)       /*!< ISSL 標準フォーマットの ETX 1st byte の取得
                                                          これは C2A 形式/非 C2A 形式共通の TLM HEADER 仕様である */
#define DS_ISSLFMT_ETX_2ND_BYTE         (0x79)       /*!< ISSL 標準フォーマットの ETX 2nd byte の取得
                                                          これは C2A 形式/非 C2A 形式共通の TLM HEADER 仕様である */


/**
 * @brief  TLM Version の取得
 * @note   フレームが確定しているときに呼び出すこと
 * @note   これは非 C2A 形式で使われる TLM HEADER 仕様である
 * @param  p_stream_config: DriverSuper 構造体の DS_StreamConfig
 * @return バージョン（バージョン 0x01 では uint8_t だが将来性のために uint32_t で返す）
 */
uint32_t DS_ISSLFMT_get_tlm_version(const DS_StreamConfig* p_stream_config);

/**
 * @brief  TLM count の取得
 * @note   フレームが確定しているときに呼び出すこと
 * @note   これは非 C2A 形式で使われる TLM HEADER 仕様である
 * @param  version:         TLM Version
 * @param  p_stream_config: DriverSuper 構造体の DS_StreamConfig
 * @return TLM count（バージョン 0x01 では uint8_t だが将来性のために uint32_t で返す）
 */
uint32_t DS_ISSLFMT_get_tlm_count(const uint32_t version, const DS_StreamConfig* p_stream_config);

/**
 * @brief  TLM id の取得
 * @note   フレームが確定しているときに呼び出すこと
 * @note   これは非 C2A 形式で使われる TLM HEADER 仕様である
 * @param  version:         TLM Version
 * @param  p_stream_config: DriverSuper 構造体の DS_StreamConfig
 * @return id（バージョン 0x01 では uint16_t だが将来性のために uint32_t で返す）
 */
uint32_t DS_ISSLFMT_get_tlm_id(const uint32_t version, const DS_StreamConfig* p_stream_config);

/**
 * @brief  TLM length の取得
 * @note   フレームが確定しているときに呼び出すこと
 * @note   これは C2A 形式/非 C2A 形式共通の TLM HEADER 仕様である
 * @param  p_stream_config: DriverSuper 構造体の DS_StreamConfig
 * @return id（バージョン 0x01ではuint16_tだが将来性のためにuint32_tで返す）
 */
uint32_t DS_ISSLFMT_get_tlm_length(const DS_StreamConfig* p_stream_config);

/**
 * @brief  CRCの計算
 * @note   CRC-16-IBM を使う
 * @note   これは C2A 形式/非 C2A 形式共通の TLM HEADER 仕様である
 * @param  c: CRC を計算するデータのポインタ
 * @param  n: データ長
 * @return CRC
 */
uint16_t DS_ISSLFMT_calc_crc(const unsigned char* c, size_t n);

/**
 * @brief  受信フレームからユーザーデータの先頭ポインタを取得する
 * @note   フレームが確定しているときに呼び出すこと
 * @note   これは C2A 形式で使われる TLM HEADER 仕様である
 * @param  p_stream_config: DriverSuper 構造体の DS_StreamConfig
 * @return ユーザーデータの先頭ポインタ
 */
const uint8_t* DS_C2AFMT_get_user_data_head(const DS_StreamConfig* p_stream_config);

/**
 * @brief  CommonTlmPacket を受信データからコピーして取得する
 * @note   フレームが確定しているときに呼び出すこと
 * @note   これは C2A 形式で使われる TLM HEADER 仕様である
 * @param[in]  p_stream_config: DriverSuper 構造体の DS_StreamConfig
 * @param[out] ctp: 抽出したパケット
 * @retval DS_ERR_CODE_OK:  正常終了
 * @retval DS_ERR_CODE_ERR: パケット長が異常など，エラーでコピーできない
 */
DS_ERR_CODE DS_C2AFMT_get_ctp(const DS_StreamConfig* p_stream_config, CommonTlmPacket* ctp);

/**
 * @brief  CommonCmdPacket を受信データからコピーして取得する
 * @note   フレームが確定しているときに呼び出すこと
 * @note   これは C2A 形式で使われる TLM HEADER 仕様である
 * @param[in]  p_stream_config: DriverSuper 構造体の DS_StreamConfig
 * @param[out] ccp: 抽出したパケット
 * @retval DS_ERR_CODE_OK:  正常終了
 * @retval DS_ERR_CODE_ERR: パケット長が異常など，エラーでコピーできない
 */
DS_ERR_CODE DS_C2AFMT_get_ccp(const DS_StreamConfig* p_stream_config, CommonCmdPacket* ccp);

#endif
