/**
 * @file
 * @brief  DriverのためのISSLフォーマット通信のための公開定義，関数など
 */
#ifndef DRIVER_SUPER_ISSL_FORMAT_H_
#define DRIVER_SUPER_ISSL_FORMAT_H_

#include "driver_super.h"
#include <src_user/Library/stdint.h>


#define DS_ISSLFMT_STX_SIZE             (2)          //!< ISSL標準フォーマットのSTXのサイズ
#define DS_ISSLFMT_ETX_SIZE             (2)          //!< ISSL標準フォーマットのETXのサイズ
#define DS_ISSLFMT_LEN_SIZE             (2)          //!< ISSL標準フォーマットのLENのサイズ
#define DS_ISSLFMT_CRC_SIZE             (2)          //!< ISSL標準フォーマットのCRCのサイズ
#define DS_ISSLFMT_COMMON_HEADER_SIZE   (DS_ISSLFMT_STX_SIZE + DS_ISSLFMT_LEN_SIZE)  //!< ISSL標準フォーマットのコモンヘッダーのサイズ
#define DS_ISSLFMT_COMMON_FOOTER_SIZE   (DS_ISSLFMT_ETX_SIZE + DS_ISSLFMT_CRC_SIZE)  //!< ISSL標準フォーマットのコモンフッターのサイズ
#define DS_ISSLFMT_CMD_HEADER_SIZE      (4)          //!< ISSL標準フォーマットのCMDヘッダーのサイズ
#define DS_ISSLFMT_TLM_HEADER_SIZE      (4)          //!< ISSL標準フォーマットのTLMヘッダーのサイズ
#define DS_ISSLFMT_STX_1ST_BYTE         (0xEB)       /*!< ISSL標準フォーマットのSTX 1st byteの取得
                                                          これはC2A形式/非C2A形式共通のTLM HEADER仕様である  */
#define DS_ISSLFMT_STX_2ND_BYTE         (0x90)       /*!< ISSL標準フォーマットのSTX 2nd byteの取得
                                                          これはC2A形式/非C2A形式共通のTLM HEADER仕様である  */
#define DS_ISSLFMT_ETX_1ST_BYTE         (0xC5)       /*!< ISSL標準フォーマットのETX 1st byteの取得
                                                          これはC2A形式/非C2A形式共通のTLM HEADER仕様である  */
#define DS_ISSLFMT_ETX_2ND_BYTE         (0x79)       /*!< ISSL標準フォーマットのETX 2nd byteの取得
                                                          これはC2A形式/非C2A形式共通のTLM HEADER仕様である  */


// [TODO] 以下は本当はTCPの定義から持ってきたい．TCPの整理が終わったらそうする
#define DS_C2AFMT_TCP_CMD_PRIMARY_HEADER_SIZE     (6)     //!< C2A間通信のTCPCMDののPrimary header size
#define DS_C2AFMT_TCP_CMD_SECONDARY_HEADER_SIZE   (1)     //!< C2A間通信のTCPCMDののSecondary header size
#define DS_C2AFMT_TCP_TLM_PRIMARY_HEADER_SIZE     (6)     //!< C2A間通信のTCPのTLMのPrimary header size
#define DS_C2AFMT_TCP_TLM_SECONDARY_HEADER_SIZE   (7)     //!< C2A間通信のTCPのTLMのSecondary header size．ADU分割無し


/**
 * @brief  TLM Versionの取得
 * @note   フレームが確定しているときに呼び出すこと
 * @note   これは非C2A形式で使われるTLM HEADER仕様である
 * @param  p_stream_config DriverSuper構造体のDS_StreamConfig
 * @return バージョン（バージョン 0x01ではuint8_tだが将来性のためにuint32_tで返す）
 */
uint32_t DS_ISSLFMT_get_tlm_version(const DS_StreamConfig* p_stream_config);

/**
 * @brief  TLM countの取得
 * @note   フレームが確定しているときに呼び出すこと
 * @note   これは非C2A形式で使われるTLM HEADER仕様である
 * @param  version         TLM Version
 * @param  p_stream_config DriverSuper構造体のDS_StreamConfig
 * @return TLM count（バージョン 0x01ではuint8_tだが将来性のためにuint32_tで返す）
 */
uint32_t DS_ISSLFMT_get_tlm_count(const uint32_t version, const DS_StreamConfig* p_stream_config);

/**
 * @brief  TLM idの取得
 * @note   フレームが確定しているときに呼び出すこと
 * @note   これは非C2A形式で使われるTLM HEADER仕様である
 * @param  version         TLM Version
 * @param  p_stream_config DriverSuper構造体のDS_StreamConfig
 * @return id（バージョン 0x01ではuint16_tだが将来性のためにuint32_tで返す）
 */
uint32_t DS_ISSLFMT_get_tlm_id(const uint32_t version, const DS_StreamConfig* p_stream_config);

/**
 * @brief  TLM lengthの取得
 * @note   フレームが確定しているときに呼び出すこと
 * @note   これはC2A形式/非C2A形式共通のTLM HEADER仕様である
 * @param  p_stream_config DriverSuper構造体のDS_StreamConfig
 * @return id（バージョン 0x01ではuint16_tだが将来性のためにuint32_tで返す）
 */
uint32_t DS_ISSLFMT_get_tlm_length(const DS_StreamConfig* p_stream_config);

/**
 * @brief  CRCの計算
 * @note   CRC-16-IBMを使う
 * @note   これはC2A形式/非C2A形式共通のTLM HEADER仕様である
 * @param  c CRCを計算するデータのポインタ
 * @return n データ長
 */
uint16_t DS_ISSLFMT_calc_crc(const unsigned char* c, size_t n);

/**
 * @brief  TLM idの取得
 * @note   フレームが確定しているときに呼び出すこと
 * @note   これはC2A形式で使われるTLM HEADER仕様である
 * @param  p_stream_config DriverSuper構造体のDS_StreamConfig
 * @return id
 */
uint8_t DS_C2AFMT_get_tlm_id(const DS_StreamConfig* p_stream_config);

#endif
